#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 5 // uma thread por mensagem

int total = 0; //variavel compartilhada entre as threads
sem_t cond_vinda, cond_volte; //semaforos para sincronizar a ordem de execucao das threads

void *volteSempre (void *t) {
  while(total < 3){
    sem_wait(&cond_volte);
  }

  printf("Volte sempre!\n");
  
  pthread_exit(NULL);
}

void *aVontade (void *t) {
  if(total == 0){
    sem_wait(&cond_vinda);
  }

  printf("Fique a vontade.\n");
  total++;

  sem_post(&cond_volte);

  pthread_exit(NULL);
}

void *senteSe (void *t) {
  if(total == 0){
    sem_wait(&cond_vinda);
  }

  printf("Sente-se por favor.\n");
  total++;

  sem_post(&cond_volte); 

  pthread_exit(NULL);
}

void *copoAgua (void *t) {
  if(total == 0){
    sem_wait(&cond_vinda);
  }

  printf("Aceita um copo d'agua?\n");
  total++;

  sem_post(&cond_volte); 

  pthread_exit(NULL);
}

void *bemVindo (void *t) {
  printf("Seja bem-vindo! \n");
  
  for(int i = 0; i < 3; i++){
    sem_post(&cond_vinda);
  }

  pthread_exit(NULL);
}

// criar as threads
void createThreads(pthread_t *tids, int nthreads) {
  int error;

  error = pthread_create(&tids[0], NULL, volteSempre, NULL);
  error = pthread_create(&tids[1], NULL, aVontade, NULL);
  error = pthread_create(&tids[2], NULL, senteSe, NULL);
  error = pthread_create(&tids[3], NULL, copoAgua, NULL);
  error = pthread_create(&tids[4], NULL, bemVindo, NULL);

  if (error) {
    fprintf(stderr, "ERRO--pthread_create()\n");
    exit(1);
  }
}

// aguarda o termino das threads
void joinThreads(pthread_t *tids, int nthreads) {
  for (int i = 0; i < nthreads; i++) {
    if (pthread_join(tids[i], NULL)) {
      fprintf(stderr, "ERRO--pthread_join()\n");
      exit(2);
    }
  }
}

int main(int argc, char *argv[]) {
  pthread_t tids[NTHREADS];
  
  // inicializa as variaveis de semaforo
  sem_init (&cond_vinda, 0, 0);
  sem_init (&cond_volte, 0, 1);

  createThreads(tids, NTHREADS);
  joinThreads(tids, NTHREADS);

  return 0;
}