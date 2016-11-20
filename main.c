#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "process.h"

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
    printf("\n");
  }

  return 0;
}
