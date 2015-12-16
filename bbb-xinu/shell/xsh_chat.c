/**
 * @file     xsh_chat.c
 * @provides xsh_chat
 *
 * $Id$
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */
 


#include "netIPC.h"
#include <string.h>
 
/**
 * Shell command echos input text to standard out.
 * @param stdin descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_chat(int nargs, char *args[])
{
         
        
          /* Output help, if '--help' argument was supplied */
           if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	   { 
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tSends the message to the destination, both specified as arguments - chat <message_to_send> <ip address> <port>\n");
                printf("Options (one per invocation):\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
 
           }
          
	if(nargs == 1){
		fprintf(stderr, "%s: too less arguments\n", args[0]);
		fprintf(stderr, "%s command accepts only one argument\n",args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
		return 1;

	}
        /* Output Error, if incorrect number of arguments are given */
        if (nargs > 2) {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
                return 1;
        }
         /* check if the user has provided invalid options */
         if (nargs == 2) {
               
			char *request = args[1];
                        int recvBufferln = 50;
                        char *response;
                        //response = (char *)getmem(sizeof(char) * recvBufferln);
                        //memcpy(" ",response,recvBufferln);
              		char dest_ip_str[] = "192.168.1.100";
			uint32 dest_port = 7891;
			uint32 local_port = 7892;
			uid32 slot;
                        slot = net_send(request, dest_ip_str, dest_port, local_port);
                        response = net_recv(slot);
                        printf("Message from server: %s\n",response);
                                
                        return 0;
                        
               } else {
                        fprintf(stderr, "%s: invalid argument\n", args[0]);
                        fprintf(stderr,
                                "Try '%s --help' for more information\n",
                                args[0]);
                        return 1;
                }
        

    /* Just so the next prompt doesn't run on to this line */
    printf("\n");
 
    /* there were no errors so, return OK */
    return 0;
}

