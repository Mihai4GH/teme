#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgraph.h"
#include "wgraph.h"

void run_first_task() {
    Edge *edges;
    HTable t = initTable(1);
    float L;
    int k;
    SGraph g = readSGraph(t, &edges, &L, &k);
    g = passKYears(g, k);
    formatPrintEdges(g, edges, t, L);
    freeGraph(g);
    freeTable(t);
    free(edges);
}

void run_second_task() {
    WEdge *edges;
    HTable t = initTable(1);
    int K;
    WGraph g = readWeightedGraph(t, &edges, &K);
    Dijkstra(g, K, edges, t);
    freeTable(t);
    freeGraphW(g);
    free(edges);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if(!strcmp(argv[1], "1"))
            run_first_task();
        if(!strcmp(argv[1], "2"))
            run_second_task();
    }
    return 0;
}