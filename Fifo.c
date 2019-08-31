// Fifo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: November 14, 2018
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
// --UUU-- Declare state variables for Fifo
//        buffer, put and get indexes
#define SIZE 9
int PutI; 
int GetI;
int count;
char FIFO[SIZE];

// *********** Fifo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init(){
// --UUU--Complete this
	PutI = 0;		// set start indexes to 0
	GetI = 0; 
//	count = 0;
}

// *********** Fifo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){

	 if(((PutI +1) %SIZE)== GetI){		// fail if next value of putI is at getI because buffer is full
		return 0;
	}
	FIFO[PutI] = data;					// otherwise, put data into buffer
	PutI = (PutI +1)%SIZE;			// increment putI
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){ 

if (GetI == PutI)		// if buffer is empty
	{
		return 0; 
	}
	*datapt = FIFO[GetI];		// otherwise, load value at getI into parameter variable
		GetI = (GetI + 1)%SIZE;	// increment getI
		return 1; 	
}


