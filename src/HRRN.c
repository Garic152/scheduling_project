#include "../lib/HRRN.h"

static queue_object *HRRN_queue;
// You can add more global variables and structs here
int current_time = 0;

process *HRRN_tick(process *running_process) {
  current_time += 1;
  if (running_process != NULL) {
    running_process->time_left -= 1;
    if (running_process->time_left > 0) {
      return running_process;
    }
  }
  return NULL;
}

int HRRN_startup() {
  HRRN_queue = new_queue();
  if (HRRN_queue == NULL) {
    return 1;
  }
  return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process) {
  // TODO
  return NULL;
}

void HRRN_finish() {
  // TODO
}
