#include "future.h"
#include "port_futures.h"
#include <stdio.h>
#include <string.h>
//#include <math.h>
int power(int n, int m){
    int res = 1;
    while(m != 0){
        res *= n;
        m--;
    }
    return res;
}

void net_udp_receive(uint32 localPort, uint32 slot){
	//printf("LP-%u,S-%u\n",localPort, slot);
	
	future* f; 			/* entry mapped with port */
	int j;
	int32 status; 			/* Status for future set */

	/* Check for an entry of Port and Future */
	f = checkPortFuture(localPort);

	if(f == NULL)
		return SYSERR;
	
	char* msgRecieve;
	int msgRecieve_int = 0;
	/* Call receive function with the slot */
	msgRecieve = net_recv(slot);
	//printf("Message in string %s\n",msgRecieve);
	/* Convert char to integer */
	int msgRecieveLen = strlen(msgRecieve);
	for(j = 0; j < msgRecieveLen; j++)
		//msgRecieve_int += (msgRecieve[j] - '0') * 10;
		msgRecieve_int += (msgRecieve[j] - '0') * power(10, msgRecieveLen - j);
	
	//printf("Message in int %d\n", msgRecieve_int);
	/* Set the value in future & Wake up child process*/
	//printf("Future PID : %u\n", f->pid);
	status = future_set(f, &msgRecieve_int);
	if(status == OK)
		printf("Future set %d\n", *(f->value));
	
}
