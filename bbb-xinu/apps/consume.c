
#include "prodcons.h"
#include <xinu.h>
void consumer(int count){
int consume;
	for(consume = 1; consume <= count; consume++){
                wait(consumed);
		printf("  Consumed : %d\n",n);
		n--;
                signal(produced);
	}
	send(prodconsPID,1);
}

