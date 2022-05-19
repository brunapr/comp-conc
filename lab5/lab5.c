#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// uma thread por mensagem
#define NTHREADS 5

int total = 0; 
pthread_mutex_t mutex; 
pthread_cond_t cond_vinda, cond_volte; // variaveis de controle

void *volteSempre (void *t) {
  pthread_mutex_lock(&mutex);
  while (total < 4) { // espera as outras threads
    pthread_cond_wait(&cond_volte, &mutex);
  }
  pthread_mutex_unlock(&mutex); 

  printf("Volte sempre!\n");
  return NULL;
}

void *aVontade (void *t) {
  pthread_mutex_lock(&mutex);
  if (total == 0) { // espera a thread 5
    pthread_cond_wait(&cond_vinda, &mutex);
  }
  pthread_mutex_unlock(&mutex); 

  printf("Fique a vontade.\n");

  pthread_mutex_lock(&mutex);
  total++; // atualiza o total de mensagens
  pthread_cond_signal(&cond_volte); // sinaliza caso a thread 1 esteja esperando
  pthread_mutex_unlock(&mutex); 

  return NULL;
}

void *senteSe (void *t) {
  pthread_mutex_lock(&mutex);
  if (total == 0) { // espera a thread 5
    pthread_cond_wait(&cond_vinda, &mutex);
  }
  pthread_mutex_unlock(&mutex); 

  printf("Sente-se por favor.\n");

  pthread_mutex_lock(&mutex);
  total++; // atualiza o total de mensagens
  pthread_cond_signal(&cond_volte); // sinaliza caso a thread 1 esteja esperando
  pthread_mutex_unlock(&mutex); 

  return NULL;
}

void *copoAgua (void *t) {
  pthread_mutex_lock(&mutex);
  if (total == 0) { // espera a thread 5
    pthread_cond_wait(&cond_vinda, &mutex);
  }
  pthread_mutex_unlock(&mutex); 

  printf("Aceita um copo d'agua?\n");

  pthread_mutex_lock(&mutex);
  total++; // atualiza o total de mensagens
  pthread_cond_signal(&cond_volte); // sinaliza caso a thread 1 esteja esperando
  pthread_mutex_unlock(&mutex); 

  return NULL;
}

void *bemVindo (void *t) {
  printf("Seja bem-vindo! \n");

  pthread_mutex_lock(&mutex);
  total++; // atualiza o total de mensagens
  pthread_cond_broadcast(&cond_vinda); // sinaliza as outras threads que estejam esperando para que possam imprimir
  pthread_mutex_unlock(&mutex);

  return NULL;
}

// criar as threads
void createThreads(pthread_t *tids, int nthreads) {
  int error;

  error = pthread_create(&tids[0], NULL, aVontade, NULL);
  error = pthread_create(&tids[1], NULL, bemVindo, NULL);
  error = pthread_create(&tids[2], NULL, senteSe, NULL);
  error = pthread_create(&tids[3], NULL, copoAgua, NULL);
  error = pthread_create(&tids[4], NULL, volteSempre, NULL);

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

// inicializa as variaveis de exclusao mutua e condicao
void init(pthread_mutex_t *mutex, pthread_cond_t *cond_vinda, pthread_cond_t *cond_volte) {
  pthread_mutex_init(mutex, NULL);
  pthread_cond_init (cond_vinda, NULL);
  pthread_cond_init (cond_volte, NULL);
}

// desaloca as variaveis criadas
void destroy(pthread_mutex_t *mutex, pthread_cond_t *cond_vinda, pthread_cond_t *cond_volte) {
  pthread_mutex_destroy(mutex);
  pthread_cond_destroy(cond_vinda);
  pthread_cond_destroy(cond_volte);
}

int main(int argc, char *argv[]) {
  pthread_t tids[NTHREADS];
  
  init(&mutex, &cond_vinda, &cond_volte);
  createThreads(tids, NTHREADS);
  joinThreads(tids, NTHREADS);
  destroy(&mutex, &cond_vinda, &cond_volte);

  return 0;
}