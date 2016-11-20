#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct Queue{
  int capacity;
  int size;
  int front;
  int rear;
  int *elements;
} Queue;

Queue * createQueue(int maxElements);
void DeQueue(Queue *Q);
int front(Queue *Q);
void EnQueue(Queue *Q, int element);
bool isEmpty(Queue *Q);

#endif
