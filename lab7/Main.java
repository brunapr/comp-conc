import java.util.Random;

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Counter {
  //guarda o total de numeros pares no vetor 
  //recurso compartilhado entre as threads
  private int total = 0; 

  public synchronized void addTotal() {
    this.total++;
  }

  public synchronized int getTotal() {
    return this.total;
  }
}

//--PASSO 1: criar uma classe que implementa a interface Runnable 
class Pares implements Runnable {
  private int tid; //identificador da thread
  private int nthreads; //total de threads
  private int[] vetor; //vetor de inteiros
  private Counter counter; //objeto compartilhado com outras threads

  //construtor
  public Pares(int tid, int nthreads, int[] vetor, Counter counter) {
    this.tid = tid;
    this.nthreads = nthreads;
    this.vetor = vetor;
    this.counter = counter;
  }

  //metodo executado pela thread
  //percorre os elementos de forma intercalada
  @Override
  public void run() {
    for (int i = tid; i < vetor.length; i += nthreads) {
      if (vetor[i] % 2 == 0) this.counter.addTotal();
    }
  }
}

//classe do metodo main
class Main {
  static final int SIZE = 10000000;
  static final int N = 4;

  //metodo para contar o total de numeros pares de forma sequencial
  static int contaSeq(int[] vetor) {
    int totalSeq = 0;
    for (int i = 0; i < vetor.length; i++) {
      if (vetor[i] % 2 == 0)
        totalSeq++;
    }
    return totalSeq;
  }

  //metodo para conferir se a contagem sequencial e concorrente de numeros pares eh a mesma
  static void verifica(Counter counter, int[] vetor) {
    int totalSeq = contaSeq(vetor);
    int totalConc = counter.getTotal();

    System.out.println("Concorrente encontrou: " + totalConc + " pares");
    System.out.println("Sequencial encontrou: " + totalSeq + " pares");

    if (totalSeq == totalConc) {
      System.out.println("O valor encontrado pela funcao concorrente esta correto!");
    } else {
      System.out.println("O valor encontrado pela funcao concorrente esta errado.");
    }
  }

  public static void main(String[] args) {
    Thread[] threads = new Thread[N]; //reserva espaço para um vetor de threads
    int[] vetor = new int[SIZE]; //reserva espaco para o vetor de inteiros

    //cria uma instancia do recurso compartilhado entre as threads
    Counter counter = new Counter();

    //inicializa o vetor de inteiros
    Random rand = new Random();
    for (int i = 0; i < SIZE; i++) {
      vetor[i] = rand.nextInt();
    }

    //--PASSO 2: transformar o objeto Runnable em Thread
    for (int i=0; i<threads.length; i++) {
      threads[i] = new Thread(new Pares(i, N, vetor, counter));
    }

    //--PASSO 3: iniciar a thread
    for (int i=0; i<threads.length; i++) {
      threads[i].start();
    }

    //--PASSO 4: esperar pelo termino das threads
    for (int i=0; i<threads.length; i++) {
      try { threads[i].join(); } 
      catch (InterruptedException e) { return; }
    } 

    //verifica a corretude do resultado
    verifica(counter, vetor);
  }
}