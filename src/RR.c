#include "../lib/RR.h"
#include <stdio.h>

static queue_object *RR_queue;
int time = 0;
int RR;
// You can add more global variables

void RR_update_queue() {
  if (RR_queue->next == NULL || RR_queue->next->next == NULL) {
    return;
  }

  queue_object *current = RR_queue->next; // Start from the second element
  queue_object *last = RR_queue->next;

  while (last->next != NULL) {
    last = last->next;
  }

  RR_queue->next = current->next;
  last->next = current;
  current->next = NULL;
}

process *RR_select_next() {
  if (RR_queue->next == NULL) {
    return NULL;
  }
  return RR_queue->next->object;
}

process *RR_tick(process *running_process) {
  if (running_process == NULL || running_process->time_left == 0) {
    time = 0;
    running_process = queue_poll(RR_queue);
    if (running_process == 0) {
      return NULL;
    }
  }

  if (time >= RR) {
    time = 0;
    queue_add(running_process, RR_queue);
    running_process = queue_poll(RR_queue);
  }

  running_process->time_left -= 1;
  time += 1;

  return running_process;
}

int RR_startup(int quantum) {
  RR_queue = new_queue();
  RR = quantum;
  if (RR_queue == NULL) {
    return 1;
  }
  return 0;
}

process *RR_new_arrival(process *arriving_process, process *running_process) {
  if (arriving_process != NULL) {
    if (running_process == NULL) {
      return arriving_process;
    } else {
      queue_add(arriving_process, RR_queue);
    }
  }
  return running_process;
}

void RR_finish() {
  if (RR_queue != NULL) {
    free_queue(RR_queue);
    RR_queue = NULL;
  }
}
