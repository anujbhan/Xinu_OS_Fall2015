#include "future.h"
syscall future_get(future *f, int *value)
{
	if (f->flag == FUTURE_EXCLUSIVE) {
		if (f->state == FUTURE_EMPTY) //Check if the future is empty, if yes update the state to waiting, to show that processes are waiting to consume 
		{
			f->state = FUTURE_WAITING;
			f->pid = getpid();
			f->value = value;
			//printf("suspending\n");
			suspend(f->pid); // The consumer process is suspended until the value is produced.
			*value = *(f->value);
			//printf("Value in get %d\n", *value);			
			f->state = FUTURE_EMPTY;
			return OK;
		}
		else if (f->state == FUTURE_VALID) //Check if the value is produced, if yes consume it.
		{
			*value = *(f->value);
			//printf("The getvalue variable : %d  *(f->value) : %d \n", *value, *(f->value));
			resume(f->pid);
			f->state = FUTURE_EMPTY;
			return OK;
		}
		else {
			return SYSERR;
		}

	}
	else if (f->flag == FUTURE_SHARED) {
		*value = *(f->value);
		return OK;
	}
	else if (f->flag == FUTURE_QUEUE) {
		*value = *(f->value);
		if (isEmpty(f->get_queue) && isEmpty(f->set_queue))
			f->state = FUTURE_EMPTY;
		return OK;
	}
}
