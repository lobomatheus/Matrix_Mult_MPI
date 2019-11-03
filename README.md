# Matrix_Mult_MPI
Trabalho de implementação de algoritmo para multiplicação de matrizes utilizando o MPI para a disciplina de Sistemas Distribuídos em 2019.2

- serial.c: Versão serial da multiplicação de matrizes
- paralelo_int.c: Versão paralela da multiplicação de matrizes de inteiros (usada apenas para teste)
- paralelo_float_1000.c: Versão paralela da multiplicação de matrizes de float de ordem 1000 x 1000
- paralelo_float_5000.c: Versão paralela da multiplicação de matrizes de float de ordem 5000 x 5000
- paralelo_float_10000.c: Versão paralela da multiplicação de matrizes de float de ordem 10000 x 10000

## Compilação e execução
1.  Para compilar: "mpicc -o NOME_DO_EXECUTÁVEL NOME_DO_CÓDIGO"
2.  Para executar: "mpirun -np NÚMERO_DE_PROCESSOS NOME_DO_EXECUTÁVEL"
