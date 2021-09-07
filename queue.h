#include "queue.c"

struct queue;

void newQueue(queue* fila, int max_processos);
bool isFull(queue fila);
bool isEmpty(queue fila);
void joinQueue(queue* fila, int processoPID);
int exitQueue(queue* fila);