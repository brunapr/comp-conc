# Computação Concorrente | Laboratório 7

## Atividade 1
  ### Questão 3.
  Sim. Há mudança. Dá para ver pelos prints que nem sempre todas as threads rodam antes do programa printar que "Terminou".
  ### Questão 5.
  As threads são criadas primeiro, logo após elas executam em momentos diferentes e o último print é sempre "Terminou", diferente de antes, quando essas linhas estavam comentadas.

## Atividade 3
  ### Questão 1.
  A seção crítica do código é exatamente o que está sendo compartilhado, que no caso é o incremento que acontece na Classe S.
  A saída esperada final é 200000. Temos duas threads, então ambas irão incrementar a variável "s" 100000 vezes, fazendo com que a saída seja essa.
  ### Questão 2.
  Não. Os valores mudam toda vez que o programa é chamado. Isso se dá porque há condição de corrida e as threads acabam sobrescrevendo os resultados uma em cima da outra.  

## Atividade 4
  ### Questão 3.
  Sim. Não há condição de corrida e as threads dividem a tarefa de forma satisfatória, fazendo com que elas sejam criadas, façam o incremento e por fim, o programa espera as duas terminarem para lançar o resultado 200000, que é o esperado.