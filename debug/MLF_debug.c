#include "../lib/MLF.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  process *proc;
  uint8_t level;
} ProcessInfo;

static queue_object **MLF_queues;
uint8_t MAX_LEVEL = 4;
uint8_t TIME = 0;
uint8_t QUANTUM = 1;

uint8_t basic_pow(int base, int exp) {
  uint8_t result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

uint8_t return_level(int number) {
  if (number < 0) {
    return -1;
  } else if (number == 1 || number == 2) {
    return number;
  } else {
    return 3;
  }
}

void print_queue() {
  printf("-Printing Queues\n");

  for (uint8_t level = 0; level < MAX_LEVEL; level++) {
    printf("--Printing Level %u\n", level);
    queue_object *MLF_queue = MLF_queues[level];
    if (MLF_queue->next == NULL) {
      printf("---Level %u is empty\n", level);
    }

    queue_object *iterator = MLF_queue->next;

    while (iterator != NULL) {
      printf("---%c, %d\n", ((process *)(iterator->object))->id,
             ((process *)(iterator->object))->time_left);
      iterator = iterator->next;
    }
  }
}

void MLF_sort_queue(queue_object *MLF_queue) {
  if (MLF_queue->next == NULL || MLF_queue->next->next == NULL) {
    return;
  }

  queue_object *prev = MLF_queue;
  queue_object *current = MLF_queue->next;
  queue_object *nextNode = MLF_queue;

  while (current->next != NULL) {
    nextNode = current->next;

    if (((process *)(current->object))->priority >
        ((process *)(nextNode->object))->priority) {
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

ProcessInfo get_next_process() {
  ProcessInfo result = {NULL, 0};

  for (uint8_t level = 0; level < MAX_LEVEL; level++) {
    if (MLF_queues[level]->next == NULL) {
      continue;
    }
    MLF_sort_queue(MLF_queues[level]);
    result.proc = queue_poll(MLF_queues[level]);
    result.level = level;
    return result;
  }
  return result;
}

process *MLF_tick(process *running_process) {
  ProcessInfo result;
  if (running_process == NULL || running_process->time_left == 0) {
    result = get_next_process();
    if (result.proc == NULL) {
      QUANTUM = 1;
      TIME = 0;
      return NULL;
    }
    QUANTUM = basic_pow(2, result.level);
    TIME = 0;
    running_process = result.proc;
  }

  if (TIME >= QUANTUM) {
    if (QUANTUM == 8) {
    } else {
      printf("-Calculated new level for %c: %u\n", running_process->id,
             return_level(QUANTUM));
      queue_add(running_process, MLF_queues[return_level(QUANTUM)]);
      result = get_next_process();
      if (result.proc == NULL) {
        return NULL;
      }
      running_process = result.proc;
      printf("-New process: %c\n", running_process->id);
      QUANTUM = basic_pow(2, result.level);
      TIME = 0;
    }
  }

  running_process->time_left -= 1;
  TIME += 1;

  printf("\n\nAfter tick %u with quantum %u\n", TIME, QUANTUM);
  print_queue();

  printf("Time left for process %c is: %d\n", running_process->id,
         running_process->time_left);

  return running_process;
}

int MLF_startup() {
  MLF_queues = (queue_object **)malloc(MAX_LEVEL * sizeof(queue_object *));
  if (MLF_queues == NULL) {
    return 1;
  }

  for (uint8_t i = 0; i < MAX_LEVEL; i++) {
    MLF_queues[i] = new_queue();
  }

  return 0;
}

process *MLF_new_arrival(process *arriving_process, process *running_process) {
  if (arriving_process != NULL) {
    if (running_process != 0) {
      queue_add(arriving_process, MLF_queues[0]);
    } else {
      return arriving_process;
    }
  }
  return running_process;
}

void MLF_finish() {
  for (uint8_t i = 0; i < MAX_LEVEL; i++) {
    if (MLF_queues[i] != NULL) {
      free_queue(MLF_queues[i]);
      MLF_queues[i] = NULL;
    }
  }
}
