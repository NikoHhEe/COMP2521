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
// is 30, prevent some overhead, seted 35
// using the queue structure to record the path
Queue route[35];
// index currently occupied route
int route_occupy[35]={0};

int routeEmpty(){
    int i =0;
    for (i = 0; i < 35; i++) {
        /* serch in route_occupy array */
        if (route_occupy[i] == 0) {
            /* this route is empty */
            route_occupy[i] = 1;
            return i;
        }
    }
    exit(1);
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
    // have to have these variables
    assert(g != NULL);

    int visited[30];

    // create a queue to do BFS
    Queue q = newQueue();

    // initial all the path
    for (int i = 0; i <35; i++){
        route[i] = newQueue();
    }
    if (src == dest ) {
        /* special case, not need to Search */
        path[0] = src;
        return 1;
    }
    // push the first route into queue
    QueueJoin(route[0],src);
    QueueJoin(q, 0);
    route_occupy[0] =1;
    // try to find a path by BFS
    while(!QueueIsEmpty(q)){
        int path_id = QueueLeave(q);
        
        int i;
        for (i = 0; i < 30; i++) {
            /* scan through the edge of this route */
            if (visited[i] == 1) {
                /* don't need to search this node */
                continue;
            }


            if (g->edges[route[path_id]->tail->value][i] <= max) {
                /* valid next vertex */
                if (i == dest) {
                    /* this vertex is the Destination */
                    // try to return
                    int k = 0;
 
                    QueueJoin(route[path_id],i);
                    while (!QueueIsEmpty(route[path_id])){
                        // append the record path into
                        path[k] = (int)QueueLeave(route[path_id]);
                        k++;
                    }

                    return k;
                }

                int new_empty = routeEmpty();
                
                // malloc this path
                route_occupy[new_empty]= 1;

                // set this place is visited to it wouldn't be touched
                visited[i] = 1;

                while (!QueueIsEmpty(route[new_empty])) {
                    /* leave all the information in the queue */
                    QueueLeave(route[new_empty]);
                }
                for (QueueNode *curr = route[path_id]->head ; curr != NULL; curr = curr->next) {
                    /* copy all the node to dest */
                    QueueJoin(route[new_empty], curr->value);
                }


                // add the next vertex to this route
                QueueJoin(route[new_empty],i);

                // addsign this route to do BFS
                QueueJoin(q,new_empty);

            }
        }
        // free this path
        route_occupy[path_id]= 0;

    }

    // free the memory of Queue
    dropQueue(q);
    for (int i = 0; i < 35; i++) {
        /* free all the route */
        dropQueue(route[i]);
    }
    
	return 0;
}
