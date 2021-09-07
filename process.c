#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int seed = 5;

typedef struct process {
    int pid;
    int ppid;
    int start;
    int end;
    int priority;
    int status;
    int programCounter;
    int service;
    int *diskRequests;
    int diskNumber;
    int *tapeRequests;
    int tapeNumber;
    int *printerRequests;
    int printerNumber;
} Process;


void startProcess (Process* process, int* processCounter, Process* father, int actualTime) {
    srand((unsigned)((time(NULL)*seed)));
    seed = rand()* *processCounter;

    process->pid = 100 + *processCounter;
    *processCounter += 1;
    process->priority = 1;
    process->status = 0;
    process->programCounter = 1;
    process->service = (rand() % 40) + 3;
    process->start = rand() % 51;
    if (father == NULL) process->ppid = 1;
    else process->ppid = father->pid;

    process->diskNumber = rand() % 3;
    process->diskRequests = (int*)malloc(sizeof(int) * process->diskNumber);
    process->tapeNumber = rand() % 3;
    process->tapeRequests = (int*)malloc(sizeof(int) * process->tapeNumber);
    process->printerNumber = rand() % 3;
    process->printerRequests = (int*)malloc(sizeof(int) * process->printerNumber);
    process->end = 0;

    int *checkRepeated = (int*)malloc(sizeof(int)*6);
    int index = 0;
    int random;
    int flag = 0; 

    for (int i = 0; i < process->diskNumber; i++) {
        random = 2 + (rand() % (process->service - 2));
        for (int j = 0; j < index; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->diskRequests[i] = random;
            checkRepeated[index++] = random;
        }
        flag = 0;
    }

    for (int i = 0; i < process->tapeNumber; i++) {
        random = 2 + (rand() % (process->service - 2));
        for (int j = 0; j < index; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->tapeRequests[i] = random;
            checkRepeated[index++] = random;
        }
        flag = 0;
    }

    for (int i = 0; i < process->printerNumber; i++) {
        random = 2 +(rand() % (process->service - 2));
        for (int j = 0; j < index; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->printerRequests[i] = random;
            checkRepeated[index++] = random;
        }
        flag = 0;
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
        for (j = 0; j < process[i].diskNumber; j++) {
            printf("%d", process[i].diskRequests[j]);
            if (j != (process[i].diskNumber - 1)) {
                printf(",");
            }
        }
        printf("\t\t");
        for (j = 0; j < process[i].tapeNumber; j++) {
            printf("%d", process[i].tapeRequests[j]);
            if (j != (process[i].tapeNumber - 1)) {
                printf(",");
            }
        }
        printf("\t\t");
        for (j = 0; j < process[i].printerNumber; j++) {
            printf("%d", process[i].printerRequests[j]);
            if (j != (process[i].printerNumber - 1)) {
                printf(",");
            }
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
