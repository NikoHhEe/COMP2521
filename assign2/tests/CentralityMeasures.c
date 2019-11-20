// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

static int numPaths(PredNode **pred, int j, PredNode *next);
static int numPathi(PredNode **pred, int j, PredNode *next, int i, int flag);

// calculate the closeness parameter of the nodes
NodeValues closenessCentrality(Graph g){
    // initialize the result
    NodeValues result;
    result.numNodes = GraphNumVertices(g) - 1;
    result.values = malloc(sizeof(double) * GraphNumVertices(g));
    for (int i = 0; i < GraphNumVertices(g); i++) {
        // count num of vertices that i can reach
        double count = 1.0;

        // get the information of path start from i
        ShortestPaths path = dijkstra(g, i);
        for (int j = 0; j < GraphNumVertices(g); j++) {
            if (path.pred[j] != NULL) {
                count += 1.0;
            }
        }

        // calculate sum of all paths
        double dist = 0.0;
        double sum = 0.0;
        for (int j = 0; j < GraphNumVertices(g); j++) {
            dist = (double)(path.dist[j]);
            sum = sum + dist;
        }

        double numNodes = (double)(result.numNodes);
        // skip if sum = 0
        if (sum == 0) {
            result.values[i] = 0.0;
            continue;
        }
        double result1 = (count - 1.0)/numNodes;
        double result2 = (count - 1.0)/sum;
        result.values[i] = result1 * result2;
    }        
    return result;
}

// calculate the betweeness parameter of the nodes
NodeValues betweennessCentrality(Graph g){
    // initialize the result
    int NumV = GraphNumVertices(g);
	NodeValues result;
	result.numNodes = NumV - 1;
	result.values = malloc(sizeof(double) * GraphNumVertices(g));
	
    // check all the nodes to see their betweeness
    for (int i = 0; i < NumV; i++) {
	    double sum = 0.0;
	    for (int j = 0; j < NumV; j++) {
	        ShortestPaths new = dijkstra(g, j);
	        for (int k = 0; k < NumV; k++) {
	            //skip if i==j==k
	            if (j == i || k == i || k == j) {
                    continue;
                }
                //get num of paths which pass i amony paths from j to k
                int numi = numPathi(new.pred, j, new.pred[k], i, 0);
                if (numi == 0) continue;

                //get num of shortestpaths from j to k
	            int numPath = numPaths(new.pred, j, new.pred[k]);
                if (numPath == 0) continue;
                
                // calculate the partial result and add to sum
                double result1 = (double)numi;
                double result2 = (double)numPath;
                double partial = result1 / result2;
                sum += partial;
	        }
	    }
	    result.values[i] = sum;
	}
	return result;
}

// normalised betweenness
NodeValues betweennessCentralityNormalised(Graph g){
    // get the original betweenness
    NodeValues between = betweennessCentrality(g);
    
    // initialise the normalised result
    NodeValues normalBetween;
    normalBetween.values = malloc(sizeof(double) * GraphNumVertices(g));
    normalBetween.numNodes = GraphNumVertices(g) - 1;
    
    int numV = GraphNumVertices(g);
    // calculate the divider
    int divide = (numV - 1) * (numV - 2);
    double ddivide = (double)(divide);

    // apply the division to every value of the result
    for (int i = 0; i < numV; i++)  {
        double result = (double)(between.values[i]) / ddivide;
        normalBetween.values[i] = result;
    }
    return normalBetween;
}

// show the centrality result of the nodes
void showNodeValues(NodeValues nvs){
    for (int i = 0; i <= nvs.numNodes; i++) {
        printf("%d: %.6lf\n", i, nvs.values[i]);
    }
}

// free the values of nodeValues
void freeNodeValues(NodeValues nvs){
    free(nvs.values);
}

// calculate the num of paths from j to k
static int numPaths(PredNode **pred, int j, PredNode *node) {
    PredNode *curr = node;
    if (curr == NULL) {
        return 0;
    }
    // if find a path then check the next pred node
    else if (curr->v == j) {
        return 1 + numPaths(pred, j, curr->next);
    }
    // if no path is found, check the next pred node 
    // and the pred node of the current node recursively
    return numPaths(pred, j, pred[curr->v]) + numPaths(pred,j, curr->next);
}

// calculate the num of paths from j to k which passes i
static int numPathi(PredNode **pred, int j, PredNode *node, int i, int flag) {
    PredNode *curr = node;
    if (curr == NULL) {
        return 0;
    } 
    // if come to node j, check the flag variable
    else if (curr->v == j) {
        // if flag == 1 add 1 to the result and 
        // recursively check the next pred node
        if (flag == 1) {
            return 1 + numPathi(pred, j, curr->next, i, flag);
        }
        // else just call recursively
        return numPathi(pred, j, curr->next, i, flag);
    } 
    // if the node is i
    else if (curr->v == i) {
        // set the flag variable to 1 and recursively check the pred node
        flag = 1;
        return numPathi(pred, j, pred[curr->v], i, flag);
    } 
    // otherwise just recursively check the next pred node and the pred node
    else {
        return numPathi(pred, j, pred[curr->v], i, flag) + numPathi(pred, j, curr->next, i, flag);
    }
}
