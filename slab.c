/* 
CPSC/ECE 3220, Spring 2021
Roderick "Rance" White
Program 4

The following program allocates memory from a slab and releases it.
*/



#include "slab.h"

int FindFirstObject(int CountMax, int *SlabIndex);


unsigned char *slab_allocate()
{
	int i = 0, j = 0;

	/* Return NULL if all masks are full (meaning there are no slabs left) */
	if(full_mask == 0xffff)
		return NULL;

	/* If there are any partial slabs left, allocate from the first partially empty slab */
	if(partial_mask != 0x0000) {
		j = FindFirstObject(14, &i);
		if(j==-1) return NULL;							//Return NULL if search failed.

		/* Update the slab's mask to be empty when the count is going from 1 to 0 */
		if(s[i].free_count == 1) {
			partial_mask=partial_mask & ~(1<<(15-i));	//Subtract bit from partial mask
			full_mask=full_mask | (1<<(15-i));			//Add bit to full mask
		}
	}

	/* If there are no partial slabs but there are empty slabs, find first empty slab */
	else if(empty_mask != 0x0000) {
		j = FindFirstObject(15, &i);
		if(j==-1) return NULL;							//Return NULL if search failed.

		/* Change the current slab's bit masks as it is no longer completely empty */
		empty_mask=empty_mask & ~(1<<(15-i));			//Subtract bit from empty mask
		partial_mask=partial_mask | (1<<(15-i));		//Add bit to partial mask
	}

	s[i].free_count -=1;								//Remove one free count from slab

	return start + 4096*i + 256*(j+1);
}


int slab_release( unsigned char *addr )
{
	int i,j;
	short unsigned int newmask;

	/* Look if the address is in the range of addresses */
	for(i=0; i<16; i++)
	{
		/* If the address is in the range for the slab, find the specific address */
		if((start + 4096*i) <= addr && addr < (start + 4096*(i+1)))
		{
			/* Check signature. If invalid, then the release is a failure. */
			if(s[i].signature != 0x51ab51ab) return 1;
			/* Loop to check for address location */
			for(j=0; j<16; j++)
			{
				/* Break if it is the specific address */
				if((start + 4096*i + 256*j) <= addr && addr < (start + 4096*i + 256*(j+1)))
				{
					newmask = s[i].free_mask | (1<<(15-j));			//Add to address to compare
					/* Return 2 if the address is already present. */
					if(newmask == s[i].free_mask) return 2;

					/* Otherwise, release the address back to the slab */
					s[i].free_count +=1;							//Add one free count to slab
					s[i].free_mask = newmask;

					//Update the slab's mask to be empty when the count changes from 0 to one
					if(s[i].free_count == 1) {
						partial_mask=partial_mask | (1<<(15-i));	//Add bit to partial mask
						full_mask=full_mask & ~(1<<(15-i));			//Subtract bit from full mask
					}
					//Change the current slab's bit masks as it is now completely empty
					else if(s[i].free_count == 15) {
						empty_mask=empty_mask | (1<<(15-i));		//Add bit to empty mask
						partial_mask=partial_mask & ~(1<<(15-i));	//Subtract bit from partial mask
					}
					return 0;
				}

			}
		}
	}
	return 1;														//Returns 1 if not found in range
}






/* Function to find the indexes for the slab and the the object of the slab */
int FindFirstObject(int CountMax, int *SlabIndex)
{
	int SlabInd = 0, ObjInd = 0;

	//Loop to find the next non empty slab
	for(SlabInd=0; SlabInd<16; SlabInd++){
		/* Find the index of the slab with a free count between the bounds */
		// Count will be between 1 & 14 for partial fill and between 1 & 15 for empty slab
		if(0 < s[SlabInd].free_count && s[SlabInd].free_count <= CountMax) 
			if(s[SlabInd].signature == 0x51ab51ab)					//Signature check
				break;
	}

	*SlabIndex = SlabInd;					//Slab index value to be returned to the function

	//If the entire search has failed, then return indicator of failure
	if(SlabInd == 16) return -1;

	/* Find the index of the first available object */
	for(ObjInd=0; ObjInd<15; ObjInd++){
		/* The first 1 bit encountered is the first available object */
		// The bit indexing isolates a single bit from the mask to check its value
		if((((s[SlabInd].free_mask) & (1<<(14-ObjInd))) >> (14-ObjInd)) == 1) break;
	}

	/* Remove the bit for the corresponding object from the slab's free mask */
	s[SlabInd].free_mask=s[SlabInd].free_mask & ~(1<<(14-ObjInd));	//Subtract bit from free mask

	return ObjInd;
}












