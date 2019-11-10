// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders
// <vince@kyllikki.org>

#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graph.h"
#include "html.h"
#include "set.h"
#include "queue.h"
#include "url_file.h"

#define BUFSIZE 1024

#define strEQ(g, t) (strcmp ((g), (t)) == 0)

typedef unsigned char Num;

typedef struct GraphRep {
	size_t nV;
	size_t maxV;
	char **vertex;
	Num **edges;
} GraphRep;

static void setFirstURL (char *, char *);

static ssize_t vertexID (char *str, char **names, size_t N)
{
	for (size_t i = 0; i < N; i++) {
		if (strEQ (str, names[i]))
			return (ssize_t) i;
	}
		
	return -1;
}

static size_t addVertex (char *str, char **names, size_t N)
{
	names[N] = str;
	return N;
}


int main (int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char *next;
	int maxURLs;

	if (argc > 2) {
		strcpy (baseURL, argv[1]);
		setFirstURL (baseURL, firstURL);
		maxURLs = atoi (argv[2]);
		printf("max: %d\n", maxURLs);
		if (maxURLs < 40)
			maxURLs = 40;
	} else {
		fprintf (stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit (1);
	}

	Queue ToDo = newQueue();
	enterQueue(ToDo, firstURL);
	Graph URLGraph = newGraph((size_t)maxURLs);
	addVertex(firstURL, URLGraph->vertex, URLGraph->nV);
	
	while (!emptyQueue(ToDo) && URLGraph->nV <= maxURLs) {
		printf("URLGraph->nV: %zu\n", URLGraph->nV);
		next = leaveQueue(ToDo);
		if (!(handle = url_fopen(next, "r"))) {
			fprintf (stderr, "Couldn't open %s\n", next);
			exit(1);
		}

		if (!strstr(next,"unsw.edu.au")) { continue; }

		while (!url_feof (handle)) {
			url_fgets (buffer, sizeof (buffer), handle);
			int pos = 0;
			char result[BUFSIZE];
			memset (result, 0, BUFSIZE);
			while ((pos = GetNextURL (buffer, firstURL, result, pos)) > 0) {
				
				if (vertexID(result, URLGraph->vertex, URLGraph->nV && NormalizeURL(result))) {
                    enterQueue(ToDo, result);
                    printf("Found: '%s'\n",result);
                }
				addEdge(URLGraph, next, result);
				memset(result,0,BUFSIZE);
			}
		}
		url_fclose(handle);
		free(next);
	    sleep(1);
	}
	printf("\n");
	showGraph(URLGraph,0);
	printf("\n");
	showGraph(URLGraph,1);
	dropGraph(URLGraph);
	
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
static void setFirstURL (char *base, char *first)
{
	char *c;
	if ((c = strstr (base, "/index.html")) != NULL) {
		strcpy (first, base);
		*c = '\0';
	} else if (base[strlen (base) - 1] == '/') {
		strcpy (first, base);
		strcat (first, "index.html");
		base[strlen (base) - 1] = '\0';
	} else {
		strcpy (first, base);
		strcat (first, "/index.html");
	}
}
