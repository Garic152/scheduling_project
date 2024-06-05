#include "../lib/LCFS.h"

static queue_object *LCFS_queue;
// You can add more global variables here

int LCFS_startup() {
  // Reserves memory for queue_object
  LCFS_queue = new_queue();
  if (LCFS_queue == NULL) {
    return 1;
  }
  return 0;
}

process *LCFS_tick(process *running_process) {
  // Checks if process still got time left
  if (running_process->time_left > 0) {
    running_process->time_left--;
    if (running_process->time_left > 0) {
      return running_process;
    } else {
      free(running_process);
    }
  }

  process *next_process = (process *)queue_poll(LCFS_queue);

  return next_process;
}

process *LCFS_new_arrival(process *arriving_process, process *running_process) {
  if (running_process) {
    queue_add(arriving_process, LCFS_queue);
  }

  return arriving_process;
}

void LCFS_finish() {
  if (LCFS_queue != NULL) {
    free_queue(LCFS_queue);
    LCFS_queue = NULL;
  }
}
