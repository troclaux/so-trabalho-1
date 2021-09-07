#include "queue.h"
#include "process.h"

#define DISK 96290
#define TAPE 96291
#define PRINTER 96292

typedef struct scheduler {
    queue highPriority;
    queue lowPriority;
    int processingPID;
    int processorTime;
    int quantumTime;

    queue disk;
    int diskTime;
    int totalDiskTime;    
    queue tape;
    int tapeTime;
    int totalTapeTime;
    queue printer;
    int printerTime;
    int totalPrinterTime;
} Scheduler;

void startScheduler (Scheduler* scheduler, int max_process, int timeDiskIO, int timeTapeIO, int timePrinterIO, int quantumTime) {
    newQueue(&(scheduler->disk), max_process);
    newQueue(&(scheduler->tape), max_process);
    newQueue(&(scheduler->printer), max_process);
    newQueue(&(scheduler->highPriority), max_process);
    newQueue(&(scheduler->lowPriority), max_process);
    scheduler->totalDiskTime = timeDiskIO;
    scheduler->totalTapeTime = timeTapeIO;
    scheduler->totalPrinterTime = timePrinterIO;
    scheduler->quantumTime = quantumTime;
    scheduler->diskTime = 0;
    scheduler->tapeTime = 0;
    scheduler->printerTime = 0;
    scheduler->processorTime = 0;
    scheduler->processingPID = 0;

}

void IOreturn (Scheduler* scheduler, int IO) {
    if (IO == DISK) {
        joinQueue(&(scheduler->lowPriority), exitQueue(&(scheduler->disk)));
    }
    if (IO == TAPE) {
        joinQueue(&(scheduler->highPriority), exitQueue(&(scheduler->tape)));
    }
    if (IO == PRINTER) {
        joinQueue(&(scheduler->highPriority), exitQueue(&(scheduler->printer)));
    }
}

void processIO (Scheduler* scheduler) {
    if ((scheduler->disk.inUse == 0) && !(isEmpty(scheduler->disk))) {
        scheduler->disk.inUse = 1;
    }

    if (scheduler->disk.inUse == 1) {
        scheduler->diskTime += 1;
        if (scheduler->diskTime > scheduler->totalDiskTime) {
            IOreturn(scheduler, DISK);
            scheduler->diskTime = 0;
            if (isEmpty(scheduler->disk)) {
                scheduler->disk.inUse = 0;
            }
        }
    }

    if ((scheduler->tape.inUse == 0) && !(isEmpty(scheduler->tape))) {
        scheduler->tape.inUse = 1;
    }

    if (scheduler->tape.inUse == 1) {
        scheduler->tapeTime += 1;
        if (scheduler->tapeTime > scheduler->totalTapeTime) {
            IOreturn(scheduler, TAPE);
            scheduler->tapeTime = 0;
            if (isEmpty(scheduler->tape)) {
                scheduler->tape.inUse = 0;
            }
        }
    }

    if ((scheduler->printer.inUse == 0) && !(isEmpty(scheduler->printer))) {
        scheduler->printer.inUse = 1;
    }

    if (scheduler->printer.inUse == 1) {
        scheduler->printerTime += 1;
        if (scheduler->printerTime > scheduler->totalPrinterTime) {
            IOreturn(scheduler, PRINTER);
            scheduler->printerTime = 0;
            if (isEmpty(scheduler->printer)) {
                scheduler->printer.inUse = 0;
            }
        }
    }

}

void printAllQueues (Scheduler scheduler) {

    if (!isEmpty(scheduler.highPriority)){
        printw ("\nFila de Alta priority: ");
        printQueue (scheduler.highPriority);
    }
    else {
        printw ("\nFila de Alta priority: \n");
    }
    if (!isEmpty(scheduler.lowPriority)){
        printw ("Fila de Baixa priority: ");
        printQueue (scheduler.lowPriority);
    }
    else {
        printw ("Fila de Baixa priority: \n");
    }
    if (!isEmpty(scheduler.disk)){
        printw ("Fila do Disco: ");
        printQueue (scheduler.disk);
    }
    else {
        printw ("Fila do Disco: \n");
    }
    if (!isEmpty(scheduler.tape)){
        printw ("Fila da Fita: ");
        printQueue (scheduler.tape);
    }
    else {
        printw ("Fila da Fita: \n");
    }
    if (!isEmpty(scheduler.printer)){
        printw ("Fila da Impressora: ");
        printQueue (scheduler.printer);
    }
    else {
        printw ("Fila da Impressora: \n");
    }
}

void process(Scheduler *scheduler, Process* processes, int time) {

    int index;

    if (scheduler->processingPID == 0) {
        scheduler->processingPID = exitQueue(&(scheduler->highPriority));
        if(scheduler->processingPID == 0) {
            scheduler->processingPID = exitQueue(&(scheduler->lowPriority));
        }
        scheduler->processorTime += 1;

        index = getProcessByPID(processes, scheduler->processingPID);

        processes[index].status = 1;
        processes[index].programCounter += 1;
    }
    else {
        scheduler->processorTime = scheduler->processorTime + 1;

        index = getProcessByPID(processes, scheduler->processingPID);
        
        if (processes[index].status == 1) {
            processes[index].programCounter += 1;
        }

        int i;
        for (i = 0; i < processes[index].diskNumber; i++) {
            if ((processes[index].programCounter == processes[index].diskRequests[i]) && (processes[index].status == 1)) {
                processes[index].status = 0;
                processes[index].priority = 2;
                scheduler->processorTime = scheduler->quantumTime;
                joinQueue(&(scheduler->disk), processes[index].pid);
            }
        }
        for (i = 0; i < processes[index].tapeNumber; i++) {
            if ((processes[index].programCounter == processes[index].tapeRequests[i]) && (processes[index].status == 1)) {
                processes[index].status = 0;
                processes[index].priority = 1;
                scheduler->processorTime = scheduler->quantumTime;
                joinQueue(&(scheduler->tape), processes[index].pid);
            }
        }
        for (i = 0; i < processes[index].printerNumber; i++) {
            if ((processes[index].programCounter == processes[index].printerRequests[i]) && (processes[index].status == 1)) {
                processes[index].status = 0;
                processes[index].priority = 1;
                scheduler->processorTime = scheduler->quantumTime;
                joinQueue(&(scheduler->printer), processes[index].pid);
            }
        }
        
        if (processes[index].programCounter == processes[index].service) {
            processes[index].end = time;
            processes[index].status = 0;
        }

        if (scheduler->processorTime >= scheduler->quantumTime) {
            scheduler->processorTime = 0;
            if (processes[index].status == 1) {
                processes[index].priority = 2;
                joinQueue(&(scheduler->lowPriority), processes[index].pid);
            }
            processes[index].status = 0;
            if (!isEmpty(scheduler->highPriority)){
                index = getProcessByPID(processes, exitQueue(&(scheduler->highPriority)));
            }
            else {
                index = getProcessByPID(processes, exitQueue(&(scheduler->lowPriority)));
            }
            scheduler->processingPID = processes[index].pid;
            processes[index].status = 1;
        }
    }
}

void newProcess(Scheduler *scheduler, int PID) {
    joinQueue(&(scheduler->highPriority), PID);
}
