#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define L 4
#define E 4

#define NTHREADS L+E

// Variaveis globais
sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais

//funcao executada pelos leitores
void *Leitor (void *threadid) {
  int tid =  * (int*) threadid;

  while(1) {
   printf("L[%d] quer ler.\n", tid);
   sem_wait(&leit);
   sem_wait(&em_l); l++;

   if(l == 1) sem_wait(&escr);

   sem_post(&em_l);
   sem_post(&leit);

   //le...
   printf("L[%d] esta lendo...\n", tid);
   sem_wait(&em_l); l--;
   printf("L[%d] terminou de ler.\n", tid);

   if(l == 0) sem_post(&escr);

   sem_post(&em_l);
   sleep(1);
  }

  pthread_exit(NULL);
}

//funcao executada pelos escritores
void *Escritor (void *threadid) {
  int tid = *(int*) threadid;

  while(1) {
   printf("E[%d] quer escrever.\n", tid);
   sem_wait(&em_e); e++;

   if(e == 1) sem_wait(&leit);

   sem_post(&em_e);
   sem_wait(&escr);

   //escreve...
   printf("E[%d] esta escrevendo...\n", tid);
   sem_post(&escr);
   sem_wait(&em_e); e--;
   printf("E[%d] terminou de escrever.\n", tid);

   if(e == 0) sem_post(&leit);
   
   sem_post(&em_e);
   sleep(1);
  }

  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  pthread_t tid[NTHREADS];
  int t, id[NTHREADS];
  
  //inicia os semaforos
  sem_init(&leit, 0, 1);
  sem_init(&escr, 0, 1);
  sem_init(&em_l, 0, 1);
  sem_init(&em_e, 0, 1);

  //cria as threads
  for (t=0; t<E; t++) {
     id[t]=t;
     if (pthread_create(&tid[t], NULL, Escritor, (void *)&id[t])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  }

  for (t=L; t<NTHREADS; t++) {
     id[t]=t-L;
     if (pthread_create(&tid[t], NULL, Leitor, (void *)&id[t])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  }  

  pthread_exit(NULL);
  return 0;
}
