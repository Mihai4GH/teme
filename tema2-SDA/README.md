***The core functionality***

**C1**: the main functions of this task are the one that build the tree and the one that displays it. The dfsTree function is used to also display the compressed tree, so it treats each character as a string.

**C2**: In order to find the required parametres, I opted for a DFS, that checks each cell based of the following prorpieties:
    - each leaf has the nr_childs set to 0;
    - each leaf of lenght K has the level set to K+1 (The '$' is not counted as part of the sufix);
    - max_nr_childs is greater than all nr_childs of the other cells.

**C3**: Firstly we are building the tree, then we are checking each sufix.

**C4**: Firstly we are building the tree, then we compress it. Befor BFS, it is need that the level of each cell are actualized.