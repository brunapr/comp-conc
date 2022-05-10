#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int dim, global = 0; // dimensao do vetor de entrada
int nthreads, *vetor; // threads, vetor de entrada
float *vetorSeq, *vetorCon; // vetores de saida
pthread_mutex_t lock; // variavel de lock para exclusao mutua

int ehPrimo(long long int n) {
  if (n<=1) return 0;
  if (n==2) return 1;
  if (n%2==0) return 0;
  for (int i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

// funcao sequencial
void processaPrimosSeq(int *vetor, long long int dim) {
  for(long long int i=0; i<dim; i++) {
    if (ehPrimo(vetor[i]))
      vetorSeq[i] = sqrt(vetor[i]);
    else
      vetorSeq[i] = vetor[i];
  };
}

void* processaPrimosCon() {
  pthread_mutex_lock(&lock);
  int i = global; global++;
  pthread_mutex_unlock(&lock);

  while(i < dim) {
    if (ehPrimo(vetor[i]))
      vetorCon[i] = sqrt(vetor[i]);
    else
      vetorCon[i] = vetor[i];

    pthread_mutex_lock(&lock);
    i = global; global++;
    pthread_mutex_unlock(&lock);

  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t *tid;
  long int t;
  double ini, fim;

  // recebe e valida os parametros
  if(argc < 3) {
    fprintf(stderr, "Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
    return 1;
  }

  dim = atoll(argv[1]);
  nthreads = atoi(argv[2]);

  // alocacao de memoria dos vetores
  vetor = (int*) malloc(sizeof(int)*dim);
  if(vetor == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  vetorSeq = (float*) malloc(sizeof(float)*dim);
  if(vetorSeq == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }
  
  vetorCon = (float*) malloc(sizeof(float)*dim);
  if(vetorCon == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  // preenche o vetor de entrada com numeros aleatorios de 0 a 100
  srand(time(NULL));

  for(long int i = 0; i < dim; i++) {
    vetor[i] = rand() % dim;
  };

  // faz o calculo sequencial
  GET_TIME(ini);
  processaPrimosSeq(vetor, dim);
  GET_TIME(fim);
  printf("Tempo sequencial: %lf\n", fim - ini);

  GET_TIME(ini);
  //inicilaiza o mutex (lock de exclusao mutua)
  pthread_mutex_init(&lock, NULL);

  // cria as threads e aloca a estrutura
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if( tid == NULL ) { fprintf(stderr, "ERRO--malloc\n"); return 2;}

  // cria as threads

  for(t=0; t<nthreads; t++) {
    if (pthread_create(tid+t, NULL, processaPrimosCon, NULL)) {
      printf("--ERRO: pthread_create()\n"); return 2;
    };
  };

  //espera todas as threads terminarem
  for (t=0; t<nthreads; t++) {
    if (pthread_join(*(tid+t), NULL)) {
         printf("--ERRO: pthread_join() \n"); return 2; 
    } 
  } 
  
  pthread_mutex_destroy(&lock);

  GET_TIME(fim);
  printf("Tempo concorrente: %lf\n", fim - ini);
  
  free(vetor);
  free(tid);
  
  return 0;
}