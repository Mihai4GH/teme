#include "tree.h"

Tree createTree(char label, int level) {
    Tree t = malloc(sizeof(Node));
    // Because we operate with label as a string we need to 
	// allocate 2 bytes of memory (1 for char. , 1 for string
	// terminator)
    t->label = malloc(sizeof(char) * 2);
    *(t->label) = label;
	*(t->label + 1) = '\0';
	// We initialize nr_childs with 0 and all the childs to NULL
    t->nr_childs = 0;
    memset(t->children, 0, 27 * sizeof(Tree));
	// Set the level of the new cell (for BFS)
	// The level helps with displaing the BFS
	// When moving to a higher level in BFS, we know to prin \n
	t->level = level;
	return t;
}

int get_idx(char chr) {
	// This function modularize the code
	// 0 -> '$', 1-26 -> 'a' - 'z'
    if(chr == '$')
        return 0;
    return 1 + chr - 'a';
}

void insertTree(Tree tree, char *str) { 
    // Recursiv function (can't be casted to a non initialized tree)
	// If it reaches the NULL charachter ('\0') it stops
    if (str[0] == 0) 
        return;
    char label = str[0];
    int children_idx = get_idx(label);
    if (tree->children[children_idx] == NULL) {
		// There isnt a common path so we create one
        tree->children[children_idx] = createTree(label, tree->level + 1);
		// We increment the number of childs
        tree->nr_childs++;
        insertTree(tree->children[children_idx], str+1);
        return;
    } else {
		// There is a common path
        insertTree(tree->children[children_idx], str+1);
        return;
    }
}

Tree destroyTree(Tree tree) {
	// This function frees the memory occupied by the whole tree
	if (tree) {
		for (int i = 0; i < 27; i++) {
			destroyTree(tree->children[i]);
		}
		free(tree->label);
		free(tree);
	}
	return NULL;
}

void insertWordSufixes(Tree tree, char *word) {
	// This function inserts in the tree all the sufixes of a word
	// First it concatenes to the words the sequence '$\0', then it 
	// iterates trough it's sufixes, from the shortest to the longhest
	int l = strlen(word);
	char *new_word = malloc(l + 2);
	strcpy(new_word, word);
	new_word[l] = '$';
	new_word[l + 1] = '\0';
	// Inserting all the sufixes
	for (int i = l; i >= 0; i--) {
		insertTree(tree, new_word + i);
	}
	free(new_word);
}

void bfsTree(Tree tree, FILE *out) {
	// In order to know when to move to a new line
	// we keep track of the level of each cell, starting
	// from level 1. (Level of first children)
	// For a BFS we are using a queue. 
	// Firstly we are enqueueing all the children of the root
	Queue q = initQueue();
	for (int i = 0; i < 27; i++) 
		if (tree->children[i])
			q = enqueue(q, tree->children[i]);
	int level = 1;
	while(!isEmptyQueue(q)) {
		Tree tmp = first(q);
		if(tmp->level != level)
			// We moved to a new level
			fprintf(out, "\n"); 
		fprintf(out, "%s ", tmp->label); 
		// Enqueueing all the existing children
		for (int i = 0; i < 27; i++) {
			if(tmp->children[i])
				enqueue(q, tmp->children[i]);
		}
		// Actualizating the curent level 
		level = tmp->level;
		q = deuqueue(q);
	}
	fprintf(out, "\n");
	q = freeQueue(q);
}

Tree buildTree(FILE *source, int nr_words) {
	// This function reads from source nr_words words and 
	// builds the respective sufix tree
	char *word = malloc(WORD_SIZE);
	// Initialize the tree
	Tree t = createTree(0, 0); 
	for (int i = 0; i < nr_words; i++) {
		fscanf(source, "%s", word);
		insertWordSufixes(t, word);
	}
	free(word);
	return t;
}

void dfs_c2(Tree t, int *leafs, int *k_leafs, int *max_childs, int k) {
	// This function evaluetes the paramatres needed for task-2
	// Because of the big number of children each cell has, we are only
	// add on the stack the childs that are different from NULL. 
	// (In order not to fill up the stack)
	if (t) { // Cazul in care e apelat pe un tree gol
		if(t->nr_childs == 0) {
			(*leafs)++;
			if(t->level == k)
				(*k_leafs)++;
		}
		if(t->nr_childs > (*max_childs))
			(*max_childs) = t->nr_childs;
		for(int i = 0; i < 27; i++) {
			if(t->children[i])
				dfs_c2(t->children[i], leafs, k_leafs, max_childs, k);
		}
	}
}

int checkSuffix(Tree tree, char *suf) {
	// This function checks if the sufix contained in *suf
	// is included in tree.
	// It returns 1 if is contained, otherwise 0.
	// !! this function assumes that *suf is terminated with '$'
	// (The cell with the label '$' doesnt have any childrens)
	if(*suf == 0)
		return 1;
	int idx = get_idx(*suf);
	if(tree->children[idx])
		return checkSuffix(tree->children[idx], suf+1);
	return 0;
}

void compress(Tree tree) {
	// This function compresses the tree.
	// Because of its nature it disrupts the levels of each cell.
	// After utilizing this function, it is need that the level 
	// of each cell of the tree is actualized.
	if(tree) {
		while (tree->nr_childs == 1 && !tree->children[0]) {
			// We found a cell that can be compressed.
			// We compress it untill it has either 2 or more childrens or it is 
			// followed by '$'
			tree->label = realloc(tree->label, strlen(tree->label) + 5);
			int i;
			for(i = 1; i < 27; i++) {
				if(tree->children[i])
					break;
			} 
			tree->label = strcat(tree->label, (const char *)tree->children[i]->label);
			Tree tmp = tree->children[i];
			// Pass the children's children and their number
			for(i = 0; i < 27; i++)
				tree->children[i] = tmp->children[i];
			tree->nr_childs = tmp->nr_childs;
			free(tmp->label);
			free(tmp);
		}
		// This functions is appealed recursively
		for (int i = 1; i < 27; i++) {
			if(tree->children[i])
				compress(tree->children[i]);
		}
	}
}

void dfs_actualize_level(Tree tree, int level) {
	// This function actualize the level of each cell
	// Should be applied to the root, wiht level = 0
	if (tree) {
		tree->level = level;
		for (int i = 0; i < 27; i++) {
			if (tree->children[i])
				dfs_actualize_level(tree->children[i], level + 1);
		}
	}
}

// Cerinta 1

void c1(FILE *source, FILE *dst) {
	// This function solves the first task
	int n;
	fscanf(source, "%d", &n);
	Tree tree = buildTree(source, n);
	bfsTree(tree, dst);
	destroyTree(tree);
}

// Cerinta 2

void c2(FILE *source, FILE *dst, int k) {
	// This function solves the seconds task
	int n;
	fscanf(source, "%d", &n);
	Tree tree = buildTree(source, n);
	int leafs = 0, k_leafs = 0, max_childs = 0;
	dfs_c2(tree, &leafs, &k_leafs, &max_childs, k + 1);
	fprintf(dst, "%d\n%d\n%d\n", leafs, k_leafs, max_childs);
	destroyTree(tree);
}

// Cerinta 3

void c3(FILE *source, FILE *dst) {
	// This function solves the thirds task
	int n, m;
	fscanf(source, "%d %d", &n ,&m);
	Tree tree = buildTree(source, n);
	char *suf = malloc(WORD_SIZE); 
	for (int i = 0; i < m; i++) {
		// It reads the sufix and adds '&' and '\0' to it
		// then it checks if it is contained inside the tree
		fscanf(source, "%s", suf);
		int l = strlen(suf);
		suf[l] = '$';
		suf[l + 1] = '\0';
		fprintf(dst, "%d\n", checkSuffix(tree, suf));
	}
	free(suf);
	destroyTree(tree);
}

void c4(FILE *source, FILE *dst) {
	// This function solves the fourth task 
	int n;
	fscanf(source, "%d", &n);
	Tree tree = buildTree(source, n);
	compress(tree);
	// After compressing we need to actualize the level of each cell
	// in order to use bfsTree corectly
	dfs_actualize_level(tree, 0);
	bfsTree(tree, dst);
	destroyTree(tree);
}


// Functii de desenare a tree-ului
void drawTreeHelper(Tree tree, FILE* stream) {
	Tree tmp;
	if (tree == NULL) {
		return;
	}
	if (tree->nr_childs == 0) {
		fprintf(stream, "    %ld[label=\"%s\", fillcolor=red]\n", (intptr_t) tree, tree->label);
	} else {
		fprintf(stream, "    %ld[label=\"%s\", fillcolor=yellow]\n", (intptr_t) tree, tree->label);
	}
	if (tree->children == NULL)
		return;
	int i;
	for (i = 0; i < 27; i++) {
		tmp = tree->children[i];
		if (tmp == NULL)
			continue;
		fprintf(stream, "    %ld -> %ld \n", (intptr_t) tree, (intptr_t) tmp);
		drawTreeHelper(tmp, stream);
	}
}

void drawTree(Tree tree, char *fileName) {
	FILE* stream = fopen("test.dot", "w");
	char buffer[SIZE];
	fprintf(stream, "digraph TRIE {\n");
	fprintf(stream, "    node [fontname=\"Arial\", shape=rectangle, style=filled, fillcolor=yellow];\n");
	if (!tree)
		fprintf(stream, "\n");
	else if (!tree->children)
		fprintf(stream, "    %ld [label=\"%s\"];\n", (intptr_t) tree, tree->label);
	else
		drawTreeHelper(tree, stream);
	fprintf(stream, "}\n");
	fclose(stream);
	sprintf(buffer, "dot test.dot | neato -n -Tpng -o %s", fileName);
	system(buffer);
}