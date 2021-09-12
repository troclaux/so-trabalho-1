#include "queue.c"

struct Queue;

Queue *newQueue(int length);
bool isEmpty(Queue *queue);
void enqueue(Queue *queue, Process process);
Process dequeue(Queue *queue);