#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
  struct MeasuredTime times[3];
};

struct MeasuredTime create_measured_time() {
  struct MeasuredTime mt;
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
  printf("pid: %d\nnombre: %s\nestado: %d\n", p.id, p.name, p.state);
}

// -- HERE
/*
void get_data(struct Process p){
  printf ("Ingrese tiempo1 CPU1:\n");scanf  ("%d",p.times[0]);
  printf ("Ingrese tiempo2 I/O:\n");scanf  ("%d",p.times[1]);
  printf ("Ingrese tiempo3 CPU2:\n");scanf  ("%d",p.times[2]);

  return p;
}*/

void get_data(struct Process *p){
  printf ("Ingrese tiempo1 CPU1:\n");scanf ("%d",p->times[0]);
  printf ("Ingrese tiempo2 I/O:\n");scanf ("%d",p->times[1]);
  printf ("Ingrese tiempo3 CPU2:\n");scanf ("%d",p->times[2]);
}

bool empty_cpu(struct Process p){
  if (p.state == CPU1 || p.state == CPU2){
    return false;
  }
}

void load_process_cpu(){
  return 0;
}

void clock_cpu(){
  return 0;
}

void clock_IO(){
  return 0;
}

// -- HERE

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
    get_data(&processes[i]);
    show_process(processes[i]);
  }

  for (int i = 0; 9 < process_count; i++){
    if (empty_cpu(processes[i])){
      printf("Hace falta un proceso en la CPU\n");
    }
  }
}
