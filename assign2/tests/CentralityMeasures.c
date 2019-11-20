// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Paths {
    int NumPath;
    int path[100][100];
} Paths;

static int numPaths(PredNode **pred, int j, PredNode *next);
static int numPassi(PredNode **pred, int j, PredNode *next, int i, int f);
//static Paths *getPath(Paths *new, PredNode **curr, int j, int k, int row, int col);



NodeValues closenessCentrality(Graph g){
    NodeValues new;
    new.numNodes = GraphNumVertices(g) - 1;
    new.values = malloc(sizeof(double) * GraphNumVertices(g));
    int i;
    for (i = 0; i < GraphNumVertices(g); i++) {
        //count # of vertices that i can reach
        double cnt = 1.0;
        int m;
        ShortestPaths newpath = dijkstra(g, i);
        for (m = 0; m < GraphNumVertices(g); m++) {
            if (newpath.pred[m] != NULL) {
                cnt = cnt + 1.0;
            }
        }
        //calculate sum of all paths
        double dist = 0.0;
        int j;
        double sum = 0.0;
        for (j = 0; j < GraphNumVertices(g); j++) {
            dist = (double)(newpath.dist[j]);
            sum = sum + dist;
        }
        //get final res
        double numNodes = (double)(new.numNodes);
        if (sum == 0) {
            new.values[i] = 0.0;
            continue;
        }
        //multiply left and right to get values[i]
        double left = (cnt - 1.0)/numNodes;
        double right = (cnt - 1.0)/sum;
        new.values[i] = left * right;
    }
            
     return new;
}

NodeValues betweennessCentrality(Graph g){
    int NumVertices = GraphNumVertices(g);
	NodeValues new;
	new.numNodes = NumVertices - 1;
	new.values = malloc(sizeof(double) * GraphNumVertices(g));
	int i;
	for (i = 0; i < NumVertices; i++) {
	    double sum = 0.0;
	    int j;
	    int k;
	    for (j = 0; j < NumVertices; j++) {
	        //skip if j == i
	        if (j == i) continue;
	        ShortestPaths new = dijkstra(g, j);
	        for (k = 0; k < NumVertices; k++) {
	            //skip if i==j==k
	            if (k == i || k == j) continue;
	        
                //get # paths which pass i
                int passi = numPassi(new.pred, j, new.pred[k], i, 0);
                if (passi == 0) continue;
                //get # shortestpaths from j to k
	            int numOfPaths = numPaths(new.pred, j, new.pred[k]);
                if (numOfPaths == 0) continue;
                
                //convert num and passi to double
                double up = (double)passi;
                double down = (double)numOfPaths;
                double depend = up/down;
                sum = sum + depend;
	        }
	    }
	    new.values[i] = sum;
	}
	return new;
}

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues b = betweennessCentrality(g);
    NodeValues normal;
    normal.values = malloc(sizeof(double) * GraphNumVertices(g));
    normal.numNodes = GraphNumVertices(g) - 1;
    int n = GraphNumVertices(g);
    //calculate denominator as int
    int denominator = (n-1) * (n-2);
    //convert int to double
    double de = (double)(denominator);
    //normalise all betweeness and place them into node.values[]
    int i;
    for (i = 0; i < n; i++)  {
        double left = 1/de;
        double right = (double)(b.values[i]);
        normal.values[i] = left * right;
    }
    return normal;
}

void showNodeValues(NodeValues values){
    int i;
    for (i = 0; i <= values.numNodes; i++) {
        printf("%d: %.6lf\n", i, values.values[i]);
    }
}

void freeNodeValues(NodeValues values){
    free(values.values);
}

static int numPaths(PredNode **pred, int j, PredNode *next) {
    PredNode *curr = next;
    //if no path available
    if (curr == NULL) return 0;
    //if find one path and continue to next pred node
    if (curr->v == j) {
        return 1 + numPaths(pred, j, curr->next);
    }
    //normal case(path not found and continue to find recursively)
    return numPaths(pred, j, pred[curr->v]) + numPaths(pred,j, curr->next);
}

static int numPassi(PredNode **pred, int j, PredNode *next, int i, int f) {
    PredNode *curr = next;
    if (next == NULL) {
        return 0;
    } else if (curr->v == j) {
        if (f == 1) {
            return 1 + numPassi(pred, j, curr->next, i, f);
        }
        return numPassi(pred, j, curr->next, i, f);
    } else if (curr->v == i) {
        f = 1;
        return numPassi(pred, j, pred[curr->v], i, f);
    } else {
        return numPassi(pred, j, pred[curr->v], i, f) + numPassi(pred, j, curr->next, i, f);
    }
}
