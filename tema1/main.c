#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "train.h"

#define input_file "tema1.in"
#define output_file "tema1.out"

void run() {
    FILE *in = fopen(input_file, "r");
    FILE *out = fopen(output_file, "w");
    TTrain train = INIT_TRAIN();
    CQueue q = init_queue();
    int n;
    fscanf(in, "%d", &n);
    for(int i = 0; i < n; i++) {
        char str[20];
        fscanf(in, "%s", str);
        if(strcmp(str, "EXECUTE") == 0) {               // EXECUTE
            execute(train, q, out);                     // --------
        } else if(strcmp(str, "SHOW") == 0) {           // QUERRY
            show(train, out);
        } else if(strcmp(str, "SHOW_CURRENT") == 0) {
            show_current(train, out);
        } else if(strcmp(str, "SWITCH") == 0) {         
            switch_queue(q);                            // --------
        } else {
            enqueue(q, str, in);                        // UPDATE & SEARCH
        }
    }
    destroy_queue(q);
    DESTROY_TRAIN(train);
    fclose(out);
    fclose(in);
}

int main() {
    run();
    return 0;
}