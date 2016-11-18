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

int main(void)
{
  struct Process p = create_process(1, "foo");

  printf("pid: %d\nnombre: %s\nestado: %d\n", p.id, p.name, p.state);
  return (0);
}
