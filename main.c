#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "queue.h"

struct MeasuredTime {
  int max_time;
  time_t initial;
  time_t final;
};

enum CpuState {
  ReadyQueue1,
  CPU1,
  IO,
  ReadyQueue2,
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
  p.state = ReadyQueue1;
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

void next_state(struct Process *p) {
  time_t now = time(NULL);

  switch(p->state) {
  case ReadyQueue1:
    // El proceso entra a la cpu por primera vez
    p->times[0].initial = now;
    p->state = CPU1;
    break;

  case CPU1:
    // Ya se acabo el tiempo de ejecucion del proceso
    if (difftime(now, p->times[0].initial) >= p->times[0].max_time) {
      // muevo el proceso a io
      p->state = IO;
      p->times[0].final = now;

      // Momento en que ingreso a IO
      p->times[1].initial = now;
    }
    break;

  case IO:
    if (difftime(now, p->times[1].initial) >= p->times[1].max_time) {
      // muevo el proceso a la cola por segunda vez
      p->state = ReadyQueue2;
      p->times[1].final = now;
    }
    break;

  case ReadyQueue2:
    // Tiempo en que entro a la cpu por segunda vez
    p->times[2].initial = now;
    p->state = CPU2;
    break;

  case CPU2:
    if (difftime(now, p->times[2].initial) >= p->times[2].max_time) {
      p->times[2].final = now;
      p->state = Done;
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

int main(void)
{
  struct Process processes[4096];
  int process_count;

  printf("Indique la cantidad de procesos: ");
  scanf("%d", &process_count);

  printf("%d\n", process_count);

  for (int i = 0; i < process_count; i++) {
    char name[1024];

    printf("Ingrese nombre proceso %d: ", i);
    scanf("%s", name);

    processes[i] = create_process(i, name);
  }

  printf("procesos\n");

  for (int i = 0; i < process_count; i++) {
    get_data(&(processes[i]));
    show_process(processes[i]);
  }
}
