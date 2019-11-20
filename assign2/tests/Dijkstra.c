#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "PQ.h"
#include "Dijkstra.h"
#include "Graph.h"

#define INFINITY 9999999

static ItemPQ newItemPQ(int key, int value);
static void updatePred(PredNode **list, int v, int w);
static void updateDeletePred(PredNode **list, int v, int w);

ShortestPaths dijkstra(Graph g, Vertex src) {
	//initialize ShortestPaths
	ShortestPaths path;
	path.dist = malloc(sizeof(int) * (GraphNumVertices(g)));
	assert((path.dist) != NULL);
	path.pred = malloc(sizeof(struct PredNode) * (GraphNumVertices(g)));
	assert((path.pred) != NULL);
	path.src = src;
	path.numNodes = GraphNumVertices(g);
	
	//initialize dist
	PQ q = PQNew();

	// initialize priority queue
	for (int i = 0; i < path.numNodes; i++) {
		path.dist[i] = INFINITY;
		path.pred[i] = NULL;
		ItemPQ newItem = newItemPQ(i, path.dist[i]);
		PQAdd(q, newItem);
	}

	path.dist[src] = 0;

	ItemPQ next = newItemPQ(src, path.dist[src]);
	PQUpdate(q, next);
	while (!PQIsEmpty(q)) {
		ItemPQ item = PQDequeue(q);
		Vertex s = item.key;
		AdjList outNode = GraphOutIncident(g, s);
		AdjList curr = outNode;
		while (curr != NULL) {
			int cost = curr->weight;
			// if find a prev node with the same distance to the src
			if ((path.dist[s] + cost) == (path.dist[curr->v])) {
				//update dist
				path.dist[curr->v] = (path.dist[s]) + cost;
				//update queue
				ItemPQ newDist = newItemPQ(curr->v, path.dist[curr->v]);
				PQUpdate(q, newDist);
				// update pred by adding the node to pred 
				updatePred(path.pred, s, curr->v);
			}
			// if find a prev node with less distance to the src
			if ((path.dist[s] + cost) < (path.dist[curr->v])) {
				//update dist
				path.dist[curr->v] = (path.dist[s]) + cost;
				//update queue
				ItemPQ newDist = newItemPQ(curr->v, path.dist[curr->v]);
				PQUpdate(q, newDist);
				//update pred by deleting the old nodes and add new pred node
				updateDeletePred(path.pred, s, curr->v);
			}
			curr = curr->next;
		}
	}

	// change all the unreachable distances from infinity to 0
	for (int j = 0; j < path.numNodes; j++) {
		if (path.dist[j] == INFINITY) {
			path.dist[j] = 0;
		}
	} 

	PQFree(q);
	return path;
}

// show the information of all the nodes as src
void  showShortestPaths(ShortestPaths sps) {
	printf("Node %d\n",sps.src);
	printf("  Distance\n");
	for (int i = 0; i < sps.numNodes; i++) {
		if(i == sps.src)
	    	printf("    %d : X\n", i);
		else
			printf("    %d : %d\n", i, sps.dist[i]);
	}
	printf("  Preds\n");
	for (int i = 0; i < sps.numNodes; i++) {
		printf("    %d : ",i);
		PredNode* curr = sps.pred[i];
		while(curr != NULL) {
			printf("[%d]->", curr->v);
			curr = curr->next;
		}
		printf("NULL\n");
	}
}

// free the memory of shortest path
void  freeShortestPaths(ShortestPaths sps) {
	int i;
	// free dist
	free(sps.dist);
	// free pred list
	for (i = 0; i < sps.numNodes; i++) {
		if (sps.pred[i] == NULL) continue;
		PredNode *curr = sps.pred[i];
		while (curr != NULL) {
			PredNode *temp = curr->next;
			free(curr);
			curr = temp;
		}
	}
	free(sps.pred);
}

// create a new ItemPQ
static ItemPQ newItemPQ(int key, int value) {
	ItemPQ item;
	item.key = key;
	item.value = value;
	return item;
}

// update the pred list without deleting existing nodes in the list
// used when finding a prev node with the same distance to the src
static void updatePred(PredNode **list, int v, int n) {
	PredNode *node = malloc(sizeof(PredNode));
	assert(node != NULL);
	node->v = v;
	node->next = list[n];
	list[n] = node;
}

// update the pred list and deleting the existing nodes in the list
// used when finding a pred node with less distance to the src
static void updateDeletePred(PredNode **list, int v, int n) {
	PredNode *node = malloc(sizeof(PredNode));
	assert(node != NULL);
	node->v = v;
	node->next = NULL;
	list[n] = node;
}