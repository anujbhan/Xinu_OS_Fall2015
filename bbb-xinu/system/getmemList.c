#include <xinu.h>

/*--------------------------------------------------------------------------------------------------
 *  getmemList  -  Prints a list of all available memory functions with their respective addresses
 *--------------------------------------------------------------------------------------------------
 */
int getmemList( )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;

	mask = disable();
	prev = &memlist;
	curr = memlist.mnext;
        if(curr == NULL) {
               printf("Out of memory");
               return -1;
        }
	while (curr != NULL) {			/* Search free list	*/

		        printf("The address of current memory block is %u with length of %u\n", curr, curr->mlength);
                        curr = curr->mnext;

	}
	restore(mask);
	return 0;
}