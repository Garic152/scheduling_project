#include "../lib/HRRN.h"
#include <stdio.h>

typedef struct {
  char id;
  int time;
} ProcessInfo;

#define MAX_PROCESSES 100
ProcessInfo process_info_array[MAX_PROCESSES];
int process_count = 0;

static queue_object *HRRN_queue;
// You can add more global variables and structs here
int current_time = 0;

void print_queue() {
  // printf("\n----------------\n");
  // printf("Printing Queue\n");

  if (HRRN_queue->next == NULL) {
    // printf("Empty Queue\n");
    // printf("----------------\n");
    return;
  }

  queue_object *iterator = HRRN_queue->next;

  while (iterator != NULL) {
    // printf("%c, %d\n", ((process *)(iterator->object))->id,
    //        ((process *)(iterator->object))->time_left);
    iterator = iterator->next;
  }
  // printf("----------------\n");
}

float response_ratio(process *p) {
  // Calculates the response ratio of a process
  if (p == NULL) {
    fprintf(stderr, "Trying to access process NULL pointer");
    exit(EXIT_FAILURE);
  }

  if (p->time_left == 0) {
    return 99999.99;
  }

  int init_time = -1;

  for (int i = 0; i < process_count; i++) {
    if (process_info_array[i].id == p->id) {
      init_time = process_info_array[i].time;
      break;
    }
  }
  if (init_time == -1) {
    // fprintf(stderr, "Did not find arrival time for process ID: %c", p->id);
  }

  int waiting_time =
      (current_time - p->start_time) - (init_time - p->time_left);
  printf("Response-Ratio for process %c is: %.2f\n", p->id,
         (waiting_time + p->time_left) / (float)p->time_left);

  return ((waiting_time + p->time_left) / (float)p->time_left);
}

void HRRN_sort_queue() {
  if (HRRN_queue->next == NULL || HRRN_queue->next->next == NULL) {
    return;
  }

  queue_object *prev = HRRN_queue;
  queue_object *current = HRRN_queue->next; // Start from the second element
  queue_object *nextNode = HRRN_queue;

  while (current->next != NULL) {
    nextNode = current->next;

    if (response_ratio(current->object) > response_ratio(nextNode->object)) {
      prev->next = nextNode;
      current->next = nextNode->next;
      nextNode->next = current;

      prev = nextNode;
    } else if (response_ratio(current->object) ==
               response_ratio(nextNode->object)) {
      if (((process *)(current->object))->start_time >
          ((process *)(nextNode->object))->start_time) {
        prev->next = nextNode;
        current->next = nextNode->next;
        nextNode->next = current;

        prev = nextNode;
      }
    } else {
      prev = current;
      current = nextNode;
    }
  }
}

process *HRRN_tick(process *running_process) {
  print_queue();
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
  // printf("\n\nTick %d\n", current_time);

  if (arriving_process != NULL) {
    // printf("Process %c arrived\n", arriving_process->id);

    if (process_count < MAX_PROCESSES) {
      process_info_array[process_count].id = arriving_process->id;
      process_info_array[process_count].time = arriving_process->time_left;
      process_count += 1;
    } else {
      // fprintf(stderr, "Process info array is full!\n");
    }

    queue_add(arriving_process, HRRN_queue);
  }
  return running_process;
}

void HRRN_finish() { free_queue(HRRN_queue); }
