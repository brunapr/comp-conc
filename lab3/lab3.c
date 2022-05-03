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
} infoVetor;

void* tarefa(void *arg) {
  long int id = (long int) arg; // identificador da thread
  long long int blockSize = N / nthreads; // tamanho do bloco de cada thread
  long long int ini = id * blockSize; // elemento inicial do bloco da thread
  long long int fim; // elemento final (nao processado) do bloco da thread

  // alocacao da estrutura de retorno da thread
  infoVetor *conValues; 
  conValues = (infoVetor *) malloc(sizeof(infoVetor)); 
  if(conValues == NULL) { fprintf(stderr, "ERRO--malloc\n"); exit(1); }

  // trata o resto se houver
  if (id == nthreads - 1) fim = N; 
  else fim = ini + blockSize;

  // inicializa o maior e menor elemento como sendo o primeiro elemento do bloco da thread
  conValues->max = vetor[ini];
  conValues->min = vetor[ini];

  // percorre o bloco da thread para encontrar o maior e menor do bloco
  for (long long int i = ini; i < fim; i++) {
    if (vetor[i] > conValues->max) {
      conValues->max = vetor[i];
    } else if (vetor[i] < conValues->min) {
      conValues->min = vetor[i];
    } 
  }

  // retorna as informacoes do vetor
  return((void *) conValues);
}

int main(int argc, char *argv[]) {
  infoVetor *seqValues, *conValues, *retorno;
  pthread_t *tid;
  double start, finish;

  // recebe e valida os parametros
  if(argc < 3) {
    fprintf(stderr, "Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
    return 1;
  }

  N = atoll(argv[1]);
  nthreads = atoi(argv[2]);
  if (nthreads > N) nthreads = N;

  // alocacao de memoria
  vetor = (float*) malloc(sizeof(float)*N);
  if(vetor == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  seqValues = (infoVetor*) malloc(sizeof(infoVetor*)*N);
  if(seqValues == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  conValues = (infoVetor*) malloc(sizeof(infoVetor*)*N);
  if(conValues == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  // preenche o vetor com numeros aleatorios
  for(int i = 0; i < N; i++) {
    vetor[i] = rand()%1000;
  }

  // valores de min e max inicializados
  conValues->max = conValues->min = seqValues->max = seqValues->min = vetor[0];

  // calculo sequencial
  GET_TIME(start);

  for (long long int i = 1; i < N; i++) {
    if(vetor[i] > seqValues->max) {
      seqValues->max = vetor[i];
    }
    if(vetor[i] < seqValues->min) {
      seqValues->min = vetor[i];
    }
  }

  GET_TIME(finish);
  printf("Tempo sequencial: %lf\n", finish-start);

  // cria as threads e aloca a estrutura
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if( tid == NULL ) { fprintf(stderr, "ERRO--malloc\n"); return 2;}
  
  GET_TIME(start);

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
    if (retorno->max > conValues->max) {
      conValues->max = retorno->max;
    }
    if (retorno->min < conValues->min) {
      conValues->min = retorno->min;
    }
  }
  GET_TIME(finish);
  printf("Tempo concorrente: %lf\n", finish-start);

  // testa se as saidas estao iguais
  if(seqValues->min == conValues->min && seqValues->max == conValues->max) {
    printf("Saidas conferem.\n");
  } else {
    printf("Erro nas saidas.\n");
    printf("Sequencial ( min: %.lf | max: %.lf )\n", seqValues->min, seqValues->max);
    printf("Concorrente ( min: %.lf | max: %.lf )\n", conValues->min, conValues->max);
    return 2;
  }

  // exibir os resultados
  // printf("Valor minimo: %.lf\nValor maximo: %.lf\n", conValues->min, conValues->max);

  free(vetor);
  free(tid);
  
  return 0;
}