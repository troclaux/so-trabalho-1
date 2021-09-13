#include "scheduler.c"

struct Scheduler;

Scheduler newScheduler(int quantum, int numberOfProcesses, Process* allProcesses, unsigned diskQuantum, unsigned tapeQuantum, unsigned printerQuantum);
void processIOQueue(Scheduler *scheduler);
void updateRunningProcess(Scheduler *scheduler);
void updateScheduler(Scheduler *scheduler);
bool unfinishedProcessesExist(Scheduler *scheduler);
void printScheduler(Scheduler *scheduler);
void computeExecutionCycles(Scheduler *scheduler, Process* allProcesses);
void killScheduler(Scheduler *scheduler);