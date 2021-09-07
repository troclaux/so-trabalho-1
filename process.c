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
    int indice = 0;

    int i;
    int j;
    int random;
    int flag = 0; 

    for (i = 0; i < process->diskNumber; i++) {
        random = 2 + (rand() % (process->service - 2));
        for (j = 0; j < indice; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->diskRequests[i] = random;
            checkRepeated[indice++] = random;
        }
        flag = 0;
    }

    for (i = 0; i < process->tapeNumber; i++) {
        random = 2 + (rand() % (process->service - 2));
        for (j = 0; j < indice; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->tapeRequests[i] = random;
            checkRepeated[indice++] = random;
        }
        flag = 0;
    }

    for (i = 0; i < process->printerNumber; i++) {
        random = 2 +(rand() % (process->service - 2));
        for (j = 0; j < indice; j++) {
            if (random == checkRepeated[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            process->printerRequests[i] = random;
            checkRepeated[indice++] = random;
        }
        flag = 0;
    }
}


void printProcesses(Process process[], int totalProcesses) {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw ("Numero de processos: %d\n", totalProcesses);
    attron(COLOR_PAIR(2));
    printw ("PID\tPPID\tPrioridade\tInicio\tFim\tStatus\tPC\tServico\t\tDisco\t\tFita\t\tImpressora\n");
    attron(COLOR_PAIR(3));
    int i;
    for (i = 0; i < totalProcesses; i++) {
        printw ("%d\t", process[i].pid);
        printw ("%d\t", process[i].ppid);
        printw ("%d\t\t", process[i].priority);
        printw ("%d\t", process[i].start);
        printw ("%d\t", process[i].end);
        printw ("%d\t", process[i].status);
        printw ("%d\t", process[i].programCounter);
        printw ("%d\t\t", process[i].service);
        int j;
        for (j = 0; j < process[i].diskNumber; j++) {
            printw("%d", process[i].diskRequests[j]);
            if (j != (process[i].diskNumber - 1)) {
                printw(",");
            }
        }
        printw("\t\t");
        for (j = 0; j < process[i].tapeNumber; j++) {
            printw("%d", process[i].tapeRequests[j]);
            if (j != (process[i].tapeNumber - 1)) {
                printw(",");
            }
        }
        printw("\t\t");
        for (j = 0; j < process[i].printerNumber; j++) {
            printw("%d", process[i].printerRequests[j]);
            if (j != (process[i].printerNumber - 1)) {
                printw(",");
            }
        }
        printw ("\n");
    }
}
int getProcessByPID (Process* processos, int PID) {
    int i;
    for (i = 0; i< MAX_PROCESS_NUMBER; i++) {
        if (processos[i].pid == PID) {
            return i;
        }
    }
}
