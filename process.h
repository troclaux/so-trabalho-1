#include "process.c"

struct Process;
bool checkIntInArray(int number, int *array);
int *generateIOTime(int serviceTime, int *IOArray1, int *IOArray2);
Process newProcess(int pid, int ppid);
void printProcess(Process process);
void killProcess(Process process);