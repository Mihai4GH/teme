#include "tree.h"

int main() {
    FILE *in, *out;
    in = fopen("test.in", "r");
    out = fopen("test.out", "w");
    Tree t = buildTree(in, 1);
    printf("%d\n", checkSuffix(t, "banana$"));
    return 0;
}