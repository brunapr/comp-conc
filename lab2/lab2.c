#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"
#include "sequencial.c"

float *firstMatrix;
float *secondMatrix;
float *outputMatrix;
int nthreads;

typedef struct {
  int id; 
  int dim; 
} tArgs;

// funcao que as threads executarao 
void* multiply(void *arg) {
  tArgs *args = (tArgs*) arg;
  int initialLine = args->id; // linha inicial depende da thread que esta executando
  
  // linhas 
  for (int i = initialLine;  i < args->dim; i+=nthreads) {
    // colunas 
    for (int j = 0; j < args->dim; j++) {
      // linha da primeira matriz e coluna da segunda
      for (int k = 0; k < args->dim; k++) {
        outputMatrix[i*args->dim + j] += firstMatrix[i*args->dim + k] * secondMatrix[k*args->dim + j];
      }
    }
  }
  pthread_exit(NULL);
}

// calcula e exibe o tempo gasto em uma tarefa
void showTime(double start, char taskName[]) {
  double elapsed, finish;
  GET_TIME(finish);
  elapsed = finish - start;
  printf("Tempo de %s: %lf\n", taskName, elapsed);
}

int main(int argc, char* argv[]) {

  int dim; // dimensao das matrizes 
  pthread_t *tid; // identificadores das threads 
  tArgs *args; // idetificadores locais 
  double start; // controle de tempo

  if (argc < 3) {
    printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
    return 1;
  }

  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);
  if (nthreads > dim) nthreads=dim;

  GET_TIME(start);

  // alocacao de memoria
  firstMatrix = (float *) malloc(sizeof(float) * dim * dim);
  if (firstMatrix == NULL) { printf("ERRO--malloc\n"); return 2; }

  secondMatrix = (float *) malloc(sizeof(float) * dim * dim);
  if (secondMatrix == NULL) { printf("ERRO--malloc\n"); return 2; }

  outputMatrix = (float *) malloc(sizeof(float) * dim * dim);
  if (outputMatrix == NULL) { printf("ERRO--malloc\n"); return 2; }

  //inicializa matrizes
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      firstMatrix[i*dim + j] = 1;
      secondMatrix[i*dim + j] = 2;
      outputMatrix[i*dim + j] = 0;
    }
  }
  showTime(start, "alocacao e inicializacao");

  // criacao das threads, execucao da multiplicacao e espera pelo termino das threads
  GET_TIME(start);

  // alocacao das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if (tid == NULL) { puts("ERRO--malloc\n"); return 2; }

  args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
  if (args == NULL) { puts("ERRO--malloc\n"); return 2; }

  // cria as threads e faz a multiplicacao
  for (int i = 0; i < nthreads; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if (pthread_create(tid+i, NULL, multiply, (void*) (args+i))) { 
      puts("ERRO--pthread_create\n"); return 3;
    } 
  }

  // espera a multiplicacao terminar 
  for (int i = 0; i < nthreads; i++) {
    pthread_join(*(tid+i), NULL); 
  }

  showTime(start, "criacao das threads e multiplicacao (b)");

  GET_TIME(start);
  
  // liberacao da memoria
  free(firstMatrix);
  free(secondMatrix);
  free(outputMatrix);
  free(args);
  free(tid);

  showTime(start, "finalizacao");

  return 0;
}