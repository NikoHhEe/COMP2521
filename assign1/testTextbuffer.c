// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textbuffer.h"


typedef struct newline *Line;
struct newline {
    struct newline *next;
    char *str;
};

struct textbuffer {
    int numLine;
    int size;
    struct newline *head;
    struct newline *tail;
    struct textbuffer *next;
};
static void testNewTB(void);

// TODO: Add more function prototypes


int main(void) {
	
	testNewTB();

    //Test Merge and Paste
	char *t1 = "Hi\nHow\nAre\nYou\n";
	TB tb = newTB(t1);
   

    //Test delete
    deleteTB(tb, 2,3);
    assert(linesTB(tb) == 6);
    printf("After delete:\n");
    printf("\n");

    //Test copy and cut
    printf("Before cut:\n");
    printf("\n");
    TB cutLine = cutTB (tb, 1, 2);
    printf("Cut lines:\n");
    printf("\n");
    assert(linesTB(tb) == 4);
    printf("After cut\n");
    printf("\n");
    releaseTB(cutLine);
    //release
	char *t3 = "Sun\nSunn\nSunnny\nSnun\n";
    TB tb3 = newTB (t3);
    
    releaseTB(tb3);
    
    // test addPrefixTB
    char *t4 = "Please\nadd\nprefixes\nto\nme\n";
	TB tb4 = newTB(t4);
    addPrefixTB(tb4, 3, 5, "prefix ");
    printf("\n");
    releaseTB(tb4);

    // test searchTB
    char *t5 = "abracadabra alacazam\n"
                "abracadabracadabracadabracadabra\n";
	TB tb5 = newTB(t5);
    Match mt = searchTB(tb5, "abracadabra");
    while (mt != NULL) {
        printf("line: %d, column: %d\n", mt->lineNumber, mt->columnNumber);
        mt = mt->next;
    }

    releaseTB(tb5);
    printf("\n");
	
    // test formRichText
    char *t6 = "abc*string*d\n"
                "*some string*lol*\n"
                "*hh *good*\n"
                "*some string\n"
                "hello ** world\n"
                "hello **world*\n"
                "hello **world**\n"
                "**hello***world**\n"
                "***hello*\n"
                "*abc**def**ghi**\n"
                "**hello**\n\n"

                "abc_string_d\n"
                "*some string_lol_\n"
                "_hh _good_\n"
                "_some string\n"
                "hello __ world\n"
                "hello __world_\n"
                "hello __world__\n"
                "__hello___world__\n"
                "___hello_\n"
                "_abc__def__ghi__\n"
                "__hello__\n\n"

                "*some_string*again_\n"
                "*some* _string_\n"
                "some *string_again_\n\n"
                
                "some#string*once_again*\n"
                "#string_stuff_\n"
                "#\n"
                "###\n"
                "*some_string_*\n"
                "#some _string_\n";
    TB tb6 = newTB(t6);
    printf("\n");
    formRichText(tb6);
    releaseTB(tb6);

	// TODO: Call more test functions
	
	printf("All tests passed! You are awesome!\n");
}

static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");

	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");

	assert(linesTB(tb1) == 4);

	char *text1 = dumpTB(tb1, false); // Don't show line numbers
	assert(strcmp("hello there,\nhow\nare\nthings\n", text1) == 0);
	free(text1);

	releaseTB(tb1);
	
	
	printf("newTB tests passed!\n");
}

// TODO: Add more test functions
