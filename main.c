#include <stdio.h>

enum CpuState {
  ReadyQueue1,
  CPU1,
  IO,
  ReadyQueue2,
  CPU2,
  Done,
};

struct Process {
    int process_id;
    char nombre[100];
    int estado;
    int tiempo[3];
};

int main(void)
{
    printf("First commit!\n");
    return (0);
}
