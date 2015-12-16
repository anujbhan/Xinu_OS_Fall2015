#include <xinu.h>
#include "queue_fut.h"

void enqueueFut(pid32 pid, struct queueNode **head, struct queueNode **tail) {
	queueNode *newQueueNode;
	newQueueNode = (queueNode *)getmem(sizeof(struct queueNode));
	newQueueNode->next = NULL;
	newQueueNode->pid = pid;
	if (*head == *tail) {
		(*head)->next = newQueueNode;
		(*tail) = newQueueNode;
	}
	else {
		(*tail)->next = newQueueNode;
		*tail = newQueueNode;
	}
}

pid32 dequeueFut(struct queueNode **head, struct queueNode **tail) {
	pid32 pid;
	syscall state;
	queueNode *deleteQueueNode;
	deleteQueueNode = (*head)->next;
	pid = deleteQueueNode->pid;

	if (*head == *tail) {
		return 0;
	}
	else if ((*head)->next == *tail) {

		*tail = *head;
		(*head)->next = (*tail)->next = NULL;
	}
	else {

		(*head)->next = deleteQueueNode->next;
	}
	state = freemem((char *)deleteQueueNode, sizeof(*deleteQueueNode));
	if (state == OK) {

		return pid;
	}
	else {
		return SYSERR;
	}


}

int isEmpty(struct queueFut *queueFutQ) {
	if (queueFutQ->head == queueFutQ->tail)
		return 1;
	else
		return 0;
}
