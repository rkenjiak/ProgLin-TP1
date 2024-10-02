/* * * * * * * * * * * * * * * * * * * * */
/* @author Rodrigo Kenji Asato Kobayashi */
/* * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>

#include "mysimplex.h"

/*
#ifdef DEBUG
printTableau(&tableau);
#endif
*/
int main(int argc, char **argv){
    int i, j, auxi, limitado , entra, sai, brk;
    float *rCres;
    t_tableau tableau;

    limitado = 1;
    brk = 0;

    if(!readFile(argc, argv, &tableau)){
      printf("\nError reading file.\n");
      return 0;
    }
#ifdef DEBUG
printf("\n## Tableau lido do arquivo:");
printTableau(&tableau);
#endif

    if(!construivel(&tableau)){
      printf("E 1\n");
      brk = 1;
    }
    // do -> pivoteamento
    for(i = 0; i < tableau.m; i++){
      entraNaBase(&tableau, tableau.base[i] - 1, i);
    }
#ifdef DEBUG
printf("## Tableau apos pivotear a primeira base:");
printTableau(&tableau);
#endif
    // 
    if(!viavel(&tableau) && !brk){ // nao viavel
      printf("V N\n");
    }else if(!brk){ // viavel
      printf("V S\n");
      do{        
        printXCZ(&tableau);        
        if(otima(&tableau)){ // otima
          printf("O S\n");
          break;
        }else{
          printf("O N\n");

          // checar casos ilimitados
          acharIlimitados(&tableau, rCres, &limitado); // acha e imprime caso ilimitado
          if(!limitado) break; // se ilimitado sai

          //checar casos limitados caso nao for ilimitado
          acharLimitados(&tableau, &entra, &sai);
          printf("T %d %d\n", entra + 1, sai);          
  
          entraNaBase(&tableau, entra, naBase(&tableau, sai));
          tableau.base[naBase(&tableau, sai)] = entra + 1; // atualiza base guardada
#ifdef DEBUG
printf("## Tableau apos entrar %d e sair %d:", entra + 1, sai);
printTableau(&tableau);
#endif
        }
      }while(1);
    }
#ifdef DEBUG
printf("\n ## FIM ##");
printTableau(&tableau);
#endif
    freeTableau(&tableau);
    return 0;
}