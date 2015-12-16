/* getstkBest.c - getstkBest */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getstk  -  Allocate stack memory, returning highest word address using Best fit
 *------------------------------------------------------------------------
 */
char  	*getstkBest(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;	/* Walk through memory list	*/
	struct	memblk	*leftover; /* Record block that fits	*/
	struct  memblk  *minBlock, *prevmin; /* Record for Best Fit */
	uint32 minDiff = 0;
	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	//fits = NULL;
	//fitsprev = NULL;  /* Just to avoid a compiler warning */

	while (curr != NULL) {			/* Scan entire list	*/
		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else if (curr->mlength >= nbytes && (minDiff == 0 || (curr->mlength - nbytes) < minDiff)) {	/* Record block address	*/
			
			minBlock = curr;
			prevmin = prev;
			minDiff = curr->mlength - nbytes;
		} //else 
		prev = curr;
		curr = curr->mnext;
	}

	if (minDiff == 0) {			/* No block was found	*/
		restore(mask);
		return (char *)SYSERR;
	}
	// fill in for left over
	leftover = (struct memblk *)((uint32) minBlock +
				nbytes);
	// prev block to minDiff block, points to leftover
	prevmin->mnext = leftover;
	leftover->mnext = minBlock->mnext;
	leftover->mlength = minBlock->mlength - nbytes;
	memlist.mlength -= nbytes;
	restore(mask);
	return (char *)(minBlock);
}
