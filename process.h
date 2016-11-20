#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>
#include "queue.h"

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

struct Process create_process(int id, char *name);
void show_process(struct Process p);
void next_state(struct Process *p, Queue *ReadyQueue, Queue *IOQueue, Queue *CPU);
void get_data(struct Process *p);
void show_progress(struct Process *p);


#endif
