/**
 * @file     xsh_network_futures.c
 * @provides xsh_network_futures
 *
 * $Id$
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */
 
//include <xinu.h>
#include "network_futures.h"
#include "port_futures.h"
#include <stdio.h>
#include <string.h>

/**
 * Shell command echos input text to standard out.
 * @param stdin descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */

sid32 finalSumSem; 	/* Global defintion of semaphore to lock before updating */

shellcmd xsh_network_futures(int nargs, char *args[])
{
         
        
          /* Output help, if '--help' argument was supplied */
           if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	   { 
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tDemo for network futures\n");
                printf("Options (one per invocation):\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
 
           }
          
	
        /* Output Error, if incorrect number of arguments are given */
        if (nargs > 1) {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return 1;
        }
         /* check if the user has provided invalid options */
         if (nargs == 1) {
               /* Intanstiate EXCLUSIVE futures for each child processes */
		future *f_1, *f_2;
		f_1 = future_alloc(FUTURE_EXCLUSIVE);
		f_2 = future_alloc(FUTURE_EXCLUSIVE);
		
		/* Final destination to store the sum */
		int *finalSum; 
		finalSum = (int *)getmem(sizeof(int *));
		*finalSum = 0;
		
		/* Initialize semaphore for synch between updating */ 
		finalSumSem = semcreate(1);
		initPortFuture();
		
		/* Initialze port number for child process */
		uint32 childPort = 7962;

		/* Create child processes */
		resume(create(child_consumer, 1024, 20, "cCons1", 4, finalSum, "1_50", f_1, childPort++));
		resume(create(child_consumer, 1024, 20, "cCons2", 4, finalSum, "51_100", f_2, childPort++));
                
        }

     
    /* Just so the next prompt doesn't run on to this line */
    printf("\n");
 
    /* there were no errors so, return OK */
    return 0;
}

