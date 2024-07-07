#ifndef __WGRAPH_H__
#define __WGRAPH_H__

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "weightlist.h"
#include "heap.h"

#define SIZE 200
#define INFINITY 999999
#define SIZE_NAME 30

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct wgraph {
	int V; // nr de noduri din graf
    int E; // nr de laturi
	WList *adjLists; // vectorul cu listele de adiacență
	int *visited; // vector pentru marcarea nodurilor vizitate
	int *d; // Vector pentru distante
    int *start; // Orasul de start;
} *WGraph;

typedef struct wedge {
	int u, v, keep;
} WEdge;

WGraph initWGraph();
WGraph insertWEdge(WGraph g, int u, int v, int cost);
void printWGraph(WGraph g);
void printWEdges(int E, WEdge *edges, HTable t);
WGraph readWeightedGraph(HTable t, WEdge **edges, int *K);
WGraph freeGraphW(WGraph graph);
int compare(const void *a, const void *b);
void Dijkstra(WGraph g, int K, WEdge *edges, HTable t);



#endif /* __WGRAPH_H__ */