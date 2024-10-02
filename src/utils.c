/* * * * * * * * * * * * * * * * * * * * */
/* @author Rodrigo Kenji Asato Kobayashi */
/* * * * * * * * * * * * * * * * * * * * */

#include "mysimplex.h"
#include <stdio.h>
#include <stdlib.h>

void printTableau(t_tableau *tableau){ // print para DEBUG
    int i, j;
    printf("\nP n = %d, m = %d",tableau->n, tableau->m);
    printf("\nF ");
    for(i = 0; i <= tableau->n; i++) printf("%6.3f ", tableau->obj[i]);
    for(i = 0; i < tableau->m; i++){
        printf("\nR ");
        for(j = 0; j <= tableau->n; j++) printf("%6.3f ", tableau->matriz[i][j]);
    }
    printf("\nB ");
    for(i = 0; i < tableau->m; i++) printf("%d ", tableau->base[i]);
    printf("\nT\n");
}

float **alocaMatriz(int linhas, int colunas){
    int i;
    float **matriz = (float**)malloc(linhas*sizeof(float*));
    for(i = 0; i < linhas; i++){
        matriz[i] = (float*)malloc(colunas*sizeof(float));
    }
    return matriz;
}

void desalocaMatriz(float **matriz, int linhas){
    int i;
    for(i = 0; i < linhas; i++){
        if(matriz[i] != NULL) free(matriz[i]);
    }        
    if(matriz != NULL) free(matriz);
}

void freeTableau(t_tableau *tableau){
    if(tableau->obj != NULL) free(tableau->obj);
    desalocaMatriz(tableau->matriz, tableau->m);
    if(tableau->base != NULL) free(tableau->base);
}

int readFile(int argc, char **argv, t_tableau *tableau){
    int i, j, T;
    float aux;
    FILE *fin;
    char stream[100];
    char ch;

    // check arguments
    if(argc < 2){
        printf("\nSintaxe: ./simples <modelo>\n");
        return 0;
    }

    // check file
    fin = fopen(argv[1], "r");
    if(!fin){
        printf("\nFile not found: %s\n", argv[1]);
        return 0;
    }

    i = 0, T = 0;
    // read file and fill *tableau
    while(!T){
        fscanf(fin, " %c", &ch);
        switch (ch)
        {
        case 'P': // Le os valores n, m e aloca matriz
            fscanf(fin, "%d %d", &(tableau->n), &(tableau->m));
            tableau->obj = (float*)malloc((tableau->n+1)*sizeof(float));
            tableau->matriz = alocaMatriz(tableau->m, tableau->n + 1);
            tableau->base = (int*)malloc(tableau->m*sizeof(int));
            break;
        case 'F':
            for(j = 0; j < tableau->n; j++){
                //fscanf(fin, "%f", &(tableau->obj[j]));
                fscanf(fin, "%f", &aux); // troca sinal da func objetivo para guardar no tableau
                tableau->obj[j] = -aux; //
            }
            break;
        case 'R':
            for(j = 0; j <= tableau->n; j++)
                fscanf(fin, "%f", &(tableau->matriz[i][j]));
            i++;
            break;
        case 'B':
            for(j = 0; j < tableau->m; j++)
                fscanf(fin, "%d", &(tableau->base[j]));
            break;
        case 'T':
            T = 1;
            tableau->obj[tableau->n] = 0;
            break;
        }
    }
    fclose(fin);
    if(!T){
        freeTableau(tableau);
        return 0;
    } 
    return 1;
}

int naBase(t_tableau *tableau, int vari){ // se estiver na base, retorna qual linha representa no tableau, cc retorna -1
    int i, ret = -1;
    for(i = 0; i < tableau->m && ret == -1; i++){
        if(vari == tableau->base[i]) ret = i;
    }
    return ret;
}

void printXCZ(t_tableau *tableau){
    int i, auxi;
    printf("X ");
    for(i = 0; i < tableau->n; i++){  // imprimir valores xi
        auxi = naBase(tableau, i + 1); // auxi = linha na matriz se estiver na base, -1 cc
        if(auxi >= 0){
          printf("%.3f ", tableau->matriz[auxi][tableau->n]);
        }else{
          printf("0.000 ");
        }
      }
      printf("\nC");
      for(i = 0; i < tableau->n; i++){ // imprimir custos reduzidos
        printf(" %.3f", -tableau->obj[i]);
      }
      printf("\nZ %.3f\n", tableau->obj[tableau->n]); // imprimir z
}

void printI(float *vCresc, int tam, int xi){ // imprime vetor do caso ilimitado
    int i;
    printf("I %d ", xi);
    for(i = 0; i < tam; i++){
        printf("%.3f ", vCresc[i]);
    }
    printf("\n");

}