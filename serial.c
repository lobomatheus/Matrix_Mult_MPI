#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* geraMatriz (int ordem); //retorna uma matriz aleatória
int* multMat (int *m1, int *m2, int ordem); //dadas m1 e m2 de mesma ordem, retorna m1*m2
int getPos (int *mat, int ordem, int l, int c); //retorna o elemento da linha l e coluna c
int printMat (int *mat, int ordem); //imprime a matriz

int main (){
    int ordem = 3;
    int *m1, *m2, *r;

    m1 = geraMatriz(ordem);
    m2 = geraMatriz(ordem);
    r = multMat(m1, m2, ordem);
    printMat(m1, ordem);
    printMat(m2, ordem);
    printMat(r, ordem);
}

int* geraMatriz (int ordem){
    int *mat = (int*)malloc(ordem*ordem*sizeof(int));
    int i;

    srand(time(NULL));
    for(i=0; i<(ordem*ordem); i++)
        mat[i] = rand()%1000;

    return mat;
}

int* multMat (int *m1, int *m2, int ordem){
    int i, j, k, aux=0;
    int *mat = (int*)malloc(ordem*ordem*sizeof(int));

    for (i=0; i<ordem; i++){
        for (k=0; k<ordem; k++){
            for (j=0; j<ordem; j++)
                aux += getPos(m1, ordem, i, j)*getPos(m2, ordem, j, k);
            mat[i*ordem+k] = aux;
            aux = 0;
        }
    }

    return mat;
}

int getPos (int *mat, int ordem, int l, int c){
    return mat[l*ordem+c];
}

int printMat (int *mat, int ordem){
    int i;

    for (i=0; i<(ordem*ordem); i++){
        printf("%d ", mat[i]);
        if(i>0 && i%ordem==(ordem-1))
            printf("\n");
    }
    printf("\n");
}