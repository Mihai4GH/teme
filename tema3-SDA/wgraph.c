#include "wgraph.h"

WGraph initWGraph()  {
    WGraph g = (WGraph) malloc(sizeof(struct wgraph));
    g->V = g->E = 0;
    g->visited = calloc(sizeof(int), SIZE);
    g->d = calloc(sizeof(int), SIZE);
    g->adjLists = calloc(sizeof(WList), SIZE);
    g->start = malloc(sizeof(int));
    return g;
}

WGraph insertWEdge(WGraph g, int u, int v, int cost) {
    if(max(u, v) >= g->V) {
        g->V = max(u, v);
    }
    g->E++;
    WPair p1, p2;
    p1.u = u;
    p1.v = v;
    p1.cost = cost;
    g->adjLists[u] = addLastW(g->adjLists[u], p1);
    p2.u = v;
    p2.v = u;
    p2.cost = cost;
    g->adjLists[v] = addLastW(g->adjLists[v], p2);
    return g;
}

void printWGraph(WGraph g) {
    for (int i=1; i <= g->V; i++) {
        printf("[ %d ]", i);
        WList iter = g->adjLists[i];
        while (iter) {
            printf("->%d {%d} ", iter->data.v, iter->data.cost);
            iter = iter->next;
        }
        printf("\n");
    }
}

void printWEdges(int E, WEdge *edges, HTable t) {
    for (int i = 0; i < E; i++){
        printf("%d-%s %d-%s\n", edges[i].u, RequestId(t, (edges[i].u)-1), edges[i].v, RequestId(t, edges[i].v-1));
    }
}

WGraph readWeightedGraph(HTable t, WEdge **edges, int *K) {
    FILE *in = fopen("tema3.in", "r");
    int E, cost;
    char *startingTown = malloc(SIZE_NAME);
    char *town1 = malloc(SIZE_NAME);
    char *town2 = malloc(SIZE_NAME);
    WGraph g = initWGraph();
    fscanf(in, "%s %d %d", startingTown, K, &E);
    *edges = malloc(E * sizeof(WEdge));
    for (int i = 0; i < E; i++) {
        fscanf(in, "%s%s%d", town1, town2, &cost);
        if (RequestKey(t, town1) < 0)
            addToTable(t, town1);
        if (RequestKey(t, town2) < 0)
            addToTable(t, town2);
        int u = RequestKey(t, town1);
        int v = RequestKey(t, town2);
        (*edges)[i].u = u;
        (*edges)[i].v = v;
        g = insertWEdge(g, u, v, cost);
    }
    *(g->start) = RequestKey(t, startingTown);
    free(startingTown);
    fclose(in);
    free(town1);
    free(town2);
    return g;
}

WGraph freeGraphW(WGraph graph) {
    int i;
	if (!graph)
		return graph;
    if (graph->d)
        {free(graph->d);}
    if (graph->start)
        {free(graph->start);}
	if (graph->visited)
		{free(graph->visited);}
	if (graph->adjLists) {
		for (i = 0; i <= graph->V; i++)
			graph->adjLists[i] = freeListW(graph->adjLists[i]);
		free(graph->adjLists);
	}
	free(graph);
	return NULL;
}

int compare(const void *a, const void *b) {
	HPair *pa, *pb;
	pa = (HPair*) a;
	pb = (HPair*) b;
	return pb->cost - pa->cost;
}

void Dijkstra(WGraph g, int K, WEdge *edges, HTable t) {
    FILE *out = fopen("tema3.out", "w");
    int cnt = 0;
    Heap h = initHeap(compare);
    int *P = calloc(g->V+1, sizeof(int));
    for (int i=1; i<=g->V; i++) {
        g->d[i] = INFINITY;
    }
    WList iter = g->adjLists[*g->start]; 
    while (iter) {
        g->d[iter->data.v] = iter->data.cost;
        P[iter->data.v] = iter->data.u;
        HPair heapPair;
        heapPair.node = iter->data.v;
        heapPair.cost = iter->data.cost;
        h = insertHeap(h, heapPair);
        iter = iter->next;
    }
    g->d[*g->start] = 0;
    g->visited[*g->start] = 1;
    if (K >= g->V)
        K = g->V - 1;
    while (!isEmptyHeap(h) && cnt < K) {
        HPair heapPair = extractMin(h); 
        if (g->visited[heapPair.node]) {
            continue;
        }
        cnt++;
        g->visited[heapPair.node] = 1;
        iter = g->adjLists[heapPair.node];
        while (iter) {
            HPair new_pair;
            new_pair.cost = iter->data.cost+g->d[iter->data.u];
            new_pair.node = iter->data.v;
            if (!g->visited[iter->data.v] && g->d[iter->data.v] > g->d[iter->data.u] + iter->data.cost) {
                g->d[iter->data.v] = g->d[iter->data.u] + iter->data.cost;
                P[iter->data.v] = iter->data.u;
                h = insertHeap(h, new_pair);
            }
            iter = iter->next;
        }
    }
    fprintf(out, "%d\n", cnt);
    for (int i=0; i<g->E; i++) {
        if(g->visited[edges[i].u] && g->visited[edges[i].v] && (P[edges[i].v] == edges[i].u || P[edges[i].u] == edges[i].v)) 
            fprintf(out, "%s %s\n", RequestId(t, edges[i].u-1), RequestId(t, edges[i].v-1));
    }
    freeHeap(h);
    free(P);
    fclose(out);
}