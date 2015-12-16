#include "future.h"
#include "prodcons.h"
#include <stdio.h>


int future_cons(future *fut) {
	int getValue, status = 0;
        pid32 consPID = getpid(), prodsPID;
	if (fut->flag == FUTURE_SHARED) {
		
		if (fut->state == FUTURE_VALID) {
			// value already set by producer; get value! <<prod->cons>>
			status = future_get(fut, &getValue);
		}
		else {
			// wait on enqueue semaphore
			wait(getQueueSem);
			// enqueue consumer  <<cons1>>, <<cons1->cons2>>
			if (fut->state == FUTURE_EMPTY) {
				fut->state = FUTURE_WAITING;
			}
			enqueueFut(consPID, &(fut->get_queue->head), &(fut->get_queue->tail));
			// release enqueue semaphore
			signal(getQueueSem);
			// suspend itself
			suspend(consPID);
			// resumed by first available producer
			status = future_get(fut, &getValue);
		}
         printf("Consumer %d produced %d\n", consPID, getValue);
	}
	else if (fut->flag == FUTURE_QUEUE) {

		
		wait(getQueueSem);

		if (fut->state == FUTURE_VALID) {
			prodsPID = dequeueFut(&(fut->set_queue->head), &(fut->set_queue->tail));
			status = future_get(fut, &getValue);
                        resume(prodsPID);
		}
		else {
			if (fut->state == FUTURE_EMPTY) {
				fut->state = FUTURE_WAITING;
			}
			enqueueFut(consPID, &(fut->get_queue->head), &(fut->get_queue->tail));
			signal(getQueueSem);
			suspend(consPID);
			status = future_get(fut, &getValue);
		}
		
         printf("Consumer %d produced %d\n", consPID, getValue);

	}
	else if (fut->flag == FUTURE_EXCLUSIVE) {
		status = future_get(fut, &getValue);
                printf("it produced %d\n", getValue);
	}

	if (status < 1) {
		printf("future_get failed\n");
		return SYSERR;
	}
        
	if(childCount < 1) {
         send(prodconsPID, 1);
         printf("consumer sent the message: %d\n",consPID);
        } else {
         childCount--;
        }
	return OK;
}
