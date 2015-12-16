#ifndef _NETWORK_FUTURES_H_
#define _NETWORK_FUTURES_H_

#include "future.h"

extern sid32 finalSumSem; /*Semaphore declaration of synchronization while updating the sum*/

/*function Prototype*/
int child_consumer(int *sum, char *msg, future *f, uint32 localPort);

#endif /* _FUTURE_H_ */
