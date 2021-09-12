#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "scheduler.c" (depois deletar include queue)
#include "queue.h"

int main() {
  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base
  Queue *queue = newQueue(3);

  for(int i = 0; i< 3 ; i++){
    Process temp = newProcess(100 + i, 1);
    enqueue(queue, temp);
  }

  for(int i = 0; i< 3 ; i++){
    printProcess(dequeue(queue));
  }

  return 0;
}