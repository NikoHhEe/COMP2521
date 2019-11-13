//////////////////
// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

void cleanQueue(Queue q);

const char* __asan_default_options() { return "detect_leaks=0"; }

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

typedef struct QueueNode {
	Item value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head; // ptr to first node
	QueueNode *tail; // ptr to last node
} QueueRep;



// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}



// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc((size_t)nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc((size_t)nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// a method to store the path, by using the bfs, the max path would be sorte
// is 30, prevent some overhead, seted 31
// using the queue structure to record the path
Queue routeList[31];
// index currently occupied route
int occuRoute[31]={0};

int routeEmpty(){
    int i =0;
    for (i = 0; i < 31; i++) {
        /* serch in occuRoute array */
        if (occuRoute[i] == 0) {
            /* this route is empty */
            occuRoute[i] = 1;
            return i;
        }
    }
    return -1;
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
    assert(g != NULL);

    int visited[30];

    Queue q = newQueue();

    // initialization
    for (int i = 0; i <31; i++){
        routeList[i] = newQueue();
    }

    QueueJoin(routeList[0], src);
    QueueJoin(q, 0);
    occuRoute[0] = 1;
    // BFS
    while(!QueueIsEmpty(q)){
        int pathNum = QueueLeave(q);
        
        int i;
        for (i = 0; i < 30; i++) {
            if (visited[i] == 1) {
                continue;
            }

            if (g->edges[routeList[pathNum]->tail->value][i] <= max) {
                if (i == dest) {
                    // destination find
                    int j = 0;
                    QueueJoin(routeList[pathNum],i);
                    while (!QueueIsEmpty(routeList[pathNum])){
                        // add to path
                        path[j] = (int)QueueLeave(routeList[pathNum]);
                        j++;
                    }

                    return j;
                }

                int empty = routeEmpty();

                if (empty == -1) {
                    exit(1);
                }
                
                occuRoute[empty]= 1;

                // set visited
                visited[i] = 1;

                while (!QueueIsEmpty(routeList[empty])) {
                    // leave information in the queue
                    QueueLeave(routeList[empty]);
                }
                for (QueueNode *curr = routeList[pathNum]->head ; curr != NULL; curr = curr->next) {
                    // copy nodes to dest
                    QueueJoin(routeList[empty], curr->value);
                }

                // add the next vertex to this route
                QueueJoin(routeList[empty],i);
                // let this route to do BFS
                QueueJoin(q,empty);

            }
        }
        // clear the path
        occuRoute[pathNum]= 0;

    }

    // free Queues
    dropQueue(q);
    for (int i = 0; i < 31; i++) {
        dropQueue(routeList[i]);
    }

	return 0; 
}
