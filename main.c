#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// --- HERE

/*
  EnQueue(Q,1);
  EnQueue(Q,2);
  EnQueue(Q,3);
  EnQueue(Q,4);
  printf("Front element is %d\n",front(Q));
  EnQueue(Q,5);
  DeQueue(Q);
  EnQueue(Q,6);
  printf("Front element is %d\n",front(Q));*/

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

void show_process(struct Process p) {
  printf("pid: %d\nnombre: %s\nestado: %d\nTiempo ejecucion: %d\n",
	 p.id,
	 p.name,
	 p.state,
	 p.times[0].max_time + p.times[1].max_time + p.times[2].max_time);

  for (int i = 0; i < 3; i++) {
    printf("tiempo ejecucion %d: %d\n", i + 1, p.times[i].max_time);
  }
}



void next_state(struct Process *p, Queue *ReadyQueue, Queue *IOQueue, Queue *CPU) {
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
      printf("sacando %s de la cpu\n", p->name);
      DeQueue(CPU);
    }
    break;

  default:
    fprintf(stderr, "Estado invalido: %d", p->state);
    break;
  }
}

void get_data(struct Process *p){
  printf ("Ingrese tiempo1 CPU1:\n");
  scanf ("%d",&(p->times[0].max_time));

  printf ("Ingrese tiempo2 I/O:\n");
  scanf ("%d",&(p->times[1].max_time));

  printf ("Ingrese tiempo3 CPU2:\n");
  scanf ("%d",&(p->times[2].max_time));
}

void show_progress(struct Process *p) {
  int elapsed_time;

  switch(p->state) {
  case Queue1:
    break;
  case Queue2:
    printf("%d : %s > En espera\n", p->id, p->name);
    break;

  case CPU1:
    elapsed_time = difftime(time(NULL), p->times[0].initial);
    printf("%d : %s > En CPU (1) > Tiempo Ejecucion: %d\n",
	   p->id,
	   p->name,
	   elapsed_time);
    break;

  case CPU2:
    elapsed_time = difftime(time(NULL), p->times[2].initial);
    printf("%d : %s > En CPU (2) > Tiempo Ejecucion: %d\n",
	   p->id,
	   p->name,
	   elapsed_time);
    break;

  case IO:
    elapsed_time = difftime(time(NULL), p->times[1].initial);
    printf("%d : %s > En IO > Tiempo Ejecucion: %d\n",
	   p->id,
	   p->name,
	   elapsed_time);
    break;

  case Done:
    printf("%d : %s > Terminado\n", p->id, p->name);
    break;

  default:
    fprintf(stderr, "Estado invalido: %d", p->state);
    break;
  }

}

int main(void)
{
  struct Process processes[4096];
  int process_count;

  printf("Indique la cantidad de procesos: ");
  scanf("%d", &process_count);

  printf("%d\n", process_count);

  Queue *ReadyQueue = createQueue(process_count + 2);
  Queue *IOQueue = createQueue(process_count + 2);
  Queue *CPU = createQueue(1);

  for (int i = 0; i < process_count; i++) {
    char name[1024];

    printf("Ingrese nombre proceso %d: ", i);
    scanf("%s", name);

    processes[i] = create_process(i, name);
    memset(name, '\0', 1024);
  }

  for (int i = 0; i < process_count; i++) {
    get_data(&(processes[i]));
    EnQueue(ReadyQueue,processes[i].id);
    show_process(processes[i]);
  }

  for (;;) {
    int done = 1;

    for (int i = 0; i < process_count; i++) {
      next_state(&(processes[i]), ReadyQueue, IOQueue, CPU);
      show_progress(&(processes[i]));
    }
  }


  return 0;
}
