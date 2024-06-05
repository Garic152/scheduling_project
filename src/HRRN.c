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
  return (waiting_time + p->time_left) / (float)p->time_left;
}

void HRRN_sort_queue() {
  // Updates the second last element in the queue to the process with the lowest
  // respones_ratio
  if (HRRN_queue->next == NULL) {
    return;
  }

  queue_object *current = HRRN_queue->next;
  queue_object *prev = HRRN_queue;
  queue_object *lowest_node = HRRN_queue->next;
  queue_object *lowest_prev = HRRN_queue;

  while (current->next != NULL) {
    if (response_ratio(current->object) < response_ratio(lowest_node->object)) {
      lowest_node = current;
      lowest_prev = prev;
    }
    prev = current;
    current = current->next;
  }

  lowest_prev->next = current;
  prev->next = lowest_node;
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
    printf("\nProcess %c arrived at time %d\n", arriving_process->id,
           current_time);
    queue_add(arriving_process, HRRN_queue);
  }
  return running_process;
}

void HRRN_finish() { free_queue(HRRN_queue); }
