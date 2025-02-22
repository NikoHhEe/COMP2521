
/* countEven.c 
   Written by Ashesh Mahidadia, October 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "BSTree.h"

/* 
    You will submit only this one file.

    Implement the function "countEven" below. Read the exam paper for 
    detailed specification and description of your task.  

    - You can add helper functions in this file, if required.  

    - DO NOT modify code in the file BSTree.h . 
    - DO NOT add "main" function in this file. 
*/


int countEven(BSTree t){

	// Implement this function, 
	// also change the return value below!
	if (t == NULL) {
		return 0;
	}
	else {
		if (t->key % 2 == 0) {
			return 1 + countEven(t->left) + countEven(t->right);
		}
		else {
			return countEven(t->left) + countEven(t->right);
		}
	}

}



