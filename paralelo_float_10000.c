#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define LIMITE_RAND 1000
#define TAG_NLINHAS 1
#define TAG_M1 2
#define TAG_M2 3
#define TAG_RESULT 4

float* geraMatriz (int ordem);//retorna uma matriz aleatória
float* multMat (float *m1, float *m2, int l1, int ordem); //dadas m1 e m2 de mesma ordem, retorna m1*m2. l1 é o numero de linhas de m1
float getPos (float *mat, int ordem, int l, int c);//retorna o elemento da linha l e coluna c
void printMat (float *mat, int ordem); //imprime a matriz
int* vetLinhas (int n_procs, int ordem);//retorna um vetor onde vet[i] = num de linhas de m1 que o processo i tratará
void gravaArq (int ordem, int n_procs, double tempo);//salva os dados da execução no txt

int main (int argc, char **argv){
    int ordem = 10000;/*atoi(argv[1]);*/
    int n_procs, id, *n_linhas, i, linhas, ant=0;
    float *m1, *m2, *result;
    double T_ini, T_fim;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);//n_procs = numero de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);//id = id do processo atual (0 é o mestre)

    if (id == 0){
        m1 = geraMatriz(ordem);
        m2 = geraMatriz(ordem);
        result = (float*)malloc(ordem*ordem*sizeof(float));

        n_linhas = vetLinhas(n_procs, ordem);//vetor com a info do n de linhas que cada processo tratará
        
        ant = n_linhas[0]*ordem;//n de elementos da primeira matriz enviada
        T_ini = MPI_Wtime(); //começa a cronometrar a execução
        for (i=1; i<n_procs; i++){
            MPI_Send(&n_linhas[i], 1, MPI_INT, i, TAG_NLINHAS, MPI_COMM_WORLD);//passando o n de linhas que o processo terá que multiplicar pela m2
            MPI_Send(m1+ant, n_linhas[i]*ordem, MPI_FLOAT, i, TAG_M1, MPI_COMM_WORLD);//enviando as linhas de m1 correspondentes a cada processo
            MPI_Send(m2, ordem*ordem, MPI_FLOAT, i, TAG_M2, MPI_COMM_WORLD);//enviando m2
            ant += n_linhas[i]*ordem;
        }

        result = multMat(m1, m2, n_linhas[0], ordem);//mestre multiplica a primeira parte

        //recebe o resultado de cada processo
        ant = n_linhas[0]*ordem;
        for (i=1; i<n_procs; i++){
            MPI_Recv(result+ant, n_linhas[i]*ordem, MPI_FLOAT, i, TAG_RESULT, MPI_COMM_WORLD, &status);
            ant += (n_linhas[i]*ordem);
        }
        T_fim = MPI_Wtime();//fim da contagem do tempo

        /*printMat(m1, ordem);
        printMat(m2, ordem);
        printMat(result, ordem);*/
        //gravaArq(ordem, n_procs, T_fim-T_ini);

        free(m1);
        free(m2);
        free(result);
    }
    else{
        MPI_Recv(&linhas, 1, MPI_INT, 0, TAG_NLINHAS, MPI_COMM_WORLD, &status);//recebendo n_linhas

        m1 = (float*)malloc(linhas*ordem*sizeof(float));//m1 é (n_linhasXordem)
        m2 = (float*)malloc(ordem*ordem*sizeof(float));//m2 é (ordemXordem)

        MPI_Recv(m1, linhas*ordem, MPI_FLOAT, 0, TAG_M1, MPI_COMM_WORLD, &status);//recebendo as devidas linhas de m1
        MPI_Recv(m2, ordem*ordem, MPI_FLOAT, 0, TAG_M2, MPI_COMM_WORLD, &status);//recebendo m2

        float *aux = multMat(m1, m2, linhas, ordem);

        MPI_Send(aux, linhas*ordem, MPI_FLOAT, 0, TAG_RESULT, MPI_COMM_WORLD);//envia o resultado da multiplicação para o mestre
        free(aux);
    }

    MPI_Finalize();
    return 0;
}

float* geraMatriz (int ordem){
    float *mat = (float*)malloc(ordem*ordem*sizeof(float));
    int i;

    srand(time(NULL));
    for(i=0; i<(ordem*ordem); i++)
        mat[i] = rand()%LIMITE_RAND;

    return mat;
}

float* multMat (float *m1, float *m2, int l1, int ordem){
    int i, j, k;
    float aux=0;
    float *mat = (float*)malloc(ordem*ordem*sizeof(float));

    for (i=0; i<l1; i++){
        for (k=0; k<ordem; k++){
            for (j=0; j<ordem; j++)
                aux += getPos(m1, ordem, i, j)*getPos(m2, ordem, j, k);
            mat[i*ordem+k] = aux;
            aux = 0;
        }
    }

    return mat;
}

float getPos (float *mat, int ordem, int l, int c){
    return mat[l*ordem+c];
}

void printMat (float *mat, int ordem){
    int i;

    for (i=0; i<(ordem*ordem); i++){
        printf("%f ", mat[i]);
        if(i>0 && i%ordem==(ordem-1))
            printf("\n");
    }
    printf("\n");
}

int* vetLinhas (int n_procs, int ordem){
    int *n_linhas = (int*)malloc(n_procs*sizeof(int)), i, linhas, resto;

    linhas = ordem/n_procs;//n de linhas por processo
    resto = ordem%n_procs;
    for (i=0; i<n_procs; i++){
        if (i<resto)
            n_linhas[i] = linhas+1;
        else
            n_linhas[i] = linhas;
    }
    return n_linhas;
}

void gravaArq (int ordem, int n_procs, double tempo){
    FILE *arq;
    char nome[20];

    sprintf(nome, "%dX%d.txt", ordem, ordem);
    arq = fopen(nome, "a");

    fprintf(arq, "Matriz: %d X %d - Processos: %d - Tempo: %f s\n", ordem, ordem, n_procs, tempo);
    fclose(arq);
}