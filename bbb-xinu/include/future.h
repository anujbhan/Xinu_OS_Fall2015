#include <xinu.h>
#include "queue_fut.h"
#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED 2
#define FUTURE_QUEUE 3

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
   struct queueFut* set_queue;
   struct queueFut* get_queue;
   //qid16 set_queue;
   //qid16 get_queue;
} future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);

int future_prod(future *f);
int future_cons(future *f); 

/* Semaphores to sync between enqueue and dequeue operations */
extern sid32 getQueueSem, setQueueSem;
#endif /* _FUTURE_H_ */
