#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "PQ.h"
#include "Dijkstra.h"
#include "Graph.h"

#define INFI 63356

static ItemPQ createItem(int key, int value);
static void updatePred(PredNode **list, int v, int w);
static void updateDeletePred(PredNode **list, int v, int w);

ShortestPaths dijkstra(Graph g, Vertex src) {
	//init ShortestPaths
	ShortestPaths new;
	new.dist = malloc(sizeof(int) * (GraphNumVertices(g)));
	assert((new.dist) != NULL);
	new.pred = malloc(sizeof(struct PredNode) * (GraphNumVertices(g)));
	assert((new.pred) != NULL);
	new.src = src;
	new.numNodes = GraphNumVertices(g);
	//init dist and priority queue
	PQ q = PQNew();
	int i;
	for (i = 0; i < new.numNodes; i++) {
		new.dist[i] = INFI;
		new.pred[i] = NULL;
		ItemPQ newItem = createItem(i, new.dist[i]);
		PQAdd(q, newItem);
	}
	new.dist[src] = 0;
	//new.pred[src] = NULL;

	ItemPQ update = createItem(src, new.dist[src]);
	PQUpdate(q, update);
	while (!PQIsEmpty(q)) {
		ItemPQ sItem = PQDequeue(q);
		Vertex s = sItem.key;
		AdjList out = GraphOutIncident(g, s);
		AdjList curr = out;
		while (curr != NULL) {
			int len = curr->weight;
			if ((new.dist[s] + len) == (new.dist[curr->v])) {
				//update dist
				new.dist[curr->v] = (new.dist[s]) + len;
				//update queue
				ItemPQ newDist = createItem(curr->v, new.dist[curr->v]);
				PQUpdate(q, newDist);
				//update pred
				updatePred(new.pred, s, curr->v);
			}
			if ((new.dist[s] + len) < (new.dist[curr->v])) {
				//update dist
				new.dist[curr->v] = (new.dist[s]) + len;
				//update queue
				ItemPQ newDist = createItem(curr->v, new.dist[curr->v]);
				PQUpdate(q, newDist);
				//update pred
				updateDeletePred(new.pred, s, curr->v);
			}
			curr = curr->next;
		}
	}
	PQFree(q);
	return new;
}

void  showShortestPaths(ShortestPaths s) {
	int i = 0;
	printf("Node %d\n",s.src);
	printf("  Distance\n");
	for (i = 0; i < s.numNodes; i++) {
		if(i == s.src)
	    	printf("    %d : X\n",i);
		else
			printf("    %d : %d\n",i,s.dist[i]);
	}
	printf("  Preds\n");
	for (i = 0; i < s.numNodes; i++) {
		printf("    %d : ",i);
		PredNode* curr = s.pred[i];
		while(curr!=NULL) {
			printf("[%d]->",curr->v);
			curr = curr->next;
		}
		printf("NULL\n");
	}
}

void  freeShortestPaths(ShortestPaths s) {
	int i;
	//free distance
	free(s.dist);
	//free pred
	for (i = 0; i < s.numNodes; i++) {
		if (s.pred[i] == NULL) continue;
		PredNode *curr = s.pred[i];
		while (curr != NULL) {
			PredNode *temp = curr->next;
			free(curr);
			curr = temp;
		}
	}
	free(s.pred);
}

static ItemPQ createItem(int key, int value) {
	ItemPQ new;
	new.key = key;
	new.value = value;
	return new;
}

static void updatePred(PredNode **list, int v, int n) {
	PredNode *new = malloc(sizeof(PredNode));
	assert(new != NULL);
	new->v = v;
	new->next = list[n];
	list[n] = new;
}

static void updateDeletePred(PredNode **list, int v, int n) {
	PredNode *new = malloc(sizeof(PredNode));
	assert(new != NULL);
	new->v = v;
	new->next = NULL;
	list[n] = new;
}