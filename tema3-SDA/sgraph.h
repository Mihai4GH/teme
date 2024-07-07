#ifndef __SGRAPH_H__
#define __SGRAPH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CITIES 100
#define SECIONS 100


typedef struct sectionGraph {
    int V, E;
    List *adjLists;
    int *visited;
} *SGraph;

typedef struct edge {
    int u, v;    
} Edge;

SGraph initGraph();
SGraph insertEdge(SGraph g, int u, int v, int nr_sec, float *sections);
void printEdges(int E, Edge *edges, HTable t);
void formatPrintEdges(SGraph g, Edge *edges, HTable t, float L);
void printGraph(SGraph g);
SGraph freeGraph(SGraph graph);
SGraph readSGraph(HTable t, Edge **edges, float *L, int *K);
SGraph updateOld(SGraph g);
SGraph doubleWear(SGraph g);
SGraph UpdateNonUsedSections(SGraph g);
SGraph passOneYear(SGraph g);
SGraph passKYears(SGraph g, int k);

#endif /* __SGRAPH_H__ */