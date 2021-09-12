#include "queue.c"

struct Queue;

Queue *newQueue(int length);
bool isEmpty(Queue *queue);
void enqueue(Queue *queue, int pid);
int dequeue(Queue *queue);