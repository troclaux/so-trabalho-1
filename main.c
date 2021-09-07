const int QUANTUM = 5;
const int DISK_IO_TIME = 2;
const int TAPE_IO_TIME = 3;
const int PRINTER_IO_TIME = 5;
const int MAX_PROCESS_NUMBER = 51;
#include <stdio.h>
#include <unistd.h>
#include "scheduler.h"


int processCounter = 0;
int timeUnity = 0;

void start(Scheduler* scheduler, Process* processes) {
    processIO(scheduler);
    process(scheduler, processes, timeUnity);
    timeUnity++;
}

int main () {
    Scheduler scheduler;
    startScheduler(&scheduler, MAX_PROCESS_NUMBER, DISK_IO_TIME, TAPE_IO_TIME, PRINTER_IO_TIME, QUANTUM);

	Process processes[MAX_PROCESS_NUMBER];

    int userProcesses;
    printf("\nNumber of processes:\n");
    scanf("%d", &userProcesses);
    printf("\nTotal processes = %d\n", userProcesses);


    int i;
    for (i = 0; i < userProcesses; i++) {
        startProcess(&processes[i], &processCounter, NULL, timeUnity);
    }

    printf("\e[1;1H\e[2J");
    printf("Time = %d t.u.\n", timeUnity);
    printf("QUANTUM = %d t.u.\n", QUANTUM);
    printProcesses(processes, userProcesses);
    printAllQueues(scheduler);
    getchar();
    while(1) {

        for (i = 0; i < userProcesses; i++){
            if (processes[i].start == timeUnity) {
                newProcess(&scheduler, processes[i].pid);
            }
        }

        start(&scheduler, processes);
        
        printf("\e[1;1H\e[2J");
        printf("Time = %d t.u.\n", timeUnity);
        printf("QUANTUM = %d t.u.\n", QUANTUM);
        printProcesses(processes, userProcesses);
        printAllQueues(scheduler);
        getchar();
    }

	return 0;
}




