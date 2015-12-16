/**
 * @file     xsh_hello.c
 * @provides xsh_hello
 *
 * $Id$
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */
 
#include <xinu.h>
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
shellcmd xsh_hello(int nargs, char *args[])
{
         
        
          /* Output help, if '--help' argument was supplied */
           if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	   { 
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tDisplays the message - hello <string>, Welcome to the world of xinu !\n");
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
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return 1;
        }
         /* check if the user has provided invalid options */
         if (nargs == 2) {
               /* Print the output, Hello <string>, Welcome to the world of xinu ! */
                if ((args[1][0] >= 'a' && args[1][0] <= 'z') || (args[1][0] >= 'A' && args[1][0] <= 'Z') ) {
                        printf("Hello %s, Welcome to the world of xinu !", args[1]);
               } else {
                        fprintf(stderr, "%s: invalid argument\n", args[0]);
                        fprintf(stderr,
                                "Try '%s --help' for more information\n",
                                args[0]);
                        return 1;
                }
        }

     
 
   
       

    /* Just so the next prompt doesn't run on to this line */
    printf("\n");
 
    /* there were no errors so, return OK */
    return 0;
}

