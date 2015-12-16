
#include "prodcons.h"
#include <xinu.h>

void producer(int count){
/* Loop to produce values less than equal to count */
int produce;
	for(produce = 1; produce <= count; produce++){
                wait(produced);
		printf("Produced : %d",produce);
		/* Assign produced value */
		n=produce;
                signal(consumed);
	}
 }

