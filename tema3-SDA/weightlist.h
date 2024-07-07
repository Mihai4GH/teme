#ifndef __WEIGHTLIST_H__
#define __WEIGHTLIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct weightpair {
    int u, v, cost;
} WPair; 

// Pair consisnt of u, v vertexes and cost 
typedef WPair V;

typedef struct wlist {
    V data;
    struct wlist *next, *prev;
} *WList;

WList initListW(V data);
WList addFirstW(WList list, V data);
WList addLastW(WList list, V data);
WList freeListW(WList list);


#endif /* __LIST_H__ */