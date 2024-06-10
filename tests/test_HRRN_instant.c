#include "../lib/process.h"
#include "../lib/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROCESS_COUNT 9
#define RR_QUANTUM 2
#define STRATEGY HRRN

int main() {
  /*Defining the processes                Arrival Dur.    Prio    ID
                                          uint    uint    uint    char*/
  process processes[PROCESS_COUNT] = {
      {0, 5, 2, 'A'}, {1, 5, 8, 'B'}, {2, 5, 7, 'C'}, {3, 5, 1, 'D'}};

  char *expected_result = "AAAA  BBBDDCCCCCGEEEHHHHHIIIIFFFFFFFFF";

  char *resulting_schedule =
      scheduler(processes, PROCESS_COUNT, STRATEGY, RR_QUANTUM);

  if (strcmp(expected_result, resulting_schedule)) {
    fprintf(stderr, "Wrong.\nExpected: \n%s\nGot:\n%s\n", expected_result,
            resulting_schedule);
    free(resulting_schedule);
    exit(1);
  }
  printf("Passed.\n");

  free(resulting_schedule);
  exit(0);
}
