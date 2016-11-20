#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>


// --- HERE

typedef struct Queue{
  int capacity;
  int size;
  int front;
  int rear;
  int *elements;
}Queue;

Queue * createQueue(int maxElements){
  Queue *Q;
  Q = (Queue *)malloc(sizeof(Queue));
  Q->elements = (int *)malloc(sizeof(int)*maxElements);
  Q->size = 0;
  Q->capacity = maxElements;
  Q->front = 0;
  Q->rear = -1;

  return Q;
}

void DeQueue(Queue *Q){
  if (Q->size == 0){
    printf("Ready Queue is empty\n");
    return;
  } else {
    Q->size--;
    Q->front++;
    if (Q->front == Q->capacity){
      Q->front = 0;
    }
  }
  return;
}

int front(Queue *Q){
  if(Q->size == 0){
    printf("Ready Queue is empty\n");
    exit(0);
  }
  return Q->elements[Q->front];
}

bool isEmpty(Queue *Q){
  if(Q->size == 0){
    return true;
  } else {
    return false;
  }
}


void EnQueue(Queue *Q, int element){
  if (Q->size == Q->capacity){
    printf("Ready Queue is full\n");
  } else {
    Q->size++;
    Q->rear = Q->rear + 1;

    if(Q->rear == Q->capacity){
      Q->rear = 0;
    }

    Q->elements[Q->rear] = element;
  }
  return;
}

// --- HERE

struct MeasuredTime {
  int max_time;
  time_t initial;
  time_t final;
};

enum CpuState {
  Queue1,
  CPU1,
  IO,
  Queue2,
  CPU2,
  Done,
};

struct Process {
  int id;
  char name[100];
  int state;
  struct MeasuredTime times[3];
};

struct MeasuredTime create_measured_time() {
  struct MeasuredTime mt;
  mt.max_time = 0;
  mt.initial = 0;
  mt.final = 0;
  return mt;
}

struct Process create_process(int id, char *name) {
  struct Process p;
  p.id = id;
  p.state = Queue1;
  p.times[0] = create_measured_time();
  p.times[1] = create_measured_time();
  p.times[2] = create_measured_time();
  strncpy(p.name, name, strlen(name));

  return p;
}

void show_process(FILE *log, struct Process p) {
  fprintf(log, "pid: %d\nnombre: %s\nestado: %d\nTiempo ejecucion: %d\n",
	 p.id,
	 p.name,
	 p.state,
	 p.times[0].max_time + p.times[1].max_time + p.times[2].max_time);

  for (int i = 0; i < 3; i++) {
    fprintf(log, "tiempo ejecucion %d: %d\n", i + 1, p.times[i].max_time);
  }
}

void next_state(struct Process *p, Queue *ReadyQueue, Queue *IOQueue, Queue *CPU, FILE *log) {
  time_t now = time(NULL);

  switch(p->state) {
  case Queue1:
    if (isEmpty(CPU) && p->id == front(ReadyQueue)){
      // El proceso entra a la cpu por primera vez
      EnQueue(CPU, p->id);
      p->times[0].initial = now;
      p->state = CPU1;
      DeQueue(ReadyQueue); // Sacamos el proceso de la lista de espera
    }
    break;

  case CPU1:
    // Ya se acabo el tiempo de ejecucion del proceso
    if (difftime(now, p->times[0].initial) >= p->times[0].max_time) {
      // muevo el proceso a io
      EnQueue(IOQueue, p->id);
      p->state = IO;
      p->times[0].final = now;

      // Momento en que ingreso a IO
      p->times[1].initial = now;
      DeQueue(CPU);
    }
    break;

  case IO:
    if (difftime(now, p->times[1].initial) >= p->times[1].max_time) {
      // muevo el proceso a la cola por segunda vez
      EnQueue(ReadyQueue,p->id); // Ingresamos proceso a cola
      p->state = Queue2;
      p->times[1].final = now;
      DeQueue(IOQueue);
    }
    break;

  case Queue2:
    if (isEmpty(CPU) && p->id == front(ReadyQueue)){
      // Tiempo en que entro a la cpu por segunda vez
      EnQueue(CPU, p->id);
      p->times[2].initial = now;
      p->state = CPU2;
      DeQueue(ReadyQueue);
    }
    break;

  case CPU2:
    if (difftime(now, p->times[2].initial) >= p->times[2].max_time) {
      p->times[2].final = now;
      p->state = Done;
      DeQueue(CPU);
    }
    break;

  case Done:
    fprintf(log, "Proceso %s terminado.\n", p->name);
    break;

  default:
    fprintf(log, "Estado invalido: %d", p->state);
    break;
  }
}

void get_data(struct Process *p){
  printf ("Ingrese tiempo1 CPU1:\n");scanf ("%d",&(p->times[0].max_time));
  printf ("Ingrese tiempo2 I/O:\n");scanf ("%d",&(p->times[1].max_time));
  printf ("Ingrese tiempo3 CPU2:\n");scanf ("%d",&(p->times[2].max_time));
}

void show_queue(Queue *Q){
  for ( int i = 0; i < Q->size; i++ ) {
    printf( "%d ", Q->elements[ i ] );
  }

}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

struct thread_data {
  struct Process *processes;
  int process_count;

  Queue *ReadyQueue;
  Queue *IOQueue;
  Queue *CPU;

  FILE *log;
};

void *show_info(void *data) {
  struct thread_data *tdata = (struct thread_data *)data;

  for(;;) {
    sleep(1);

    for (int i = 0; i < tdata->process_count; i++) {
      fprintf(tdata->log, "------ DENTRO DEL HILO CREADO --------------\n");

      fprintf(tdata->log, "TIME ACTUAL: %lld\n", (long long) time(NULL));

      next_state(&(tdata->processes[i]),
		 tdata->ReadyQueue,
		 tdata->IOQueue,
		 tdata->CPU,
		 tdata->log);

      show_process(tdata->log, tdata->processes[i]);

      fprintf(tdata->log, "--------------------\n");
    }

    fprintf(tdata->log, "\n");
  }
}

FILE *create_terminal() {
  char *name = tempnam(NULL, NULL);
  char cmd[256];

  mkfifo(name, 0777);
  if(fork() == 0)
    {
      sprintf(cmd, "xterm -e cat %s", name);
      system(cmd);
      exit(0);
    }
  return fopen(name, "w");
}

int main(void)
{
  struct Process processes[4096];

  Queue *ReadyQueue = createQueue(4096);
  Queue *IOQueue = createQueue(4096);
  Queue *CPU = createQueue(1);

  pthread_t show_info_thread;
  struct thread_data tdata = {
    .processes = processes,
    // Por defecto hay 0 procesos
    .process_count = 0,
    .ReadyQueue = ReadyQueue,
    .IOQueue = IOQueue,
    .CPU = CPU,
    .log = create_terminal()
  };
  pthread_create(&show_info_thread, NULL, show_info, (void *)&tdata);


  for (;;) {
    char name[1024];

    printf("Ingrese nombre proceso %d: ", tdata.process_count);
    scanf("%s", name);

    processes[tdata.process_count] = create_process(tdata.process_count, name);

    get_data(&(processes[tdata.process_count]));
    EnQueue(ReadyQueue,processes[tdata.process_count].id);
    tdata.process_count++;
  }

  // nunca se deberia llegar aqui.

  return 0;
}
