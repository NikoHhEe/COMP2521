#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "textbuffer.h"

typedef struct textLine {
	char * currLine;
	struct textLine * nextLine;
} TextLine;

typedef struct textbuffer {
	int numLine;
    int size;
    TextLine * first;
    TextLine * final;
	struct textbuffer * next;
} TextBuffer;

static int numLine(char *text) {
	int numLine = 0;
	int length = strlen(text);
	int charIndex;
	for (charIndex = 0; charIndex < length; charIndex++) {
		char curr = *(text + charIndex);
		if (curr == '\n') numLine++; 
	}
	return numLine;
}

static TB appendLine(TB tb, TextLine *text) {
	
	assert(tb != NULL);
    
	//if first is empty, add the line in the first place of tb
    if(tb->first == NULL){
        tb->first = text;
        tb->final = text;
    } else {
        //if head is not NULL, append new line to the tail of tb
        tb->final->nextLine = text;
        tb->final = text;
    }
    return tb;
}

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB (char *text) {
	int lines = numLine(text);
	TB newTextbuffer = malloc(sizeof (TextBuffer));
	assert(newTextbuffer != NULL);
	int pre = 0;
	int i = 0;
	newTextbuffer->numLine = lines;
	newTextbuffer->first = NULL;
	newTextbuffer->final = NULL;
	newTextbuffer->size = strlen(text);
	int charIndex = 0;
	
	while (charIndex < newTextbuffer->size + 1) {
		// track the chars in the text
		char curr = *(text + charIndex);
		/*// make the curr char a length one string to use strcat
		char currStr[1];
		currStr[0] = curr;

		char *tempBuffer = malloc(100 * sizeof (char));*/
		// check whether reach the end of text
		if (curr != '\0') {
			// if not reach \n save the line of text at tempBuffer
			if (curr == '\n') {
				int numWords = i - pre + 1;
				char * tempBuffer = malloc(numWords * sizeof(char));
				int j = 0;
				while (j < numWords) {
					tempBuffer[j] = *(text + pre + j);
					j++;
				}
				tempBuffer[j] = '\0';
				TextLine *textline = malloc(sizeof (struct textLine));
				textline->currLine = tempBuffer;
				textline->nextLine = NULL;
				newTextbuffer = appendLine(newTextbuffer, textline);
				free(tempBuffer);
				pre = i + 1;
			}
			i++;
		}
		else {
			break;
		}	
	}
	return newTextbuffer;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {

}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB (TB tb, bool showLineNumbers) {
	return NULL;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb) {

	return -1;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {

}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB (TB tb1, int pos, TB tb2) {

}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {

}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB (TB tb, int from, int to) {
	return NULL;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
	return NULL;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB (TB tb, int from, int to) {

}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB (TB tb1, TB tb2) {
	return NULL;
}

void undoTB (TB tb) {

}

void redoTB (TB tb) {

}
