#include "future.h"
#include "prodcons.h"

int future_prod(future *fut) {
	pid32 prodPID;
	prodPID = getpid();
	int i, j, status;

	//j = (int)fut;
	j = prodPID;
	for (i = 0; i < 1000; i++) {
		j += 1;
	}
	if (fut->flag == FUTURE_SHARED) {
		pid32 consPID;
		// set the future
		status = future_set(fut, &j);
		// wait on getQueueSem, so all prev consumers are in the queue
		wait(getQueueSem);
		// If FUTURE_EMPTY, means consumer(s) are waiting
		if (fut->state != FUTURE_EMPTY) {
			// dequeue all consumers from getQueue, and set them value produced
			while (!isEmpty(fut->get_queue)) {
				consPID = dequeueFut(&(fut->get_queue->head), &(fut->get_queue->tail));
				// resume suspended consumer from PID
				resume(consPID);
			}
		}
		// release getQueueSem, so other consumer can get in
		signal(getQueueSem);
	}
	else if (fut->flag == FUTURE_QUEUE) {



		wait(getQueueSem);
		if (fut->state == FUTURE_WAITING) {
			pid32 consPID;
			status = future_set(fut, &j);

			consPID = dequeueFut(&(fut->get_queue->head), &(fut->get_queue->tail));
			signal(getQueueSem);
			resume(consPID);
		}
		else if (fut->state == FUTURE_EMPTY) {

			status = future_set(fut, &j);
			enqueueFut(prodPID, &(fut->set_queue->head), &(fut->set_queue->tail));
			fut->state = FUTURE_VALID;
			signal(getQueueSem);
			suspend(prodPID); //prod 1 here
		}
		else if (fut->state == FUTURE_VALID) {
			pri16 oldPrior;
			enqueueFut(prodPID, &(fut->set_queue->head), &(fut->set_queue->tail));
			signal(getQueueSem);
			suspend(prodPID); // prod 2 here first
			status = future_set(fut, &j);
			//increase your priority as world is cruel and won't let u suspend!
			oldPrior = chprio(prodPID, getprio(prodPID) + 1);
			signal(getQueueSem);
			suspend(prodPID); // prod 2 here second - after setting
			chprio(prodPID, oldPrior);
		}
		if (!isEmpty(fut->set_queue)) {
			resume(fut->set_queue->head->next->pid);
		}
		else {
			signal(getQueueSem);
		}
	}
	else {
		status = future_set(fut, &j);
	}
	if (status < 1) {
		printf("future_set_failed_%d\n", prodPID);
		return SYSERR;
	}
        if(childCount < 1) {
         send(prodconsPID, 1);
         printf("producer sent the message: %d\n",prodPID);
        } else {
         childCount--;
        }
	return OK;
}

