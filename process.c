#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int seed = 12312;

typedef struct process {
    int pid;
    int ppid;
    int start;
    int end;
    int priority;
    int status;
    int programCounter;
    int service;
    int diskRequests;
    int tapeRequests;
    int printerRequests;
} Process;


void startProcess (Process* process, int* processCounter, Process* father, int actualTime) {
    srand((time(NULL)*seed));
    seed = rand()* *processCounter;

    process->pid = 100 + *processCounter;
    *processCounter += 1;
    process->priority = 1;
    process->status = 0;
    process->programCounter = 0;
    process->service = (rand() % 30) + 2;
    process->start = rand() % 20;
    if (father == NULL) process->ppid = 1;
    else process->ppid = father->pid;

    process->diskRequests = (rand() % (process->service - 2));
    if(process->diskRequests == 1) {
        process->diskRequests += 1;
    }
    process->tapeRequests = (rand() % (process->service - 2));
    if(process->tapeRequests == 1) {
        process->tapeRequests += 1;
    }
    process->printerRequests = (rand() % (process->service - 2));
    if(process->printerRequests == 1) {
        process->printerRequests += 1;
    }
    process->end = 0;

    int *checkRepeated = (int*)malloc(sizeof(int)*6);
    int index = 0;
    int random;
    int flag = 0;

    if(process->diskRequests == process->tapeRequests) {
        process->tapeRequests = 0;
    }
    if(process->diskRequests == process->printerRequests) {
        process->printerRequests = 0;
    }
    if(process->printerRequests == process->tapeRequests) {
        process->printerRequests = 0;
    }

    if(process->diskRequests + 1 == process->tapeRequests || process->tapeRequests + 1 == process->diskRequests) {
        process->tapeRequests = 0;
    }
    if(process->diskRequests + 1 == process->printerRequests || process->printerRequests + 1 == process->diskRequests) {
        process->printerRequests = 0;
    }
    if(process->tapeRequests + 1 == process->printerRequests || process->printerRequests + 1 == process->tapeRequests) {
        process->printerRequests = 0;
    }
}


void printProcesses(Process process[], int totalProcesses) {
    printf("PID\tPPID\tPriority\tStart\tEnd\tStatus\tPC\tService\t\tDisk\t\tTape\t\tPrinter\n");
    for (int i = 0; i < totalProcesses; i++) {
        printf ("%d\t", process[i].pid);
        printf ("%d\t", process[i].ppid);
        printf ("%d\t\t", process[i].priority);
        printf ("%d\t", process[i].start);
        printf ("%d\t", process[i].end);
        printf ("%d\t", process[i].status);
        printf ("%d\t", process[i].programCounter);
        printf ("%d\t\t", process[i].service);
        int j;
        if (process[i].diskRequests != 0) {
            printf("%d", process[i].diskRequests);
        }
        printf("\t\t");
        if (process[i].tapeRequests != 0) {
            printf("%d", process[i].tapeRequests);
        }
        printf("\t\t");
        if (process[i].printerRequests != 0) {
            printf("%d", process[i].printerRequests);
        }
        printf ("\n");
    }
}
int getProcessByPID (Process* processes, int PID) {
    for (int i = 0; i< MAX_PROCESS_NUMBER; i++) {
        if (processes[i].pid == PID) {
            return i;
        }
    }
}
