#include <stdio.h>
#include <string.h>

struct MeasuredTime {
  time_t inicial;
  time_t final;
}

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
  char nombre[100];
  int estado;
  struct MeasuredTime[3];
};

struct Process create_process(int id, char *name) {
  struct Process p;
  p.id = id;
  p.estado = ReadyQueue1;
  p.tiempo[0] = 0;
  p.tiempo[1] = 0;
  p.tiempo[2] = 0;
  strncpy(p.nombre, name, strlen(name));

  return p;
}

int main(void)
{
  struct Process p = create_process(1, "foo");

  printf("pid: %d\nnombre: %s\nestado: %d\n", p.id, p.nombre, p.estado);
  return (0);
}
