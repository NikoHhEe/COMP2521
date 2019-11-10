// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

void test_i_a_d(DLList);
//void state(DLList);

int main (void)
{	
	// get a input text, show it and check the internal consistancy
	printf("Please input some text (end with Ctrl+D):\n");
	DLList myList = getDLList (stdin);
	printf("\nThis is the text you input:\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));
	//state(myList);

	// test DLListBefore, DLListAfter and DLListDelete when the 
	// text is more than one line
	test_i_a_d(myList);
	//state(myList);

	// test when the text is empty (empty list)
	// test DLListBefore function
	printf("\nThis is when the list is empty.\n");
	myList = newDLList();
	test_i_a_d(myList);
	//state(myList);

	// test when the text is has only one line (list with one node)
	// test DLListBefore function
	printf("\nThis is when the list has one node.\n");
	// first define a list with one node
	myList = newDLList();
	DLListBefore(myList, "one line node.");
	printf("This is the one line node:\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));
	test_i_a_d(myList);
	//state(myList);

	return EXIT_SUCCESS;
}

// a function which tests the DLListBefore, DLListAfter and 
// DLListDelete functions
void test_i_a_d(DLList myList) {
	// test the DLListBefore function
	DLListBefore(myList, "new line before");
	printf("This is the text after front insertion:\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));
	
	// test the DLListAfter function
	DLListAfter(myList, "new line after");
	printf("This is the text after end insertion:\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));

	// test the DLListDelete function
	DLListDelete(myList);
	printf("This is the text after deletion:\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));
	freeDLList (myList);
}

/*
void state(DLList L) {
	assert (DLListIsEmpty(L));

	printf("number of nodes: %zu\n", DLListLength(L));
	printf("current node: %s\n", L->curr->value);
	printf("first node: %s\n", L->first->value);
	printf("last node: %s\n", L->last->value);

}
*/
