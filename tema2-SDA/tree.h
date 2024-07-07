#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "queue.h"


typedef struct node {
    struct node *children[27];
    char nr_childs;
    char *label;
    int level;
} Node, *Tree;


#define SIZE 1000
#define WORD_SIZE 30

// Functii pentru realizarea structurii arborelui

Tree createTree(char label, int level);
void insertTree(Tree tree, char *str);
Tree destroyTree(Tree tree);
void insertWordSufixes(Tree tree, char *word);
void bfsTree(Tree tree, FILE *out);
Tree buildTree(FILE *source, int nr_words);
int checkSuffix(Tree tree, char *suf);
void compress(Tree tree);

// Taskuri

void c1(FILE *source, FILE *dst);
void c2(FILE *source, FILE *dst, int k);
void c3(FILE *source, FILE *dst);
void c4(FILE *source, FILE *dst);

// Functii de desenare a arborelui
void drawTreeHelper(Tree tree, FILE* stream);
void drawTree(Tree tree, char *fileName);

#endif