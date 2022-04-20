#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define SIZE 10000
#define NTHREADS 2

// Foi criado um array para as threads preencherem e outro para
// ser preenchido pelo resultado esperado da maquina
int array[SIZE];
int expectedOutput[SIZE];

// Preenche com valores de 1 a SIZE
// Esses valores que serao elevados pelas threads
void sizeArray() {
  for (int i = 0; i < SIZE; i++) 
    array[i] = i + 1;
}

// Resultado esperado, numeros elevados preenchidos pela maquina
void fillexpectedOutput() {
  for (int i = 1; i <= SIZE; i++)
    expectedOutput[i - 1] = i * i;
}

// Funcao que as threads vao usar para elevar dos elementos
// A thread vai pegar elementos pares ou impares dependendo de seu indice
void* squareElements(void *arg) {
  int initialIndex = *((int *) arg) - 1;

  for (int i=initialIndex; i < SIZE; i+=2)
    array[i] *= array[i];

  pthread_exit(NULL);
}

// Compara o array esperado com o obtido
void analyzeOutput() {

  fillexpectedOutput();

  for (int i = 0; i < SIZE; i++) {
    if (array[i] != expectedOutput[i]) {
      printf("Erro na posicao %d.\nValor esperado: %d\nEncontrado: %d\n", i, expectedOutput[i], array[i]);
      exit(1);
    }
  }
  printf("Saida correta. Todas as posicoes conferem. \n");
}

// Imprime o array com os resultados das threads
void printArray() {
  for (int i = 0; i < SIZE; i++)
    printf("%d \n", array[i]);
}

// Principal 
int main(void) {
  int i;
  pthread_t tid[NTHREADS];
  int ident[NTHREADS]; 

  sizeArray();

  for(i=0; i < NTHREADS; i++) {
    ident[i] = i + 1;
    if (pthread_create(&tid[i], NULL, squareElements, (void *)&ident[i]))
      printf("Erro ao criar a thread %d! \n", ident[i]);
  }
	
  for(i=0; i < NTHREADS; i++) {
    if (pthread_join(tid[i], NULL))
      printf("Erro com o join \n");
  }

  // printArray();
	
  analyzeOutput();

  return 0;
}