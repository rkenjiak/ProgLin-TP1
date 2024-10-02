/* * * * * * * * * * * * * * * * * * * * */
/* @author Rodrigo Kenji Asato Kobayashi */
/* * * * * * * * * * * * * * * * * * * * */

#ifndef __MYSIMPLEX_H__
#define __MYSIMPLEX_H__

#define EPSILON 0.00001 //10^-5

typedef struct _tableau
{
    int m;           // numero de restricoes
    int n;           // numero de variaveis
    float *obj;      // 1 x n+1
    float **matriz;  // m x n+1 // [][n] é b
    int *base;       // 1 x m 
} t_tableau;

void printTableau(t_tableau *tableau);
int  readFile(int argc, char **argv, t_tableau *tableau);
void freeTableau(t_tableau *tableau);
int  naBase(t_tableau *tableau, int vari);
void printXCZ(t_tableau *tableau);
void printI(float *vCresc, int tam, int xi);

int  construivel(t_tableau *tableau);
int  viavel(t_tableau *tableau);
int  otima(t_tableau *tableau);
void entraNaBase(t_tableau *tableau, int vari, int linha);
void acharIlimitados(t_tableau *tableau, float *rCres, int *limitado);
void acharLimitados(t_tableau *tableau, int *entra, int* sai);

#endif