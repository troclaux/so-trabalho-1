#include "queue.c"

struct Queue;

Queue *newQueue(int length);
void enqueue(Queue *queue, Process process);
Process dequeue(Queue *queue);
int isEmpty(Queue *queue);