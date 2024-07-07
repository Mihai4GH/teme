#include "sgraph.h"




SGraph initGraph() {
    SGraph g;
	g = (SGraph) malloc(sizeof(struct sectionGraph));
	g->V = g->E = 0;
    g->visited = calloc(sizeof(int), CITIES);
    g->adjLists = calloc(sizeof(List), CITIES);
	return g;
}

SGraph insertEdge(SGraph g, int u, int v, int nr_sec, float *sections) {
    if(MAX(u, v) >= g->V) {
        g->V = MAX(u, v);
    }
    g->E++;
    Pair p1, p2;
    p1.u = u;
    p1.v = v;
    p2.u = v;
    p2.v = u;
    p1.nr_sec = p2.nr_sec = nr_sec;
    p1.sections = calloc(nr_sec, sizeof(float));
    p1.old = calloc(nr_sec, sizeof(float));
    p2.sections = calloc(nr_sec, sizeof(float));
    p2.old = calloc(nr_sec, sizeof(float));
    for (int i = 0; i < nr_sec; i++) {
        p1.sections[i] = sections[i];
        p2.sections[nr_sec-i-1] = sections[i];
    }
    g->adjLists[u] = addLast(g->adjLists[u], p1);
    g->adjLists[v] = addLast(g->adjLists[v], p2);
    return g;
}

void printGraph(SGraph g) {
    for (int i=1; i<=g->V; i++) {
        printf("[ %d ]", i);
        List iter = g->adjLists[i];
        while (iter) {
            printf("->%d ", iter->data.v);
            printf("{");
            for (int j = 0; j<iter->data.nr_sec; j++)
                printf("%.2f ", iter->data.sections[j]);
            iter = iter->next;
            printf("}");
        }
        printf("\n");
    }
}

void printEdges(int E, Edge *edges, HTable t) {
    for (int i = 0; i < E; i++){
        printf("%s %s\n", RequestId(t, edges[i].u-1), RequestId(t, edges[i].v-1));
    }
}

void formatPrintEdges(SGraph g, Edge *edges, HTable t, float L) {
    FILE *out = fopen("tema3.out", "w");
    float *mean = calloc(g->E, sizeof(float));
    for (int i = 0; i < g->E; i++) {
        fprintf(out, "%s %s ", RequestId(t, (edges[i].u)-1), RequestId(t, edges[i].v-1));
        List iter = g->adjLists[edges[i].u];
        while(iter && iter->data.v != edges[i].v) {
            iter = iter->next;
        }
        if(!iter) {
            printf("EROARE FPE\n");
            exit(1);
        }
        fprintf(out, "%d ", iter->data.nr_sec);
        for (int j = 0; j < iter->data.nr_sec; j++) {
            fprintf(out, "%.2f ", iter->data.sections[j]);
            mean[i]+=iter->data.sections[j];
        }
        mean[i]/=iter->data.nr_sec;
        fprintf(out, "\n");
    }
    int OK = 1;
    for (int i = 0; i < g->E; i++) 
        if(mean[i] < L)
            fprintf(out, "%d ", i+1), OK = 0;
    free(mean);
    if(!OK)
        fprintf(out, "\n");
}

SGraph freeGraph(SGraph graph) {
    int i;
	if (!graph)
		return graph;
	if (graph->visited)
		free(graph->visited);
	if (graph->adjLists) {
		for (i = 0; i <= graph->V; i++)
			graph->adjLists[i] = freeList(graph->adjLists[i]);
		free(graph->adjLists);
	}
	free(graph);
	return NULL;
}


SGraph readSGraph(HTable t, Edge **edges, float *L, int *K) {
    FILE *in = fopen("tema3.in", "r");
    int E, nr_sections;
    float sections[SECIONS];
    char *town1 = malloc(NAME_LEN);
    char *town2 = malloc(NAME_LEN);
    SGraph g = initGraph();
    fscanf(in, "%d%d%f", &E, K, L);
    *edges = malloc(E * sizeof(Edge));
    for(int i = 0; i < E; i++) {
        fscanf(in, "%s%s%d", town1, town2, &nr_sections);
        if(RequestKey(t, town1) < 0)
            addToTable(t, town1);
        if(RequestKey(t, town2) < 0)
            addToTable(t, town2);
        int u = RequestKey(t, town1);
        int v = RequestKey(t, town2);
        (*edges)[i].u = u;
        (*edges)[i].v = v;
        for (int i = 0; i < nr_sections; i++) {
            fscanf(in, "%f", &sections[i]);
        }
        g = insertEdge(g, u, v, nr_sections, sections);
    }
    fclose(in);
    free(town1);
    free(town2);
    return g;
}

SGraph updateOld(SGraph g) {
    for (int i = 1; i <= g->V; i++) {
        List iter = g->adjLists[i];
        while (iter) {
            for(int j = 0; j < iter->data.nr_sec; j++) 
                iter->data.old[j] = iter->data.sections[j];
            iter = iter->next;
        }
    }
    return g;
}

SGraph doubleWear(SGraph g) {
    for (int i = 1; i <= g->V; i++) {
        List iter = g->adjLists[i];
        while (iter) {
            for (int j = 0; j < iter->data.nr_sec; j++) {
                iter->data.sections[j] = 2 * iter->data.sections[j];
                if (iter->data.sections[j] > 100)
                    iter->data.sections[j] = 100;
            }
            iter = iter->next;
        }
    }
    return g;
}

SGraph UpdateNonUsedSections(SGraph g) {
    for (int i = 1; i <= g->V; i++) {
        List iter = g->adjLists[i];
        while (iter) {
            // First we check the inner sections of each Route
            for (int j = 1; j < iter->data.nr_sec - 1; j++) {
                if(!iter->data.sections[j])
                    iter->data.sections[j] =  MAX(iter->data.old[j-1], iter->data.old[j+1]) / 2;
            }
            // Check the first 
            if(!iter->data.sections[0]) {
                float max = 0;
                // Tratam separat cazul in care avem mai mult de un tronson
                if (iter->data.nr_sec > 1) {
                    max = MAX( max, iter->data.old[1]);
                } else {
                    List trns = g->adjLists[iter->data.v];
                    while (trns) {
                        max = MAX(max, trns->data.old[0]);
                        trns = trns->next;
                    }
                }
                List trns = g->adjLists[iter->data.u];
                while (trns) {
                    max = MAX(max, trns->data.old[0]);
                    trns = trns->next;
                }
                iter->data.sections[0] = max/2;
            }
            // Verificam daca ultimul tronson (pt nr_sec > 1) e 0
            if(iter->data.nr_sec > 1 && iter->data.sections[iter->data.nr_sec - 1] == 0) {
                float max = MAX(0, iter->data.old[iter->data.nr_sec-2]);
                List trns = g->adjLists[iter->data.v];
                while (trns) {
                    max = MAX(max, trns->data.old[0]);
                    trns = trns->next;
                }
                iter->data.sections[iter->data.nr_sec-1] = max/2;
            }
        
        iter = iter->next;
        }
    }
    return g;
}

SGraph passOneYear(SGraph g) {
    g = updateOld(g);
    g = doubleWear(g);
    g = UpdateNonUsedSections(g);
    return g;
}

SGraph passKYears(SGraph g, int k) {
    for (int i = 0; i < k; i++) {
        g = passOneYear(g);
    }
    return g;
}