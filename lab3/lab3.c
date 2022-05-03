#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long long int N; // dimensao do vetor de entrada
int nthreads; // numero de threads
float *vetor; // vetor de entrada 

typedef struct {
  float max; // maior elemento do vetor
  float min; // menor elemento do vetor
} infoArray;

void* tarefa(void *arg) {
    long int id = (long int) arg; // identificador da thread
    long long int blockSize = N / nthreads; // tamanho do bloco de cada thread
    long long int first = id * blockSize; // elemento inicial do bloco da thread
    long long int end; // elemento final (nao processado) do bloco da thread

    // Alocacao da estrutura de retorno da thread
    infoArray *conMaxMin; // armazena o maior e menor elementos encontrados
    conMaxMin = (infoArray *) malloc(sizeof(infoArray)); 
    if(conMaxMin == NULL){
        fprintf(stderr, "ERRO--malloc\n"); 
        exit(1);
    }

    // Se houver resto a ultima thread que vai processa-lo
    if (id == nthreads - 1) end = N; 
    else end = first + blockSize;

    // Inicializando o maior e menor elemento como sendo o primeiro elemento do bloco da thread
    conMaxMin->max = vetor[first];
    conMaxMin->min = vetor[first];

    // Percorre o bloco da thread para encontrar o maior e menor do bloco
    for (long long int i = first; i < end; i++) {
        if (vetor[i] > conMaxMin->max) {
            conMaxMin->max = vetor[i];
        } else if (vetor[i] < conMaxMin->min) {
            conMaxMin->min = vetor[i];
        } 
    }

    // retorna as informacoes do array
    return((void *) conMaxMin);
}

int main(int argc, char *argv[]) {
    infoArray *seqMaxMin, *conMaxMin, *retorno;
    pthread_t *tid;
    // double start, finish, elapsed;

    // recebe e valida os parametros
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > N) nthreads=N;

    // alocacao de memoria
    vetor = (float*) malloc(sizeof(float)*N);
    if(vetor == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

    seqMaxMin = (infoArray*) malloc(sizeof(infoArray*)*N);
    if(seqMaxMin == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

    conMaxMin = (infoArray*) malloc(sizeof(infoArray*)*N);
    if(conMaxMin == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

    // preenche o vetor com numeros aleatorios
    for(int i = 0; i < N; i++) {
        vetor[i] = rand()%1000;
    }

    // valores de min e max inicializados
    conMaxMin->max = conMaxMin->min = seqMaxMin->max = seqMaxMin->min = vetor[0];

    // calculo sequencial
    for (long long int i = 1; i < N; i++) {
        if(vetor[i] > seqMaxMin->max) {
            seqMaxMin->max = vetor[i];
        }
        if(vetor[i] < seqMaxMin->min) {
            seqMaxMin->min = vetor[i];
        }
    }

    // cria as threads e aloca a estrutura
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if( tid == NULL ) { fprintf(stderr, "ERRO--malloc\n"); return 2;}
    
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    // aguarda o termino
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)) {
            fprintf(stderr, "ERRO--phtread_join\n");
            exit(4);
        }

        // atualiza os valores minimos e maximos com as saidas das threads
        if (retorno->max > conMaxMin->max) {
            conMaxMin->max = retorno->max;
        }
        if (retorno->min < conMaxMin->min) {
            conMaxMin->min = retorno->min;
        }
    }

    // testa se as saidas estao iguais
    if(seqMaxMin->min == conMaxMin->min && seqMaxMin->max == conMaxMin->max) {
        printf("Saidas conferem.\n");
    } else {
        printf("Erro nas saidas.\n");
        printf("Sequencial ( min: %.lf | max: %.lf )\n", seqMaxMin->min, seqMaxMin->max);
        printf("Concorrente ( min: %.lf | max: %.lf )\n", conMaxMin->min, conMaxMin->max);
        return 2;
    }

    // exibir os resultados
    printf("Valor minimo: %.lf\nValor maximo: %.lf\n", conMaxMin->min, conMaxMin->max);

    free(vetor);
    free(tid);
    
    return 0;
}