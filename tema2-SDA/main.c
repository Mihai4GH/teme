#include "tree.h"

void run(int argc, char *argv[]) {
    if(argc < 4)
        exit(1);
    if(!strcmp("-c1", argv[1])) {
        FILE  *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w");
        c1(in, out);
        fclose(in);
        fclose(out);
    }
    if(!strcmp("-c2", argv[1])) {
        FILE *in = fopen(argv[3], "r");
        FILE *out = fopen(argv[4], "w");
        int k = atoi(argv[2]);
        c2(in, out, k);
        fclose(in);
        fclose(out);
    }
    if (!strcmp("-c3", argv[1])) {
        FILE  *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w");
        c3(in, out);
        fclose(in);
        fclose(out);
    }
    if (!strcmp("-c4", argv[1])) {
        FILE  *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w"); 
        c4(in, out);
        fclose(in);
        fclose(out);
    }
}

int main(int argc, char *argv[]) {
    run(argc, argv);
}