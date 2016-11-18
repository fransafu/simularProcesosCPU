#include <time.h>
#include <stdio.h>
#include <string.h>

struct MeasuredTime {
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
  int max_time;
  struct MeasuredTime times[3];
};

struct MeasuredTime create_measured_time() {
  struct MeasuredTime mt;
  mt.initial = 0;
  mt.final = 0;
  return mt;
}

struct Process create_process(int id, char *name, int max_time) {
  struct Process p;
  p.id = id;
  p.state = ReadyQueue1;
  p.max_time = max_time;
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
	 p.max_time);
}

void next_state(struct Process *p) {
  switch(p->state) {
  case ReadyQueue1:
    // El proceso entra a la cpu por primera vez
    // Veo que hora es
    p->times[0].initial = time(NULL);
    p->state = CPU1;
    break;

  case CPU1:
    time_t now = time(NULL);

    // Ya se acabo el tiempo de ejecucion del proceso
    if (difftime(now, p->times[0].initial) >= p.max_time) {
      // muevo el proceso a io
      p->state = IO;
      p->times[0].final = now;

      // Momento en que ingreso a IO
      p->times[1].initial = now;
    }
    break;

  case IO:
    time_t now = time(NULL);
    if (difftime(now, p->times[1].initial) >= p.max_time) {
      // muevo el proceso a la cola por segunda vez
      p->state = ReadyQueue2;
      p->times[1].final = now;

      // Momento en que ingreso a la cola por segunda vez
      p->times[2].initial = now;
    }
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

  for (int i = 0; i < process_count; i++) {
    int max_time;
    char name[1024];

    printf("Ingrese nombre proceso %d: ", i);
    scanf("%s", name);

    printf("Ingrese el tiempo de ejecucion: ");
    scanf("%d", &max_time);

    processes[i] = create_process(i, name, max_time);
  }

  printf("procesos\n");

  for (int i = 0; i < process_count; i++) {
    show_process(processes[i]);
  }
}
