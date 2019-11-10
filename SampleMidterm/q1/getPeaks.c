// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

void addNode(DLList L, DLListNode *node) {
	assert (L != NULL);
	DLListNode *newNode = newDLListNode(node->value);
	
	if (DLListIsEmpty(L)) {
		L->curr = L->last = L->first = newNode;
	}
	
	else {
		newNode->prev = L->last;
		L->last->next = newNode;
		L->last = L->curr = newNode;
	}
	L->nitems++;
}

DLList getPeaks(DLList L){
	
	assert (L != NULL);
	DLList peakList = newDLList();
	
	if (DLListLength(L) <= 2) {
		return peakList;
	}
	
	DLListNode *check = L->first->next;

	while (check->next != NULL) {
		if ((check->value > check->prev->value) && (check->value > check->next->value)) {
			addNode(peakList, check);
		}
		check = check->next;
	}

	return peakList;

}



