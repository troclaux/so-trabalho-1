#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct queue {
    int front;
    int rear;
    int size;
    int inUse;
    int* pids;
} queue;

void newQueue(queue* queue, int max_process) {
    queue->front = -1;
    queue->rear = -1;
    queue->size = max_process;
    int i;
    queue->pids = (int*)malloc(queue->size*sizeof(int));
    queue->inUse = 0;
}

bool isFull(queue queue) {
    if ((queue.front == queue.rear + 1) || (queue.front == 0 && queue.rear == queue.size - 1)) return true;
    return false;
}

bool isEmpty(queue queue) {
    if (queue.front == -1) return true;
    return false;
}

void joinQueue(queue* queue, int processPID) {
    if (!isFull(*queue)) {
        if (queue->front == -1) queue->front = 0;
        queue->rear = (queue->rear + 1) % queue->size;
        queue->pids[queue->rear] = processPID;
    }
}

int exitQueue(queue* queue) {
    if (!isEmpty(*queue)) {
        int retorno = queue->pids[queue->front];
        if (queue->front == queue->rear) {
            queue->front = -1;
            queue->rear = -1;
        }
        else {
            queue->front = (queue->front + 1) % queue->size;
        }
        return retorno;
    }
}

void printQueue(queue queue) {
    int i = 0;
    for (i = queue.front; i != (queue.rear + 1); i = ((i + 1) % (queue.size + 1))) {
        printw("%d ", queue.pids[i]);
        refresh();
    }
    printw("\n");
}
