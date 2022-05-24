# Computação Concorrente | Laboratório 4

## Atividade 1

### Questão 2.
Sim. O objetivo do programa, que é "Bye" esperar para ser printado quando dois "Hello" já tiverem aparecido, foi confirmado em todas as tentativas.

### Questão 3.
O programa nem chegou a encerrar, pois não vai ter mais ninguém para desbloquear a thread B que foi bloqueada, fazendo com que ela nunca conclua sua tarefa. 

### Questão 5.
Para o programa funcionar de maneira correta, alterei *pthread_cond_signal(&x_cond);* para *pthread_cond_broadcast(&x_cond);*, fazendo com que não seja apenas um sinal de desbloqueio mandado.

## Atividade 2

### Questão 2.
Sim. As threads A só printaram "Bye" após B printar "Hello".

## Atividade 3
Com o uso de *if*, a condição só era checada antes do bloqueio. Isso quer dizer que, caso ela seja desbloqueada e o *x* se altere, é possível que essa condição não seja checada novamente e isso pode causar divergência no resultado.
Quando alteramos para *while*, nós garantimos que o loop trate esse problema, fazendo com que a condição sempre volte a ser checada e atualizada, sendo mais recomendado o seu uso nessa situação. 
>>>>>>> a50b45399faebd0bf53a62e631ceb3a880af6c4e
