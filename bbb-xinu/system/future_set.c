#include "future.h"

syscall future_set(future *f, int *value) {
	 if (f->flag == FUTURE_EXCLUSIVE){
 	if(f->state == FUTURE_VALID){ //fairly simple, check if the value is already present in future, if yes return error else produce the value.
 		return SYSERR;
 	}
 	else if(f->state == FUTURE_EMPTY){
 		f->value = value;
 		*(f->value) = *value;
 	}
 	else{
 		*(f->value) = *value;
		//printf("resuming  PID %u\n",f->pid);
 		resume(f->pid);
 	}
 	f->state = FUTURE_VALID;
 	return OK;

	}
	else if (f->flag == FUTURE_SHARED) {
		// set value for future, for consumer to consume
		f->value = value;
		f->state = FUTURE_VALID;
		return OK;
	}
	else if (f->flag == FUTURE_QUEUE) {
		// set value for future, for consumer to consume
		f->value = value;
		if (!isEmpty(f->get_queue)) {
			f->state = FUTURE_WAITING;
		}
		else {
			f->state = FUTURE_VALID;
		}
		return OK;
	}
}
