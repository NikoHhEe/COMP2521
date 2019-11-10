#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "textbuffer.h"

// structure designed for visiting all the lines in the textbuffer
typedef struct textline *TextLine;
struct textline {
    char *currLine;
    struct textline *next;
};

struct textbuffer {
    int numLine;
    int size;
    struct textline *head;
    struct textline *tail;
    struct textbuffer *next;
};

// copied from C sourse code from Internet
static char *strdup(const char *s)
{
  size_t len = strlen(s) + 1;
  char *result = (char*) malloc(len);
  if (result == (char*) 0)
    return (char*) 0;
  return (char*) memcpy(result, s, len);
}

// append new textline to the textbuffer
static TB appendText(TB tb, TextLine text){
    assert(tb != NULL);
    if(tb->head == NULL){
        tb->tail = tb->head = text;
    } else {
        tb->tail->next = text;
        tb->tail = text;
    }
    return tb;
}

// make new textLine
static TextLine newTextline(char *str){
    //Create the new line
    TextLine newLine = calloc(1, sizeof(struct textline));
    newLine->currLine = strdup(str);
    newLine->next = NULL;
    return newLine;
}

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB (char text[]){
    if(text == NULL){
        abort();
    }

    TB tb = calloc(1, sizeof(struct textbuffer));
    assert(tb != NULL);
    char ch = text[0];
    int i, j;
    int prev = 0;
    int textline = 0;
    tb->head = NULL;
    tb->tail = NULL;
    tb->size = strlen(text);
    // count number of lines
    for(i = 0; i < tb->size; i++){
        ch = text[i];
        if(ch == '\n'){
            textline++;
        }
    }
    tb->numLine = textline;
    i = 0;
    // save strings of each line to a string variable
    while(i < tb->size + 1){
        ch = text[i];
        if(ch == '\0'){
            break;
        }if(ch == '\n'){
            // count the allocation size
            int n = i - prev + 1;
            char *string = calloc(1, sizeof(char)*(n+1));
            j = 0;
            //Copy the string before '\n';
            while(j < n){
                string[j] = text[prev+j];
                j++;
            }
            //Create new line and Append the line into tb
            string[j] = '\0';
            TextLine text = newTextline(string);
            tb = appendText(tb,text);
            free(string);
            prev = i+1;
        }
        i++;
    }
    return tb;
}

// free the TextLine
static void freeTextLine(TextLine text){
    //free text
    free(text -> currLine);
    free(text);
}

// free the textLines in textbuffer
static TextLine freeLine(TB tb, TextLine prev, TextLine curr, int num){
    assert(tb != NULL);
    TextLine rest = NULL;
    TextLine curr1 = curr;
    int i = 0;
    
    while (i < num && curr != NULL) {
        rest = curr->next;
        freeTextLine(curr);
        curr = rest;
        i++;
    }

    if(curr1 == tb->head){
        if(num != tb->numLine){
            return rest;
        }else{
            return NULL;
        }
    }else{
        prev->next = rest;
        return tb->head;
    }

}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
	assert(tb != NULL);
    if(tb == NULL){
        return;
    }else{
        TextLine curr = tb->head;
        freeLine(tb, NULL,curr, tb->numLine);
        free(tb);
    }
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB (TB tb, bool showLineNumbers) {
	assert(tb != NULL);
    if (tb == NULL)
        return NULL;
    if (tb->head == NULL)
        return NULL;

    int length = 1;

    TextLine curr;
    
    for (curr = tb->head; curr != NULL; curr = curr->next) {
        char *s = curr->currLine;
        while(*s != '\0') {
            s++;
        }
        length += strlen (curr->currLine);
    }
    //Copy every substring of tb into str
    size_t lenPre = 3 + floor(log10(linesTB(tb)));
    if (showLineNumbers) {  
        length += lenPre;
    }
    char *string = malloc (length * sizeof(char));
    string[0] = '\0';

    curr = tb->head;
    int i = 1;
    for (curr = tb->head; curr != NULL; curr = curr->next) {
        if (showLineNumbers) {
            char *temp = malloc(lenPre * sizeof(char));
            sprintf(temp, "%d", i++);
            strcat(string, temp);
            strcat(string, ". ");
            free(temp);
        }
        strcat(string, curr->currLine);
    }

    return string;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb) {
	assert(tb != NULL);

    return tb->numLine;
}

// prepend s2 to the start of s1
static char *prepend(char *s1, char *s2)
{
	int len1 = 0, len2 = 0, j = 0, k = 0;
	char s4[30];
	if (s1 == NULL)
		return NULL;
	if (s2 == NULL)
		return s1;
	len1 = strlen(s1);
	len2 = strlen(s2);

	for (int i = 0; i < len1; i++)
	{ 
		s4[k++] = s1[i];
	}
 
	for (int i = 0; i < len2; i++)
		s1[j++] = s2[i];

	for(int i = 0; i < len1; i++)
		s1[j++] = s4[i];
 
	s1[j] = '\0';
 
	return s1;
}


/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {
    assert (tb != NULL);
    int lineDiff = to - from + 1;
    TextLine curr = newTextline(tb->head->currLine);
    curr = tb->head;
    while (--from) {
        curr = curr->next;
    } 

    while (lineDiff-- && curr != NULL) {
        curr->currLine = prepend(curr->currLine, prefix);
        curr = curr->next;
    }
    free(curr);
}

// check the range of inout position
void range(TB tb, int pos1, int pos2){
    //Boundary check
    assert(tb != NULL);
    if(pos1 < 0 || pos2 < 0 ||
        pos1 > tb->numLine-1 || pos2 > tb->numLine-1){
        printf("Out of Range\n");
        abort();
    }
}

// look for the textLine with the given line number
static TextLine FindLine(TB tb, int num){
    assert(tb != NULL);
    TextLine curr = tb->head;
    if(num == 1){
        return curr;
    }
    int flag = 0;
    //Find the postion which is represented by the number in the tb
    while(curr != NULL && flag != num){
        curr = curr->next;
        flag++;
    }
    return curr;
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
TB copyTB (TB tb, int from, int to){
    assert(tb != NULL);
    
    char *copyText = dumpTB (tb, 0);
    TB  copyTB = newTB(copyText);
    int last = copyTB->numLine - 1;

    if(to == 0 && from == 0){
        deleteTB (copyTB, from+1, last);
    }else if(from == 0 && to == 0 && last == 0){
        return copyTB;
    }else if(from == 0 && to != last){
        deleteTB (copyTB, to+1, last);
    } else if(from == 0 && to == last){

    } else{
        deleteTB (copyTB, 0, from - 1);
        deleteTB (copyTB, to, last-1);
    }
    free(copyText);
    return copyTB;
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
	assert(tb != NULL);
    //Boundary check
    if(tb->head == NULL){
        return NULL;
    }
    range(tb,from,to);
    if(to < from){
        return NULL;
    }
   //Cut the line in the tb from the number of from to the number of to
    TB cutLine = copyTB(tb,from,to);
   //Delete the lines which are cut from the tb
    deleteTB(tb, from, to);
    return cutLine;
    return NULL;
}

// mergeTB
void mergeTB (TB tb1, int pos, TB tb2) {
    assert(tb1 != NULL);
    assert(tb2 != NULL);
    //Merge tb2 to tb1
    pasteTB (tb1, pos, tb2);
    //free tb2
    releaseTB(tb2);
}

// pasteTB
void pasteTB (TB tb1, int pos, TB tb2) {
    //
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB (TB tb, int from, int to) {
    assert(tb != NULL);
    //Boundary check
    range(tb,from,to);
    TextLine curr = FindLine(tb, from);
    TextLine prev = NULL;
    if(from != 1){
        //find the previous line of curr
        prev = FindLine(tb, from - 1);
    }
    //delete the line from the num of from to the num of to
    TextLine head = freeLine(tb, prev, curr, to-from);
    //Update the head of tb and number of lines of tb
    tb->head = head;
    tb->numLine = tb->numLine - (to-from+1);
}

// create a new Match 
static Match newMatch(void) {
    Match matchList = malloc(sizeof(matchNode));
	matchList = NULL;

    return matchList;
}

// add new values to Match
static Match appendMatch(Match mt, int line, int column) {
   
    Match new = malloc(sizeof(matchNode));
    new->lineNumber = line;
    new->columnNumber = column;
    new->next = NULL;
    
    if (mt == NULL)    {
        mt = new;
        new->next = NULL;
    }
    else {
        Match temp = mt;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new;
        new->next = NULL;
    }
    return mt;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
	assert(tb != NULL);

    if (search == NULL) {
        return NULL;
    }

    int lineN, columnN, targetLine, targetColumn;
    int matchCount = 0;
    int lengthSearch = strlen(search); 

    TextLine curr = newTextline(tb->head->currLine);
    
    Match matchList = newMatch();

    for (curr = tb->head, lineN = 1; curr != NULL; curr = curr->next, lineN++) {
        char *currLine = curr->currLine;
        int length = strlen(currLine);
        int searchIndex = 0;

        for (columnN = 1; columnN < length; columnN++, searchIndex++) {
            if ( *(currLine + columnN - 1) == *(search+searchIndex)) {
                if (matchCount == 0) {
                    targetLine = lineN;
                    targetColumn = columnN;
                }
                matchCount++;
            }
            else {
                searchIndex = -1;
                matchCount = 0;
            }
            if (matchCount == lengthSearch) {
                matchList = appendMatch(matchList, targetLine, targetColumn);
            }            
        }
    }
    return matchList;
    return NULL;
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {
    if (tb == NULL) {
        abort();
    }

    TextLine curr = newTextline(tb->head->currLine);
    curr = tb->head;

    while (curr != NULL) {
        //int scopeStar = 0, scopeUnder = 0, scopeNum = 0;
        int flagStar = 0, flagUnder = 0;
        int columnStarFrom, columnStarTo;
        int columnUnderFrom, columnUnderTo;
        char *currLine = curr->currLine;
        int length = strlen(currLine);

        for (int columnN = 0; columnN < length; columnN++) {
            currLine = curr->currLine;
            char currChar = *(currLine + columnN);

            // process *string*
            if (currChar == '*') {
                if (flagStar == 2) {
                    flagUnder = 0;
                    int lengthText = columnStarTo - columnStarFrom - 1;
                    if (lengthText == 0) {
                        columnN--;
                        flagStar = 0;
                        continue;
                    }
                    else {
                        char *richText = calloc(1, (length + 5) * sizeof (char));
                        int i;

                        for (i = 0; i < columnStarFrom; i++) {
                            richText[i] = *(currLine + i);
                        }
                        richText[i++] = '<';
                        richText[i++] = 'b';
                        richText[i++] = '>';

                        for (int j = columnStarFrom + 1; j < columnStarTo; j++) {
                            richText[i++] = *(currLine + j);
                        }

                        richText[i++] = '<';
                        richText[i++] = '/';
                        richText[i++] = 'b';
                        richText[i++] = '>';

                        for (int k = columnStarTo + 1; k < length; k++) {
                            richText[i++] = *(currLine + k);
                        } 
                        curr->currLine = strdup(richText);
                        free(richText);
                        length += 5;
                        flagStar = 0;
                    }
                }
                else if (flagStar == 1) {
                    flagStar++;
                    columnStarTo = columnN--;
                }
                else {
                    flagStar++;
                    columnStarFrom = columnN;
                }   
                
            }

            // process _string_
            if (currChar == '_') {
                //scopeUnder = 1;
                if (flagUnder == 2) {
                    flagStar = 0;
                    int lengthText = columnUnderTo - columnUnderFrom - 1;
                    if (lengthText == 0) {
                        columnN--;
                        flagUnder = 0;
                        continue;
                    }
                    else {
                        char *richText = calloc(1, (length + 5) * sizeof (char));
                        int i;

                        for (i = 0; i < columnUnderFrom; i++) {
                            richText[i] = *(currLine + i);
                        }
                        richText[i++] = '<';
                        richText[i++] = 'i';
                        richText[i++] = '>';

                        for (int j = columnUnderFrom + 1; j < columnUnderTo; j++) {
                            richText[i++] = *(currLine + j);
                        }

                        richText[i++] = '<';
                        richText[i++] = '/';
                        richText[i++] = 'i';
                        richText[i++] = '>';

                        for (int k = columnUnderTo + 1; k < length; k++) {
                            richText[i++] = *(currLine + k);
                        } 
                        curr->currLine = strdup(richText);
                        free(richText);
                        length += 5;
                        flagUnder = 0;
                    }
                }
                else if (flagUnder == 1) {
                    flagUnder++;
                    columnUnderTo = columnN--;
                }
                else {
                    flagUnder++;
                    columnUnderFrom = columnN;
                }   
                
            }

            // process #string
            if (currChar == '#' && columnN == 0 && length > 2) {
    
                char *richText = calloc(1, (length + 7) * sizeof (char));
                int i = 0;

                richText[i++] = '<';
                richText[i++] = 'h';
                richText[i++] = '1';
                richText[i++] = '>';

                for (int j = 1; j < length - 1; j++) {
                    richText[i++] = *(currLine + j);
                }

                richText[i++] = '<';
                richText[i++] = '/';
                richText[i++] = 'h';
                richText[i++] = '1';
                richText[i++] = '>';
                richText[i++] = '\n';

                curr->currLine = strdup(richText);
                free(richText);
                length += 7;

                break;
        
            }
            
        }
        curr = curr ->next;
    }
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
