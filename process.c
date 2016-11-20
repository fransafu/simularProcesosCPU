#include <stdio.h>
#include <string.h>

#include "process.h"

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
    fprintf(stderr, "Estado invalido: %d\n", p->state);
    break;
  }

}
