***The core functionality***

**Task 1**
In order for this program to work i simulated a stack with a vector of char of max lenght 100 ("stack db 100, 0"). Also I used a function "get_coresponding" that will return the corespunding open pharantheses of a close one. Ex: for ')' it will return '(', for ']', it will return '['.
As a counter for the number of elements contained in the stack, I used the register ecx.
For this program to work, on the stack there will only be open pharantheses, and the close ones will remove from the stack the open ones. Any conflict will result in a bad paranthesing.

**Task 2**

*Subtask 1*
The code i used for quicksort follows this one in C:

void QuickSort(int v[], int st, int dr)
{
	if(st < dr)
	{
		//pivotul este inițial v[st]
		int m = (st + dr) / 2;
		int aux = v[st];
		v[st] = v[m];
		v[m] = aux;
		int i = st , j = dr, d = 0;
		while(i < j)
		{
			if(v[i] > v[j])
			{
				aux = v[i]; 
				v[i] = v[j];
				v[j] = aux;
				d = 1 - d;
			}
			i += d;
			j -= 1 - d;
		}
		QuickSort(v, st , i - 1);
		QuickSort(v, i + 1 , dr);
	}
}

*Subtask 2*
PSEUDOCODE:
if start < end
    return -1
m = (start+end)/2
if v[m] == needle
    return m
if v[m] < needle
    bsearch(buff, m+1, dr)
if v[m] > needle
    bsearch(buff, st, m-1)

**Task 3**
PSEUDOCODE:
if(visited[node] = 0) {
    visited[node] <- 1;
    print(node)
    for (u = neighbours(node))
        dfs(u, expand)
}