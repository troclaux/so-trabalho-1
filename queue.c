#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"

typedef struct {
  int *array;
  int front;
  int rear;
  int size; //numero de elementos na fila
  int capacity; //capacidade maxima estatica da fila
} Queue;

static const Queue EmptyQueue;

Queue *newQueue(int length) {
  Queue *queue;
  queue = (Queue *)calloc(1, sizeof(Queue));
  queue->array = (int *)calloc(length, sizeof(int));
  queue->front = 0;
  queue->rear = 0;
  queue->size = 0;
  queue->capacity = length;
  return queue;
}

bool isEmpty(Queue *queue){
  if(queue->size == 0){
    return true;
  }

  return false;
}

void enqueue(Queue *queue, int pid) {
  if(isEmpty(queue)){
    queue->front = pid;
  }

  queue->array[queue->size] = pid;
  queue->rear = queue->array[queue->size];
  queue->size++;
}

int dequeue(Queue *queue) {
  if(isEmpty(queue)){
    return 0;
  }

  int pid = queue->front;

  queue->front = queue->array[1];

  for(int i = 0; i < queue->size; i++){
    if (i == queue->capacity -1) {
      queue->array[i] = 0;
    } else {
      queue->array[i] = queue->array[i+1];
    }
  }
  queue->size--;
  
  return pid;
}

void printQueue(Queue *queue) {
  int i;
  if (!isEmpty(queue)) {
    for (i = 0 ; i < queue->size ; i++) {
      printf("%d", queue->array[i]);
      if (i != queue->size - 1) {
        printf(", ");
      }
    }
  }
}