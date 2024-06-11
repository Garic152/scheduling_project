#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object *PRIOP_queue;
//You can add more global variables here

void PRIOP_sort_queue()
{
  if (PRIOP_queue->next == NULL || PRIOP_queue->next->next == NULL) {
    return;
  }

  queue_object *prev = PRIOP_queue;
  queue_object *current = PRIOP_queue->next; // Start from the second element
  queue_object *nextNode = PRIOP_queue;

  while (current->next != NULL) {
    nextNode = current->next;

    if (((process *)(current->object))->priority < ((process *)(nextNode->object))->priority) {
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

process *select_last(process *running_process) {
  if (PRIOP_queue->next == NULL) {
    return NULL;
  }
	queue_object *iterator = PRIOP_queue->next;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}
	return iterator;
}

process *PRIOP_tick(process *running_process) {
  PRIOP_sort_queue();
  if (running_process == NULL || running_process->time_left == 0) {
    running_process = queue_poll(PRIOP_queue);
  } else {
  	running_process = select_last(PRIOP_queue);
  }
  if (running_process != NULL) {
    running_process->time_left -= 1;
  }
  return running_process;
}

int PRIOP_startup() {
  PRIOP_queue = new_queue();
  if (PRIOP_queue == NULL) {
    return 1;
  }
  return 0;
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process)
{
	if (arriving_process != NULL) {
		queue_add(arriving_process, PRIOP_queue);
	}
	return running_process;
}

void PRIOP_finish()
{
	// TODO
}
