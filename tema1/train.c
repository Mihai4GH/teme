#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "train.h"

Node create_node(char value) {
    Node node = (Node)malloc(sizeof(NodeList));
    node->label = value;
    node->next = node->prev = NULL;
    return node;
}

TTrain INIT_TRAIN() {
    TTrain train = (TTrain)malloc(sizeof(TrainList));
    train->sentinel = create_node(0);
    train->conductor = create_node('#');
    // Conductors wagon
    train->conductor->prev = train->sentinel;   
    train->conductor->next = train->sentinel;
    // Connect the locomotive with the conductor's wagon
    train->sentinel->next = train->conductor;   
    train->sentinel->prev = train->conductor;

    return train;
}

void move_left(TTrain train) {
    train->conductor = train->conductor->prev;
    // Making sure that the conductor isnt in the sentinel
    if(train->conductor == train->sentinel)
        train->conductor = train->conductor->prev;
}

void move_right(TTrain train) {
    if(train->conductor->next == train->sentinel) { // Adding a new car if needed
        Node new_node = create_node('#');
        new_node->next = train->sentinel;
        new_node->prev = train->conductor;

        train->conductor->next = new_node;
        train->sentinel->prev = new_node;
    }
    train->conductor = train->conductor->next;      // Moving the conductor
}

void write(TTrain train, char label) {
    train->conductor->label = label;
}

void clear_cell(TTrain train) {
    if(train->conductor->prev == train->sentinel && train->conductor->next == train->sentinel) {
        /* Treating the case in wich */
        train->conductor->label = '#';
        return;
    }
    Node aux = train->conductor;
    train->conductor = aux->prev;
    train->conductor->next = aux->next;
    aux->next->prev = train->conductor;
    free(aux);
    if(train->conductor == train->sentinel) {
        train->conductor = train->conductor->prev;
    }
}

void clear_all(TTrain train) {
    while(train->conductor->prev != train->sentinel || train->conductor->next != train->sentinel) {
        clear_cell(train);
    }
    train->conductor->label = '#';
}

void insert_left(TTrain train, char label, FILE *out) {
    if(train->sentinel->next == train->conductor) {
        // Can't inssert another car before the locomotive
        fprintf(out, "ERROR\n");
        return;
    }
    Node cell = create_node(label);
    // Establishing the connections
    cell->next = train->conductor;
    cell->prev = train->conductor->prev;
    train->conductor->prev->next = cell;
    train->conductor->prev = cell;
    train->conductor = cell;
}

void insert_right(TTrain train, char label) {
    Node cell = create_node(label);
    // Establishing  the connections
    cell->prev = train->conductor;
    cell->next = train->conductor->next;
    train->conductor->next->prev = cell;
    train->conductor->next = cell;
    train->conductor = cell;
}

TTrain DESTROY_TRAIN(TTrain train) {
    clear_all(train);
    free(train->conductor);
    free(train->sentinel);
    free(train);
    return NULL;
}

// Search

void search(TTrain train, const char *s, FILE *out) { 
    Node iter = train->conductor, pos = NULL;
    /* We are iterating trough every car **only once** we check
    every match found on the way. */
    int k = 0, len = strlen(s), ok;
    do {
        if(iter->label == s[0]) {
            pos = iter;
            ok = 1;
            for(k = 1; k < len; k++) {
                iter = iter->next;              //
                if(iter == train->sentinel) {   //  Making sure that iter differs from sentinel
                    iter = iter->next;          //
                }                               //
                if(iter->label != s[k]) {
                    iter = pos;
                    ok = 0;                     //  Word doesn't match 
                    break;                      //  break and reset iter
                }

                if(iter == train->conductor) {  // Condition not to go more times
                    fprintf(out, "ERROR\n");    // Words wasn't found
                    return;
                }
            }
            if(ok == 1) {
                train->conductor = pos;         // Word found!
                return;
            }
        } 

        iter = iter->next;                              //  Ne asiguram ca iter
        if(iter == train->sentinel) iter = iter->next;  //  nu este nodul santinela
    } while(train->conductor != iter);

    fprintf(out, "ERROR\n");
}

void search_left(TTrain train, const char *s, FILE *out) {
    /* Algorithm is similar to the one above. Only diference is that 
    this one searches to the left and stops at the sentinell */
    Node iter = train->conductor, pos = NULL;
    int len = strlen(s), ok = 1;
    while(iter != train->sentinel) {
        if(iter->label == s[0]) {
            pos = iter;
            ok = 1;
            for (int i = 1; i < len ; i++) {
                iter = iter->prev;
                
                if(iter == train->sentinel) {
                    fprintf(out, "ERROR\n"); // Word wasn't found
                    return;
                }
                if(s[i] != iter->label) {
                    ok = 0;
                    iter = pos;     // Reseting iter and continuing search
                    break;
                }

            }
            if(ok == 1) {
                train->conductor = iter; // Word found!
                return;
            }
            
        }

        iter = iter->prev;
    }

    fprintf(out ,"ERROR\n");            // Word wasn't found!
}

void search_right(TTrain train, const char *s, FILE *out) {
    Node iter = train->conductor, pos = NULL;
    int len = strlen(s), ok = 1;
    /* This time we search to the right */
    while(iter != train->sentinel) {
        if(iter->label == s[0]) {
            pos = iter;
            ok = 1;
            for(int i = 1; i < len; i++) {
                iter = iter->next;
                if(iter == train->sentinel) {
                    fprintf(out, "ERROR\n"); // Word wasn't found
                    return;
                }
                if(s[i] != iter->label) {
                    ok = 0;
                    iter = pos;             // Reset iter 
                    break;                  // Continue searching
                }
            }

            if(ok == 1) {
                train->conductor = iter;    // Word found!
                return;
            }
        }
        iter = iter->next;
    }

    fprintf(out, "ERROR\n");                // Word wasn't found!
}

// Querry

void show(TTrain train, FILE *out) {
    /* Prints the train */
    Node iter = train->sentinel->next;
    while(iter != train->sentinel) {
        if(iter == train->conductor) {
            fprintf(out, "|%c|", iter->label);
        } else {
            fprintf(out, "%c", iter->label);
        }
        iter = iter->next;
    }
    fprintf(out, "\n");
}

void show_current(TTrain train, FILE *out) {
    fprintf(out, "%c\n", train->conductor->label);
}

void switch_queue(CQueue q) {
    q->QSwitch = q->QSwitch * (-1);
    /* Switches the switch inside the q. 
    By doing this execute, procces_command_queue
    and enqueue will alter theirs functionalities.
    */
}

// EXECUTE

void execute(TTrain train, CQueue q, FILE *out) {
    if(q == NULL || q->first == NULL) return;
    if(q->QSwitch == 1) {
        // Default way of proccesing commands from a queue
        procces_command_queue(train, q->first, out);
        dequeue_first(q);
    } else if(q->QSwitch == -1) {
        // Altered way of proccesing command from queue. (Switch)
        procces_command_queue(train, q->last, out);
        dequeue_last(q);
    } else {
        fprintf(stderr, "EROARE EXECUTE -> QSwitch value is altered");
    }
}

/* procces_command_queue determines withc command is to be executed */

void procces_command_queue(TTrain train, CNode node, FILE *out) {
    if(strcmp(node->command, "MOVE_LEFT") == 0) {
        move_left(train);
        return;
    }
    if(strcmp(node->command, "MOVE_RIGHT") == 0) {
        move_right(train);
        return;
    }
    if(strcmp(node->command, "WRITE") == 0) {
        write(train, *(node->ArgStr));
        return;
    } 
    if(strcmp(node->command, "CLEAR_CELL") == 0) {
        clear_cell(train);
        return;
    }
    if(strcmp(node->command, "CLEAR_ALL") == 0) {
        clear_all(train);
        return;
    }
    if(strcmp(node->command, "INSERT_RIGHT") == 0) {
        insert_right(train, *(node->ArgStr));
        return;
    }
    if(strcmp(node->command, "INSERT_LEFT") == 0) {
        insert_left(train, *(node->ArgStr), out);
        return;
    }
    if(strcmp(node->command, "SEARCH") == 0) {
        search(train, node->ArgStr, out);
        return;
    }
    if(strcmp(node->command, "SEARCH_LEFT") == 0) {
        search_left(train, node->ArgStr, out);
        return;
    }
    if(strcmp(node->command, "SEARCH_RIGHT") == 0) {
        search_right(train, node->ArgStr, out);
        return;
    }
}