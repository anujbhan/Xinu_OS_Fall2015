#include "future.h"
#include "port_futures.h"

/* Declare the Port-Future mapping */
port_futures port_futures_tab[PORT_FUTURE_SIZE];

/* Initialize Port Future mapping */
void initPortFuture(){
	/* index for slot */
	uint32 i;
	/* Initialize local port and future table enteries */
	for(i = 0; i < PORT_FUTURE_SIZE; i++)
		port_futures_tab[i].mapState = PORT_FUTURE_FREE;
}

int32 addPortFuture(future* f, uint32 localPort){
	
	intmask	mask;			/* Saved interrupt mask		*/
	int32 i;			/* index for iterating slot */

	/* Ensure only one process can access the UDP table at a time	*/
	mask = disable();

	/* Check for unused space and Map port with future */
	for(i = 0; i < PORT_FUTURE_SIZE; i++){
		if(port_futures_tab[i].mapState == PORT_FUTURE_USED)
			continue;
		port_futures_tab[i].port = localPort;
		port_futures_tab[i].fut = f;
		port_futures_tab[i].mapState = PORT_FUTURE_USED;
		break;
	}

	/* Check if index of iterating slot greater than PORT_FUTURE_SIZE */
	if(i < PORT_FUTURE_SIZE){
		restore(mask);
		return OK;
	}else{
		printf("Unable to find any empty port\n");
		restore(mask);
		return SYSERR;
	}
}

future* checkPortFuture(uint32 localPort){
	
	intmask	mask;			/* Saved interrupt mask		*/
	int32 i;			/* index for iterating slot */

	/* Ensure only one process can access the UDP table at a time	*/
	mask = disable();

	for(i = 0; i < PORT_FUTURE_SIZE; i++){
		if(port_futures_tab[i].mapState == PORT_FUTURE_FREE)
			continue;
		if(port_futures_tab[i].port == localPort){
			//printf("Match for Port found. Returning future\n");
			return port_futures_tab[i].fut;
		}
	}
	/* no entry future, resume normal functionality */
	return NULL;
	
}


