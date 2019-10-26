#include <stdio.h>
#include <stdlib.h>

int* geraMatriz (int ordem); //retorna uma matriz aleatória
int* multMat (int *m1, int *m2, int ordem); //dadas m1 e m2 de mesma ordem, retorna m1*m2
int getPos (int *mat, int ordem, int l, int c); //retorna o elemento da linha l e coluna c
int printMat (int *mat, int ordem); //imprime a matriz

int main (){
    int m1[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}, m2[]={16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

    int *r;
    r = multMat(m1, m2, 4);
    printMat(r, 4);
}

int* geraMatriz (int ordem){
    int *mat = (int*)malloc(ordem*ordem*sizeof(int));
    int i;

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
}