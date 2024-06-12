#include "../lib/SRTN.h"

static queue_object *SRTN_queue;
// You can add more global variables here

void SRTN_sort_queue() {
  if (SRTN_queue->next == NULL || SRTN_queue->next->next == NULL) {
    return;
  }

  queue_object *prev = SRTN_queue;
  queue_object *current = SRTN_queue->next; // Start from the second element
  queue_object *nextNode = SRTN_queue;

  while (current->next != NULL) {
    nextNode = current->next;

    if (((process *)(current->object))->time_left <
        ((process *)(nextNode->object))->time_left) {
      prev->next = nextNode;
      current->next = nextNode->next;
      nextNode->next = current;

      prev = nextNode;
    } else {
      prev = current;
      current = nextNode;
    }
  }
}

process *SRTN_tick(process *running_process) {
  if (running_process == NULL || running_process->time_left == 0) {
    SRTN_sort_queue();
    running_process = queue_poll(SRTN_queue);
    if (running_process == NULL) {
      return NULL;
    }
  }
  if (running_process->time_left > 0) {
    running_process->time_left -= 1;
  }
  return running_process;
}

int SRTN_startup() {
  SRTN_queue = new_queue();
  if (SRTN_queue == NULL) {
    return 1;
  }
  return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process) {
  if (arriving_process == NULL) {
    return running_process;
  }

  if (running_process == NULL) {
    return arriving_process;
  }

  if (arriving_process->time_left < running_process->time_left) {
    queue_add(running_process, SRTN_queue);
    return arriving_process;
  } else {
    queue_add(arriving_process, SRTN_queue);
    return running_process;
  }
}

void SRTN_finish() {
  if (SRTN_queue != NULL) {
    free_queue(SRTN_queue);
    SRTN_queue = NULL;
  }
}
