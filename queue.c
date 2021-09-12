#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"

typedef struct {
  Process *array;
  Process front;
  Process rear;
  int size; //numero de elementos na fila
  int capacity; //capacidade maxima estatica da fila
} Queue;

static const Queue EmptyQueue;

Queue *newQueue(int length) {
  Queue *queue;
  queue = (Queue *)calloc(1, sizeof(Queue));
  queue->array = (Process *)calloc(length, sizeof(Process));
  queue->front = EmptyProcess;
  queue->rear = EmptyProcess;
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

void enqueue(Queue *queue, Process process) {
  if(isEmpty(queue)){
    queue->front = process;
  }

  queue->array[queue->size] = process;
  queue->rear = queue->array[queue->size];
  queue->size++;
}

Process dequeue(Queue *queue) {
  if(isEmpty(queue)){
    return EmptyProcess;
  }

  Process process = queue->front;

  queue->front = queue->array[1];

  for(int i = 0; i < queue->size; i++){
    if (i == queue->capacity -1) {
      queue->array[i] = EmptyProcess;
    } else {
      queue->array[i] = queue->array[i+1];
    }
  }
  queue->size--;
  
  return process;
}