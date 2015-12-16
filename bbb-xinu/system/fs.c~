#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#if FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];

int next_open_fd = 0;

int dataMask = 11;



#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2
int inodeNum = 0;
int setMask = FIRST_INODE_BLOCK;

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */


int fs_create(char *filename, int mode){
 
   
   /* check whether the maximum entries are reached */
   if (next_open_fd < 16) {
      
      /* debug */ printf ("next_open_fd : %d\n", next_open_fd);
      
      
      /* create directory and file association entry */
      oft[next_open_fd].de = (struct dirent *)getmem(sizeof(struct dirent));
      oft[next_open_fd].de->inode_num = inodeNum;
      strcpy(oft[next_open_fd].de->name, filename);
      
      /* initiate inode to accomodate the file */
      
      oft[next_open_fd].in.id = inodeNum;
      oft[next_open_fd].in.type = INODE_TYPE_FILE;
      oft[next_open_fd].in.device = dev0;
      oft[next_open_fd].in.size = 0;
  
      /* update the directory */
      
      fsd.root_dir.numentries = inodeNum + 1; //The inode number starts with 0
      fsd.root_dir.entry[inodeNum+1] = *oft[next_open_fd].de;
      
      if ((inodeNum % 8 == 0) && (inodeNum != 0)) {
	
        fs_setmaskbit(setMask);
        setMask++; 
      }
      
      /* put the inode in memory */
      if(fs_put_inode_by_num(dev0, inodeNum, &oft[next_open_fd].in) == 1) {
 	      inodeNum++;
              return next_open_fd++;
      }
           
   } 
   return SYSERR;
}

int fs_open(char *filename, int flags){
   
   /* search the existing file */
   int searchIndex = 0;
   for (searchIndex = 0; searchIndex < NUM_FD; searchIndex++) {
      
      if(strcmp(oft[searchIndex].de->name, filename) == 0) { /* <--- if found */
         oft[searchIndex].state = FSTATE_OPEN; 
         oft[searchIndex].flag = flags;
         return searchIndex;
      } 
   }  
   printf("filename doesnot exists");
   return SYSERR;
              
}

int fs_write(int fd, void *buf, int nbytes) {
	
	int numBlocks = nbytes/512;
        int dataMask = 17;
        int offset = 0;
	if (oft[fd].flag == O_RDONLY)
		return SYSERR;
	while(fs_getmaskbit(dataMask) != 0) {
		dataMask++;			
	}
        
	int blockIndex = 0;
        int blockToWrite = 0;
        if (oft[fd].in.size <= 512) {
		blockIndex = 0;
        } else {
		blockIndex = (oft[fd].in.size/512);
                
        }
	int loopIndex = 0;
	int bytesToWrite;
	while (nbytes > 0) {
	if(loopIndex > 0) {
		buf = buf + bytesToWrite;
	}
	if ((oft[fd].fileptr % 512) == 0 ) {
		blockToWrite = dataMask;
		fs_setmaskbit(blockToWrite);
		dataMask++;	
	} else {
		blockToWrite = oft[fd].in.blocks[blockIndex];	
	}
	if (blockIndex == 0) {
		bytesToWrite = 512 - oft[fd].fileptr;
                offset = oft[fd].fileptr;
                //printf("loop : %d, bytes to write: %d offset: %d\n", loopIndex, bytesToWrite, offset);
		
                if (nbytes < bytesToWrite) {
			bytesToWrite = nbytes;		
		}	
	} else {
		bytesToWrite = ((blockIndex+1)*512) - oft[fd].fileptr;
		offset = ((blockIndex)*512) - bytesToWrite;
                if (loopIndex > 0) {
			offset = 0;		
		}
		if (nbytes < bytesToWrite) {
			bytesToWrite = nbytes;		
		}
 		//printf("loop : %d, bytes to write: %d offset: %d\n", loopIndex, bytesToWrite, offset);	
	}

	if (bs_bwrite(dev0, blockToWrite, offset, buf, bytesToWrite) == 1) {
		//printf("write successful");
			
	} else {
		//printf("write failed");
		return SYSERR;	
	}
		nbytes = nbytes - bytesToWrite;
                oft[fd].fileptr = oft[fd].fileptr + bytesToWrite;
        	blockIndex++;
        	oft[fd].in.blocks[blockIndex] = dataMask;
                
        }	
	oft[fd].in.size = oft[fd].fileptr;
        return oft[fd].fileptr;
		
}

int fs_read(int fd, void *buf, int nbytes)
{    
    int inode,k,blocks_to_read,n_blk;
    struct inode in;
    char data[MDEV_BLOCK_SIZE];
    char buf1[12];
    int fptr,j,i,counter;
    inode = oft[fd].de->inode_num;
    fs_get_inode_by_num(0, inode, &in);
    fptr = oft[fd].fileptr;
    j = 0;
    counter = nbytes;
    n_blk = nbytes/512;
    printf("Num Blocks %d",n_blk);
    for(i=0;i<=n_blk;i++)
        {
            if (nbytes < MDEV_BLOCK_SIZE)
                blocks_to_read = nbytes;
	    else
                blocks_to_read = MDEV_BLOCK_SIZE;
	    //printf("Reading : %dbytes",blocks_to_read);
	    bs_bread(0, oft[fd].in.blocks[i], 0, data,blocks_to_read);
            strcpy(buf,data);
            nbytes -= blocks_to_read;
            if(nbytes==0)
            	break;
        }
    fptr = oft[fd].fileptr + counter;
    return fptr;

}
int fs_close(int fd) {
	
	if(fd > 16){
		oft[fd].state = FSTATE_CLOSED;
		return OK;	
	} else {
		return SYSERR;	
	}	
}

int fs_seek(int fd, int offset)
{ 
	int fptr = oft[fd].fileptr + offset;
	return fptr;
}

int fs_mount(int dev) {
     /* read the fsystem block to SB_BLK, from secondary */
  
  bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* read the fsystem block to BM_BLK, from secondary */
  
  bs_bread(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);  
}

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

#endif /* FS */

