#include "future.h"
future* future_alloc(int future_flag) {
	future *future_inst;
	future_inst = (future *)getmem(sizeof(*future_inst));
	future_inst->flag = future_flag;
	future_inst->state = FUTURE_EMPTY;
	if (future_flag == FUTURE_SHARED) {
		//printf("0.1\t");
		queueFut *newGetQueueFut;
		newGetQueueFut = (queueFut *)getmem(sizeof(struct queueFut));
		future_inst->get_queue = newGetQueueFut;
		queueNode *newGetQueueNode;
		newGetQueueNode = (queueNode *)getmem(sizeof(struct queueNode)); //dummy 
		newGetQueueNode->next = NULL;
		newGetQueueNode->pid = 0;
		future_inst->get_queue->head = future_inst->get_queue->tail = newGetQueueNode;
		
	}
	else if (future_flag == FUTURE_QUEUE) {
		queueFut *newSetQueueFut, *newGetQueueFut;
		newGetQueueFut = (queueFut *)getmem(sizeof(struct queueFut));
		newSetQueueFut = (queueFut *)getmem(sizeof(struct queueFut));
		
		future_inst->get_queue = newGetQueueFut;
		future_inst->set_queue = newSetQueueFut;
		queueNode *newGetQueueNode;
		queueNode *newSetQueueNode;
		newGetQueueNode = (queueNode *)getmem(sizeof(struct queueNode)); //dummy 
		newGetQueueNode->pid = 0;
		newGetQueueNode->next = NULL;
		future_inst->get_queue->head = future_inst->get_queue->tail = newGetQueueNode;

		newSetQueueNode = (queueNode *)getmem(sizeof(struct queueNode)); //dummy 
		newSetQueueNode->pid = 0;
		newSetQueueNode->next = NULL;
		future_inst->set_queue->head = future_inst->set_queue->tail = newSetQueueNode;

	}
	
	return future_inst;
}
