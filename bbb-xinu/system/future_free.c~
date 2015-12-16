#include "future.h"

syscall future_free(future* f) {
	syscall state;
	pid32 consPID, prodPID;
	if (f->flag == FUTURE_EXCLUSIVE)
	{
		state = freemem((char *)f, sizeof(*f));
		
	}
	else if (f->flag == FUTURE_SHARED)
	{
		while (!isEmpty(f->get_queue)) {
			consPID = dequeueFut(&(f->get_queue->head), &(f->get_queue->tail));
			// resume suspended consumer from PID
			kill(consPID);
		}
		state = freemem((char *)f, sizeof(f->get_queue));
			state = freemem((char *)f, sizeof(*f));
		
	}
	else {
		while (!isEmpty(f->get_queue)) {
			consPID = dequeueFut(&(f->get_queue->head), &(f->get_queue->tail));
			// resume suspended consumer from PID
			kill(consPID);
		}
		while (!isEmpty(f->get_queue)) {
			prodPID = dequeueFut(&(f->set_queue->head), &(f->set_queue->tail));
			// resume suspended consumer from PID
			kill(prodPID);
		}
		state = freemem((char *)f, sizeof(f->get_queue));
			if (state == SYSERR) {
				return SYSERR;
			}
			
		state = freemem((char *)f, sizeof(*f));
			
	}
      return state;

}
