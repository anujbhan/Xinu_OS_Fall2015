#include "network_futures.h"
#include <stdio.h>
#include <string.h>
#include "port_futures.h"

int child_consumer(int *sum, char* msg, future *f, uint32 localPort){
	
	int partialSum;			/* To get the value of partial sum, TO-DO; Get a pointer to store the total sum */
	uid32 slot;			/* Slot used for sending message */
	int32 status; 			/* Status  */

	/* Assign own PID to the future */
	f->pid = getpid();
	//printf("Child cons, PID %u\n", f->pid);
	//printf("Local port - child_consumer %u\n", localPort);
	/* Send UDP packet */
	//char msg[] = "Yo";
	char dest_ip_str[] = "192.168.1.100";
	uint32 dest_port = 7891;
	
	/* Add mapping of Port and Future */
	status = addPortFuture(f, localPort);
	
	if(status == OK){
		/* Send packet to server */
		slot = net_send(msg, dest_ip_str, dest_port, localPort);
	}else
		return SYSERR;
	
	if(slot != SYSERR){
		//if(f->flag == FUTURE_EXCLUSIVE)
		//	printf("YES\n");
		/* future_get will hold the value of partial sum, go to sleep and be woken up by send */
		status = future_get(f, &partialSum);

		if(status == OK){
			wait(finalSumSem); 			/* Wait to get the lock to update the value */
			*sum += partialSum;			/* Update the final sum */
			printf("finalSum %d\n", *sum);
			signal(finalSumSem); 			/* release the lock for other process to take up */
			//printf("Partial sum %d f->value %d\n", partialSum, *(f->value));
		}
		else
			printf("Someting ugly happened");
		return status;
	}else
		return SYSERR;
	
}

