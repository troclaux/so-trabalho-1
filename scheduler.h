#include "scheduler.c"

struct Scheduler;

Scheduler newScheduler(int quantum, int numberOfProcesses, Process* allProcesses, unsigned diskTimer, unsigned tapeTimer, unsigned printerTimer);
void updateProcesses(Scheduler *scheduler);
bool unfinishedProcessesExist(Scheduler *scheduler);
void computeExecutionCycles(Scheduler *scheduler, Process* allProcesses);
void killScheduler(Scheduler *scheduler);