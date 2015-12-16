#include <xinu.h>
#include <stdio.h>
#include <string.h>
 
/**
 * Shell command walks the list of free memory blocks and echoes address with length of each free block
 * @param stdin descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_freememwalk(int nargs, char *args[])
{
         
        
          /* Output help, if '--help' argument was supplied */
           if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	   { 
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tWalks the list of free memory blocks and prints address with length of each free block\n");
                printf("Options (one per invocation):\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
 
           }
          
	if(nargs == 1){
		// call function to walk the list of free mem
		printf("Initial free memory blocks\n");
		getmemList();
		printf("making memory segmented to demonstrate the best fit behavior.....\n");
		int *n4,*n1,*n2,*n3, state;
		n1 = (int *)getmem((sizeof(int))*3);
                n2 = (long *)getmem((sizeof(int))*2);
                n3 = (int *)getmem(sizeof(int));
                n4 = (int *)getmem((sizeof(int))*4);
                printf("Allocated 100,200,100 de-allocated 1st and 3rd.\n");
                state = freemem((char *)n1, (sizeof(int))*3);
                state = freemem((char *)n3, sizeof(int));
                getmemList();
                printf("Calling the Getmem with best fit\n");
		int *newnew;
		newnew = (int *)getmemBest(sizeof(int));
		getmemList();
                
		return 0;

	}
        /* Output Error, if incorrect number of arguments are given */
        if (nargs > 1) {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return 1;
        }
         
    /* Just so the next prompt doesn't run on to this line */
    printf("\n");
 
    /* there were no errors so, return OK */
    return 0;
}

