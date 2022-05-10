#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int dim; // dimensao do vetor de entrada
int nthreads; // numero de threads
int global; // variavel compartilhada
int *vetorEntrada; 
float *vetorSeqSaida;
float *vetorConSaida;
pthread_mutex_t lock; //variavel de lock para exclusao mutua

int ehPrimo(long long int n) {
  if (n<=1) return 0;
  if (n==2) return 1;
  if (n%2==0) return 0;
  for (int i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

// funcao sequencial
void processaPrimosSeq(int **vetorEntrada, long long int dim) {
  int primo = 1;
  for(int i=0; i<dim; i++) {
    int n = (*vetorEntrada)[i];
    if (n<=1) primo = 0;
    if (n==2) primo = 1;
    if (n%2==0) primo = 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
      if(n%i==0) primo = 0;
    if (primo == 1)
      vetorSeqSaida[i] = sqrt((*vetorEntrada)[i]);
    else
      vetorSeqSaida[i] = (*vetorEntrada)[i];
  };
}

void* processaPrimosCon(void *arg) {
  pthread_mutex_lock(&lock);
  int i = global; global++;
  pthread_mutex_unlock(&lock);

  while(i < dim) {
    if (ehPrimo(vetorEntrada[i]))
      vetorConSaida[i] = sqrt(vetorEntrada[i]);
    else
      vetorConSaida[i] = vetorEntrada[i];
    pthread_mutex_lock(&lock);
    i = global; global++;
    pthread_mutex_unlock(&lock);
  }
  // for(i; i<dim, i++) {
  //   n = vetorEntrada[i]
  //   if (ehPrimo(vetorEntrada[i])
  //     vetorSaida[i] = sqrt(vetorEntrada[i]);
  //   else
  //     vetorSaida[i] = vetorEntrada[i];
  // }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  // pthread_t *tid;
  pthread_t tid[NTHREADS];
  int t, id[NTHREADS];
  // double ini, fim;

  // recebe e valida os parametros
  if(argc < 3) {
    fprintf(stderr, "Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
    return 1;
  }

  dim = atoll(argv[1]);
  nthreads = atoi(argv[2]);

  // alocacao de memoria dos vetores
  vetorEntrada = (int*) malloc(sizeof(int)*dim);
  if(vetorEntrada == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }
  vetorSeqSaida = (float*) malloc(sizeof(float)*dim);
  if(vetorSeqSaida == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }
  vetorConSaida = (float*) malloc(sizeof(float)*dim);
  if(vetorConSaida == NULL) { fprintf(stderr, "ERRO--malloc\n"); return 2; }

  // preenche o vetor de entrada com numeros aleatorios de 0 a 100
  srand(time(NULL));

  for(long int i = 0; i < dim; i++) {
    vetorEntrada[i] = ((int) rand())%100;
  };

  // faz o calculo sequencial
  processaPrimosSeq(&vetorEntrada, dim);

  //inicilaiza o mutex (lock de exclusao mutua)
  pthread_mutex_init(&lock, NULL);

  // cria as threads
  for(t=0; t<nthreads; t++) {
    id[t]=t;
    if (pthread_create(&tid[t], NULL, processaPrimosCon, (void *) &id[t])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //espera todas as threads terminarem
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  } 

  pthread_mutex_destroy(&lock);

  for(int i = 0; i < dim; i++) {
    if(vetorConSaida[i] != vetorSeqSaida[i]) {
      printf("diferente: %f \n", vetorConSaida[i]);
    }    
  };
  
  free(vetorEntrada);
  free(vetorSeqSaida);
  
  return 0;
}