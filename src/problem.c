/* * * * * * * * * * * * * * * * * * * * */
/* @author Rodrigo Kenji Asato Kobayashi */
/* * * * * * * * * * * * * * * * * * * * */

#include "mysimplex.h"
#include <stdio.h>
#include <stdlib.h>

int construivel(t_tableau *tableau){ // 
    int i, indice, ret = 1;
    float aux;
    for(i = 0; i < tableau->m && ret; i++){
        indice = tableau->base[i];
        aux = tableau->matriz[i][indice - 1];
        if(aux > -EPSILON && aux < EPSILON) ret = 0; 
    }
    return ret;
}

int viavel(t_tableau *tableau){ // assume-se pivoteado
    int i, ret = 1;
    for(i = 0; i < tableau->m && ret; i++){
        if(tableau->matriz[i][tableau->n] < - EPSILON) ret = 0;
    }
    return ret; // 1 = viavel; 0 = inviavel (achou negativo)
}

int otima(t_tableau *tableau){ // assume-se viavel
    int i, ret = 1;
    for(i = 0; i < tableau->n && ret; i++){
        //if(tableau->obj[i] < -EPSILON) ret = 0;  // (problema de maximizacao)
        if(tableau->obj[i] > EPSILON) ret = 0; // (problema de minimizacao)
    }
    return ret; // 1 = otimo; 0 = nao otimo
}

void entraNaBase(t_tableau *tableau, int vari, int linha){ //
    int i, j;
    float aux;
    // fazer a linha ser 1
    aux = tableau->matriz[linha][vari];
    if(aux < 1 - EPSILON || aux > 1 + EPSILON){ // aux != 1
        for(i = 0; i <= tableau->n; i++){
            if(i == vari) continue;
            tableau->matriz[linha][i] /= aux;
        }
        tableau->matriz[linha][vari] = 1.0;
    }

    // dividir ate as outras linhas serem 0
    for(i = 0; i < tableau->m; i++){
        if(i == linha) continue;
        aux = -tableau->matriz[i][vari];
        if(aux < -EPSILON || aux > EPSILON){ // aux != 0
            for(j = 0; j <= tableau->n; j++){
                if(j == vari) tableau->matriz[i][j] = 0.0;
                else tableau->matriz[i][j] += aux*(tableau->matriz[linha][j]);
            }
        }
    }
    aux = -tableau->obj[vari];
    if(aux < -EPSILON || aux > EPSILON){
        for(j = 0; j <= tableau->n; j++){
            if(j == vari) tableau->obj[j] = 0.0;
            else tableau->obj[j] += aux*(tableau->matriz[linha][j]);
        }
    }
}

void acharIlimitados(t_tableau *tableau, float *rCres, int *limitado){ // 1 = ilimitado, 0 = limitado
    int i, j, auxi, ilimitado;

    for(i = 0; i < tableau->n; i++){
        ilimitado = 1;
        if(tableau->obj[i] < EPSILON)  continue; //
        for(j = 0; j < tableau->m && ilimitado; j++){
            if(tableau->matriz[j][i] > EPSILON) ilimitado = 0;
        }
        if(ilimitado){
            *limitado = 0;
#ifdef DEBUG
printf("Ilimitado: %d\n", i + 1);
#endif
            rCres = (float*)malloc(tableau->n*sizeof(float));
            for(j = 0; j < tableau->n; j++){
                auxi = naBase(tableau, j + 1);
                if(auxi >= 0){
                    rCres[j] = -tableau->matriz[auxi][i]; //
                }else{
                    if(j == i){
                        rCres[j] = 1.0; // a que esta entrando na base
                    }else{
                        rCres[j] = 0.0; // ja esta fora da base(?)
                    }              
                }
            }
            printI(rCres, tableau->n, i + 1);
            free(rCres);
        }
    }
}

int min(float *vet, int tam){
    int i, j = 0;
    float aux = vet[0];
    for(i = 0; i < tam; i++){
        if(vet[i] < aux){
            aux = vet[i];
            j = i;
        }
    }
    return j;
}

int max(float *vet, int tam){
    int i, j = 0;
    float aux = vet[0];
    for(i = 0; i < tam; i++){
        if(vet[i] > aux){
            aux = vet[i];
            j = i;
        }
    }
    return j;
}

void acharLimitados(t_tableau *tableau, int *entra, int* sai){ // retorna quem decidiu entrar/sair alem de imprimir o caso Limitado (assume-se que nao eh ilimitado nesta iteracao)
    int i, j, k, tam;
    float auxf, minimo;
    int *vet;

    vet = (int*)malloc(tableau->n*sizeof(int));
    //*entra = min(tableau->obj, tableau->n); // (problema de maximizacao) escolhe qm entra
    *entra = max(tableau->obj, tableau->n); // (problema de minimizacao) escolhe qm entra de forma gulosa

    for(i = 0; i < tableau->n; i++){
        tam = 0;
        if(tableau->obj[i] < EPSILON) continue; //
        for(j = 0; j < tableau->m; j++){            
            if(tableau->matriz[j][i] > EPSILON){ // (problema de minimizacao) se entrar, tem custo reduzido apropriado, ou seja, negativo(positivo no tableau)
                auxf = tableau->matriz[j][tableau->n]/tableau->matriz[j][i];
                if(tam == 0){
                    if(auxf > -EPSILON){ // vet sao candidatos para entrada, alem do menor valor positivo, admite-se zero pelo caso de b zero(degenerado)
                        vet[tam++] = tableau->base[j];
                        minimo = auxf;
                    }
                }else if(tam > 0 && auxf - minimo > -EPSILON && auxf - minimo < EPSILON){ // se valor foi igual, aumenta o vet
                    vet[tam++] = tableau->base[j];
                }else if(tam > 0 && auxf - minimo < -EPSILON){ // se valor foi melhor, atualiza vetor para novo min
                    vet[0] = tableau->base[j];
                    tam = 1;
                    minimo = auxf;
                }
            }            
        }        
        
        if(i == *entra){
            if(vet[0] == 0){
                *sai = 0;
            }else{
                *sai = vet[0];
            }            
        }
        
        printf("L %d %d ", i+1, tam); 
        for(k = 0; k < tam; k++){
            printf("%d ", vet[k]);
        }
        printf("\n");
        
    }
    free(vet);
}