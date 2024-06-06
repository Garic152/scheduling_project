#include "../lib/HRRN.h"
#include <stdio.h>

static queue_object *HRRN_queue;
// You can add more global variables and structs here
int current_time = 0;

float response_ratio(process *p) {
  if (p == NULL) {
    fprintf(stderr, "Trying to access process NULL pointer");
    exit(EXIT_FAILURE);
  }
  // Calculates the response ratio of a process
  int waiting_time = current_time - p->start_time;

  if (p->time_left == 0) {
    return 99999.99;
  }

  return (waiting_time + p->time_left) / (float)p->time_left;
}

void HRRN_sort_queue() {
  if (HRRN_queue->next == NULL || HRRN_queue->next->next == NULL) {
    return;
  }

  queue_object *prev = HRRN_queue;
  queue_object *current = HRRN_queue->next; // Start from the second element
  queue_object *nextNode = HRRN_queue;

  // Traverse the queue until the last node
  while (current->next != NULL) {
    nextNode = current->next;

    // If the response ratio of the current node is greater than the next node
    if (response_ratio(current->object) > response_ratio(nextNode->object)) {
      // Move the current node to the end of the queue
      prev->next = nextNode;
      current->next = nextNode->next;
      nextNode->next = current;

      // Update the pointers for the next iteration
      prev = nextNode;
    } else {
      // Move to the next pair of nodes
      prev = current;
      current = nextNode;
    }
  }
}

process *HRRN_tick(process *running_process) {
  current_time += 1;

  if (running_process == NULL || running_process->time_left == 0) {
    HRRN_sort_queue();
    running_process = queue_poll(HRRN_queue);
  }
  if (running_process != NULL) {
    running_process->time_left -= 1;
  }
  return running_process;
}

int HRRN_startup() {
  HRRN_queue = new_queue();
  if (HRRN_queue == NULL) {
    return 1;
  }
  return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process) {
  if (arriving_process != NULL) {
    // printf("\nProcess %c arrived at time %d\n", arriving_process->id,
    // current_time);
    queue_add(arriving_process, HRRN_queue);
  }
  return running_process;
}

void HRRN_finish() { free_queue(HRRN_queue); }
