const int QUANTUM = 5;
const int DISK_IO_TIME = 2;
const int TAPE_IO_TIME = 3;
const int PRINTER_IO_TIME = 5;
const int MAX_PROCESS_NUMBER = 51;
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
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
    initscr();
    start_color();
    printw("\nNumber of processes:\n");
    refresh();
    scanw("%d", &userProcesses);
    printw("\nTotal processes = %d\n", userProcesses);
    refresh();


    int i;
    for (i = 0; i < userProcesses; i++) {
        startProcess(&processes[i], &processCounter, NULL, timeUnity);
    }

    while(1) {

        for (i = 0; i < userProcesses; i++){
            if (processes[i].start == timeUnity) {
                newProcess(&scheduler, processes[i].pid);
            }
        }

        start(&scheduler, processes);
        
        clear();
        init_pair(50, COLOR_WHITE, COLOR_BLACK);
        attron(COLOR_PAIR(50));
        printw("Time = %d t.u.\n", timeUnity);
        refresh();
        printProcesses(processes, userProcesses);
        refresh();
        attron(COLOR_PAIR(2));
        printAllQueues(scheduler);

        refresh();
        getch();
    }

	return 0;
}




