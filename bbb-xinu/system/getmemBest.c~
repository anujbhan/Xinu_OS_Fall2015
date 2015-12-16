/* getmemBest.c - getmemBest */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address using Best Fit Strategy
 *------------------------------------------------------------------------
 */
char  	*getmemBest(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	struct  memblk  *minBlock, *prevmin; /* Record for Best Fit */

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	uint32 minDiff = 0;
	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else if (curr->mlength > nbytes && ((curr->mlength - nbytes) < minDiff) || minDiff == 0) { /* check for minDiff	*/

			// store pointer of prev block to minDiff block
			prevmin = prev;

			// store pointer of block with minDiff found so far
			minBlock = curr;

			// update minDiff 
			minDiff = curr->mlength - nbytes;
			
		} //else {			/* Move to next block	*/
		prev = curr;
		curr = curr->mnext;
		//}
	}
	if (minDiff != 0){
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
	} else {
	// nothing found
		restore(mask);
		return (char *)SYSERR;
	}
}
