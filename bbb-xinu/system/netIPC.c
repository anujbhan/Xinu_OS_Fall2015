#include <xinu.h>

uid32 net_send(char *msg, char *dest_ip_str, uint32 dest_port,  uint32 local_port)
{
                      
			uint32 dest_ip;
			uint32 result;
			uid32 slot;

			/* convert dot-separated IP address to unsigned int */
			result = dot2ip(dest_ip_str, &dest_ip);
			

			/* check if IP converted to unsigned int */
			if(result == OK){

				/* register UDP port; get a slot */
				slot = udp_register(dest_ip, dest_port, local_port);
				
				/* check if slot already assigned, or no slot available */
				if(slot == SYSERR){
                                     return SYSERR;
                                }else{
					/* UDP send */
					result = udp_send(slot, msg, strlen(msg));
					
					return slot;
				}			
                              }else{  
				return SYSERR;
			}
}

char* net_recv(uid32 slot)
{
                        int recvBufferln = 50;
                        char *recvBuffer;
			recvBuffer = (char *)getmem(sizeof(char) * recvBufferln);  
                        int recvBufferTimeOut = 3000; 
			uint32 result;
                        
			/* udp reveive*/
                        char *msg;
                         
                        result = udp_recv(slot,recvBuffer,recvBufferln,recvBufferTimeOut);
                        
                        /* extract the message from buffer */
                        msg = (char *)getmem(sizeof(char) * result);
                        int i = 0;
                        if(result > 0)
                        {
                           
                           for( i = 0; i < result; i++) {
                        
                              msg[i] = recvBuffer[i];
                           }
                           msg[result] = '\0';
                         }else{
                          
                          return "Error";
                         }
                                              
                        
                        return msg;           
				
			
}



