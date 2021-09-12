#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "scheduler.c" (depois deletar include queue)
#include "queue.h"

int main() {
  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base
  int pid;
  Process process;
  Process *processes;
  processes = (Process *)calloc(3, sizeof(Process));
  Queue *queue = newQueue(3);

  for(int i = 0; i< 3 ; i++){
    Process temp = newProcess(100 + i, 1);
    processes[i] = temp;
    enqueue(queue, temp.pid);
  }

  for(int i = 0; i< 3 ; i++){
    pid = dequeue(queue);
    process = getProcessByPID(pid, processes, 3);
    printProcess(process);
    killProcess(process);
  }
  return 0;
}