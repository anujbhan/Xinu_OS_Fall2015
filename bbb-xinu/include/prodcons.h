#include <stddef.h>
#include <kernel.h>   

   /*Global variable for producer consumer*/
   extern int n; /*this is just declaration*/
   extern pid32 prodconsPID; //Value will be used to store PID of prodcons process
   extern int childCount; //Keep count of child processes
   extern sid32 produced, consumed; /*Semaphore declaration of synchronization between producer consumer processes*/
   extern sid32 prodconWait;

  /*function Prototype*/
  void consumer(int count);
  void producer(int count);

