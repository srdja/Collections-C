#include <stdio.h>
#define TRUE (1==1)

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) size: size of the array of elements
	
	Set the roots of each element as itself
*/

void initiateSet (int *set, int size){
	int i;
	for (i=0; i < size; i++) set[i]=i;
	return;
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) pos: position of the element you want to know it's root
	
	Return the root of the element in the position you want
*/

int root (int *set, int pos){
	int root;
	for (root = set[pos]; root != set[root]; root = set[root]);
	return root;
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) pos1: position of one of the elements you want to unite
	@ (int) pos2: position of the other element you want to unite with
	
	Unite two elements of the set by setting the root of the first as the root of the other
*/

void unite (int *set, int pos1, int pos2){
	set[root(set, pos1)] = root(set, pos2);
	return;
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) size: size of the array of elements
	
	Set each element's root to the primary root it's linked in
*/

void compress (int *set, int size){
	int i;
	for (i=0;i < size; i++) set[i] = root (set, i);
	return;
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) size: size of the array of elements
	
	Print the direct connection of each element in the set
*/

void printConnections (int *set, int size){
	int i;
	printf ("Connections:\n");
	for (i=0;i<size;i++)printf ("%d ", i);
	printf ("\n");
	for (i=0;i<size;i++)printf ("%d ", set[i]);
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) size: size of the array of elements
	
	Print the primary root of each element
*/

void printRoots (int *set, int size){
	int i;
	printf ("Connections:\n");
	for (i=0;i<size;i++)printf ("%d ", i);
	printf ("\n");
	for (i=0;i<size;i++)printf ("%d ", root(set,i));
}

/*
	parameters:
	@ (int*) set: array with the elements
	@ (int) size: size of the array of elements
	
	Return the number of sets (or primary roots) in the array
*/

int numberSets(int *set, int size){
	int i, count=0;
	for (i=0;i<size;i++) if (set[i] == i) count++;
	return count;
}

/*
	Example of application of the union-find
*/

int main (){
	int *set, SIZE, option, a, b;
	
	printf("\n\nUNION - FIND\n\nInput the set's number of elements:");
	scanf("%d",&SIZE);
	
	set = (int*) malloc(SIZE*sizeof(int));
	
	initiateSet (set, SIZE);
	
	while (TRUE){
		printf ("\n\n1 Unite two positions\n2 Find the root of a position\n3 Copress set\n4 Print the connections of the positions\n5 Print the root of the positions\n6 Number of sets\n0 Close Program\n\n");
		scanf ("%d", &option);
		printf ("\n\n");
		switch (option){
			case 0:
				free(set);
				return 0;
			case 1:
				printf ("Input two positions to unite:\n");
				scanf ("%d %d", &a, &b);
				unite (set, a, b);
				printf ("%d and %d united!",a,b);
				break;
			case 2:
				printf ("Input the position which you want to find the root:\n");
				scanf ("%d",&a);
				printf ("Root of %d = %d",a, root(set, a));
				break;
			case 3:
				compress (set, SIZE);
				printf ("Set compressed!");
				break;
			case 4:
				printConnections(set, SIZE);
				break;
			case 5:
				printRoots(set, SIZE);
				break;
			case 6:
				printf ("There are a total of %d subsets in the set", numberSets(set, SIZE));
				break;
			default:
				break;
		}
	}
}
