#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Queue * createQueue(int maxElements){
  Queue *Q;
  Q = (Queue *)malloc(sizeof(Queue));
  Q->elements = (int *)malloc(sizeof(int)*maxElements);
  Q->size = 0;
  Q->capacity = maxElements;
  Q->front = 0;
  Q->rear = -1;

  return Q;
}

void DeQueue(Queue *Q){
  if (Q->size == 0){
    printf("Ready Queue is empty\n");
    return;
  } else {
    Q->size--;
    Q->front++;
    if (Q->front == Q->capacity){
      Q->front = 0;
    }
  }
  return;
}

int front(Queue *Q){
  if(Q->size == 0){
    printf("Ready Queue is empty\n");
    exit(0);
  }
  return Q->elements[Q->front];
}

bool isEmpty(Queue *Q){
  if(Q->size == 0){
    return true;
  } else {
    return false;
  }
}


void EnQueue(Queue *Q, int element){
  if (Q->size == Q->capacity){
    printf("Ready Queue is full\n");
  } else {
    Q->size++;
    Q->rear = Q->rear + 1;

    if(Q->rear == Q->capacity){
      Q->rear = 0;
    }

    Q->elements[Q->rear] = element;
  }
  return;
}
