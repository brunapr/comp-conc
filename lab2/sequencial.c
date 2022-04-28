/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include "timer.h"
#define RAND_MAX 5

float *firstMatrix;
float *secondMatrix;
float *outputMatrix;
int dim; //dimensao das matrizes de entrada

srand(time(NULL));

int main(int argc, char* argv[]) {
   double start, finish, elapsed;

   if(argc < 2) {
       printf("Comando incorreto. Digite: %s <dimensao das matrizes>\n", argv[0]);
       return 1;
   };

   GET_TIME(start);

   dim = atoi(argv[1]);

   firstMatrix = (float *) malloc(sizeof(float) * dim * dim);
   if (firstMatrix == NULL) { printf("ERRO -- malloc\n"); return 2; }
   
   secondMatrix = (float *) malloc(sizeof(float) * dim * dim);
   if (firstMatrix == NULL) { printf("ERRO -- malloc\n"); return 2; }
   
   outputMatrix = (float *) malloc(sizeof(float) * dim * dim);
   if (firstMatrix == NULL) { printf("ERRO -- malloc\n"); return 2; }

   // inicializacao
   for (int i = 0; i < dim; i++) {
       for (int j = 0; j < dim; j ++) {
           firstMatrix[i*dim + j] = (rand() %10);
           secondMatrix[i*dim + j] = (rand() %10);
           outputMatrix[i*dim + j] = 0;
       }
   }

   GET_TIME(finish);
   elapsed = finish - start;
   printf("Tempo inicializacao: %lf\n", elapsed);

   // multiplicando

   GET_TIME(start); 

   //linha
   for (int i = 0; i < dim; i++) {
       //coluna
       for (int j = 0; j < dim; j++) {
           for (int k = 0; k < dim; k++) {
               outputMatrix[i*dim + j] += firstMatrix[i*dim +k] * secondMatrix[k*dim +j];
           }
       }
   }

   GET_TIME(finish);
   elapsed = finish - start;
   printf("Tempo de multiplicacao: %lf\n", elapsed); 

   //resultado

   GET_TIME(start);

//    puts("matriz resultante:");
//    for (int i = 0; i < dim; i++) {
//        for (int j = 0; j < dim; j ++) {
//            printf("%.1lf ", outputMatrix[i*dim +j]);
//        }
//        printf("\n");
//    }
//    puts("");

   GET_TIME(finish);
   elapsed = finish - start;
   printf("Tempo de exibicao do resultado: %lf\n", elapsed);

   // liberar memoria
   free(firstMatrix);
   free(secondMatrix);
   free(outputMatrix);

   return 0;
}