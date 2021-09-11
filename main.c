#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"

int main() {
  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base

  Process process1;
  process1 = newProcess(100, 1);
  printProcess(process1);

  return 0;
}