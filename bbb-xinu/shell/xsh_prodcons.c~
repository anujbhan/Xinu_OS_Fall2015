#include "prodcons.h"
#include "future.h"




int n;                 //Definition for global variable 'n'
sid32 produced, consumed, prodconWait; //definition for global semaphore values

sid32 getQueueSem, setQueueSem; //definition for global semaphores for enqueue and dequeue sync

pid32 prodconsPID; //Definition for global PID container

int childCount; // Number of child processes

shellcmd xsh_prodcons(int nargs, char *args[])
{
	int count = 0;
	future *f_exclusive, *f_shared, *f_queue; //declare futures which will be subsequently used 

        /*Instantiate semapores which will be used for synhronization*/
	consumed = semcreate(0);
	produced = semcreate(1);
        //prodconWait = semcreate(-9);
	prodconsPID = getpid(); //store the PID for prodcons process, to help communication between parent and child.
        childCount = 8;
	//Argument verifications and validations
	// Maximum of two values are allowed						
	if (nargs > 2)
	{
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return 1;
	}


	if (nargs == 2) {
		/* Output info for '--help' argument */
		if (strncmp(args[1], "--help", 7) == 0) {
			printf("Usage: %s [integer]\n\n", args[0]);
			printf("Description:\n");
			printf("\tSpawns two processes producer and consumer as two seperate processes\n");
			printf("Options (one per invocation):\n");
			printf("\t[integer]\n");
			printf("\tONLY ARGS\tRun naive producer consumer with semaphore implementation\n");
			printf("\t-f\\-F\tRun producer consumer with FUTURE implementation\n");
			printf("\t--help\tdisplay this help and exit\n");
			return 0;
		}
		//check if -f option is specified, which will will used to invoke future implementation
		else if ((strncmp(args[1], "-f", 3) == 0) || (strncmp(args[1], "-F", 3) == 0))
		{
			
			getQueueSem = semcreate(1);
			//setQueueSem = semcreate(1);
			f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
			f_shared = future_alloc(FUTURE_SHARED);
			f_queue = future_alloc(FUTURE_QUEUE);

			/*// Test FUTURE_EXCLUSIVE
			printf("Test FUTURE_EXCLUSIVE\n");
			resume(create(future_cons, 1024, 20, "fcons1", 1, f_exclusive));
			resume(create(future_prod, 1024, 20, "fprod1", 1, f_exclusive));
			*/
			
                        /*
			// Test FUTURE_SHARED
			printf("Test FUTURE_SHARED\n");
			resume(create(future_cons, 1024, 20, "fcons2", 1, f_shared));
			resume(create(future_cons, 1024, 20, "fcons3", 1, f_shared));
			resume(create(future_cons, 1024, 20, "fcons4", 1, f_shared));
			resume(create(future_cons, 1024, 20, "fcons5", 1, f_shared));
			resume(create(future_prod, 1024, 20, "fprod2", 1, f_shared));
			*/
                        
			// Test FUTURE_QUEUE
			printf("Test FUTURE_QUEUE\n");
			resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
			resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
			resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
			resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
			resume(create(future_prod, 1024, 20, "fprod3", 1, f_queue));
			resume(create(future_prod, 1024, 20, "fprod4", 1, f_queue));
			resume(create(future_prod, 1024, 20, "fprod5", 1, f_queue));
			resume(create(future_prod, 1024, 20, "fprod6", 1, f_queue));
			
			
                        //wait(prodconWait);
                        receive();
                        /*future_free(f_exclusive);
                        future_free(f_shared);
                        future_free(f_queue);*/
			printf("\n");
			return 0;

		}
		//check args[1] if present assign value to count
		else if (args[1][0] >= '0' && args[1][0] <= '9') {      /*Check if the argument specified is an actual number*/
			int i;
			for (i = 0; args[1][i] != '\0'; ++i) {              /*Implement atoi()*/
				if (args[1][i] >= '0' && args[1][i] <= '9') {
					count = count * 10 + args[1][i] - '0';
				}
				else {
					fprintf(stderr, "%s: invalid character found in argument\n", args[0]);
					fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
					return 1;
				}
			}
		}
		else {
			fprintf(stderr, "%s: invalid argument passed\n", args[0]);
			fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
			return 1;
		}
	}

	//if the integer argument is not specified, assign the default value of 2000
	if (nargs == 1) {
		count = 2000;
	}


	//create the process producer and consumer and put them in ready queue.
	//Look at the definations of function create and resume in exinu/system folder for reference.
	resume(create(producer, 1024, 20, "producer", 1, count));
	//resume(create(consumer, 1024, 20, "consumer", 1, count));
	printf("After create process call\n");
	getmemList();
	resume(icreate(producer, 1024, 20, "producer", 1, count));
	//resume(create(consumer, 1024, 20, "consumer", 1, count));
	printf("After icreate process call\n");
	getmemList();
	
	//receive();
	//semdelete(produced);
	//semdelete(consumed);
	printf("\n\n");
	return 0;
}
