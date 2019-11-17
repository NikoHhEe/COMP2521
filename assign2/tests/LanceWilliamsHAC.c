// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    return NULL;
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {

}
/*
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INFI 0x70000000

static void updateArray(Dendrogram *Den, int indexm, int length);
static double Dist(double **dist, int i, int j, int k, int method);
static void updateMatrix(double **dist, int i, int length);
static void updateMatrixArray(double *list, int index, int length);
static void updateMatrixRow(double **dist, int i, int length);

Dendrogram LanceWilliamsHAC(Graph g, int method) {
    //init dist[][] to 
    double **dist;
    dist = malloc(sizeof(double) * GraphNumVertices(g) * GraphNumVertices(g));
    int i;
    for (i = 0; i < GraphNumVertices(g); i++) {
        int j;
        for (j = 0; j < GraphNumVertices(g); j++) {
            dist[i][j] = (double)INFI;
        }
    }
    //assign all dist in g to dist[][]
    int m;
    for (m = 0; m < GraphNumVertices(g); m++) {
        dist[m][m] = 0;
        AdjList curr = GraphOutIncident(g, m);
        int n = 0;
        while (curr != NULL) {
            double temp = (double)(curr->weight);
            dist[m][curr->weight] = 1.0/temp;
            curr = curr->next;
            n++;
        }
    }
    //choose a smaller one for dist[][]
    int c;
    for (c = 0; c < GraphNumVertices(g); c++) {
        int d;
        for (d = 0; d < GraphNumVertices(g); d++) {
            if (dist[c][d] < dist[d][c]) {
                dist[d][c] = dist[c][d];
            } else {
                dist[c][d] = dist[d][c];
            }
        }
    }
    
    //init Dendrogram array
    Dendrogram *Den = malloc(sizeof(struct DNode) * GraphNumVertices(g));
    int k;
    for (k = 0; k < GraphNumVertices(g); k++) {
        Den[k]->vertex = k;
        Den[k]->left = NULL;
        Den[k]->right = NULL;
    }
    int n;
    int len = GraphNumVertices(g);
    for(n = 1; n < GraphNumVertices(g); n++) {
        //find closest clusters
        int row;
        double min = (double)INFI;
        int start;
        int end;
        for (row = 0; row < GraphNumVertices(g); row++) {
            int col;
            for (col = 0; col < GraphNumVertices(g); col++) {
                if (dist[row][col] < min) {
                    min = dist[row][col];
                    start = row;
                    end = col;
                }
            }
        }
        //merge two clusters(update dentrogram)
        Dendrogram new = malloc(sizeof(struct DNode));
        if (start < end) {
            new->vertex = start;
            new->left = Den[start];
            new->right = Den[end];
            //update Dentrogram array
            Den[start] = new;
            updateArray(Den, end, 10);
        } else {
            new->vertex = end;
            new->left = Den[end];
            new->right = Den[start];
            //update Dentrogram array
            Den[end] = new;
            updateArray(Den, start, 10);
        }
        //decreament length of the array and matrix
        len--;
        //update dist[][]
        if (start > end) {
            updateMatrix(dist, start, len);
        }
            
        else {
            updateMatrix(dist, end, len);

        }
        int a;
        //if end has been removed
        if (start < end) {
            for (a = 0; a < len; a++) {
                if (a == start) dist[start][a] = (double)INFI;
                double res = Dist(dist, start, end, a, method);
                dist[start][a] = res;
                dist[a][start] = res;
            }
        } else {
            for (a = 0; a < len; a++) {
                if (a == end) dist[end][a] = (double)INFI;
                double res = Dist(dist, end, start, a, method);
                dist[end][a] = res;
                dist[a][end] = res;
            }
        }
    }
    return Den[0];
}

static void updateMatrix(double **dist, int i, int length){
    int j;
    for (j = 0; j < length; j++) {
        updateMatrixArray(dist[i], i, length);
    }
    updateMatrixRow(dist, i, length);
}


//deleting an element from the matrixarray
static void updateMatrixArray(double *dist, int index, int length) {
    int i = index;
    while (i < length) {
	    dist[i] = dist[i+1];
	    i++;
    }
    dist[i] = 0.0;
}


// deleting a row from the matrix
static void updateMatrixRow(double **dist, int i, int length) {
    int j;
    for (j = 0; j < length; j++) {
	    dist[i][j] = dist[i+1][j];
    }
    int k;
    for (k = 0;k < length; k++) {
	    dist[length][k] = 0.0;
    }
}

//calculate new distance for the new cluster
//method should be = 1 or = 2
//method = 1 single link
//method = 2 complete link
static double Dist(double **dist, int i, int j, int k, int method) {
    double res = 0.0;
    if (method == 1) {
	    res = 0.5 * dist[i][k] + 0.5 * dist[j][k] - 0.5 * abs(dist[i][k] - dist[j][k]);
    } else {
	    res = 0.5 * dist[i][k] + 0.5 * dist[j][k] + 0.5 * abs(dist[i][k] - dist[j][k]);
    }
    return res;
}
static void updateArray(Dendrogram *Den, int index, int length) {
    int i = index;
    while (i < length) {
	    Den[i] = Den[i+1];
	    i++;
    }
    Den[i] = NULL;
}
*/
