#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
  int pid;
  int ppid;
  int start;
  int end;
  int priority;
  int status;
  int processedTU;
  int service;
  int *printerRequests;
  int *tapeRequests;
  int *diskRequests;
} Process;

static const Process EmptyProcess;

bool checkIntInArray(int number, int *array) {
  int i, length;
  length = array[0];
  for (i = 1; i < length; i++) {
    if (array[i] == number) {
      return true;
    }
  }
  return false;
}

int *generateIOTime(int serviceTime, int *IOArray1, int *IOArray2) {
  if (serviceTime == 0) {
    return NULL;
  }
  bool exists;
  int i, temp;
  int *IOTimeArray;
  int quantity = rand() % 4;
  IOTimeArray = (int *)calloc(quantity + 1, sizeof(int));
  IOTimeArray[0] = quantity;
  for (i = 1; i < quantity + 1; i++) {
    temp = rand() % serviceTime;
    if (checkIntInArray(temp, IOTimeArray) || 
        checkIntInArray(temp, IOArray1) || 
        checkIntInArray(temp, IOArray2)) {
      exists = true;
      while (exists) {
        temp = rand() % serviceTime;
        if (checkIntInArray(temp, IOTimeArray) || 
            checkIntInArray(temp, IOArray1) || 
            checkIntInArray(temp, IOArray2)) {
          continue;
        }
        exists = false;
      }
    }
    IOTimeArray[i] = temp;
  }
  return IOTimeArray;
}

Process newProcess(int pid, int ppid) {
  Process process;
  process.pid = pid;
  process.ppid = ppid;
  process.start = rand() % 30;
  process.end = -1;
  process.processedTU = 0;

  process.priority = 1;
  process.status = 0;
  process.service = rand() % 30;
  process.diskRequests = (int *)calloc(4, sizeof(int));
  process.tapeRequests = (int *)calloc(4, sizeof(int));
  process.printerRequests = (int *)calloc(4, sizeof(int));
  process.diskRequests = generateIOTime(process.service, process.tapeRequests, process.printerRequests);
  process.tapeRequests = generateIOTime(process.service, process.diskRequests, process.printerRequests);
  process.printerRequests = generateIOTime(process.service, process.diskRequests, process.tapeRequests);

  return process;
}

void printProcess(Process process){
  printf("PID | PPID | status | start | end | priority | processedTU | service | printer | tape | disk \n");
  printf("%d | ", process.pid);
  printf("%d | ", process.ppid);
  printf("%d | ", process.status);
  printf("%d | ", process.start);
  printf("%d | ", process.end);
  printf("%d | ", process.priority);
  printf("%d | ", process.processedTU);
  printf("%d | ", process.service);
  printf("%p | ", (void *) process.printerRequests);
  printf("%p | ", (void *) process.tapeRequests);
  printf("%p \n", (void *) process.diskRequests);
}

void killProcess(Process process) {
  free(process.diskRequests);
  free(process.tapeRequests);
  free(process.printerRequests);
}