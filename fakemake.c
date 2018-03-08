#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"


#define WHITE 0
#define GREY 1
#define BLACK 2


typedef struct node {
	char* name;
	int timestamp;
	int visited;
	int basic;
	int pending;
	int numDep;
	struct node** dependencies;
} NODE;

// displays the adjacency list
void display_list(NODE** list, int size) {
	int outer = 0;
	int inner = 0;
	for(outer = 0; outer < size; outer++) {
		printf("%s -> ", list[outer]->name);
		for(inner = 0; inner < list[outer]->numDep; inner++) {
			printf("%s -> ", list[outer]->dependencies[inner]->name);
		}
		printf("\n");
	}
}

// recursive depth-first search checking for cycles
void dfs_r(NODE **list, int src, int *labels, HMAP* graph) {
	NODE **p;
	int v;
	labels[src] = GREY;
	p = list[src]->dependencies;
	int count = 0;
	for(count = 0; count < list[src]->numDep; count++) {
		v = hmap_get(graph, p[count]->name);
		if(labels[v] == WHITE) {
			dfs_r(list, v, labels, graph);
		} else if(labels[v] == GREY) {
			fprintf(stderr, "Loop detected... aborting program\n");
			abort();
		}
	}
	labels[src] = BLACK;
}

// helper function for dfs_r
void dfs(NODE **adjList, int size, int src, HMAP* graph) {
	int *labels = malloc(sizeof(int)*size);

	int v = 0;
	for(v=0;v<size;v++) {
		labels[v] = WHITE;
	}
	dfs_r(adjList, src, labels, graph);
	free(labels);
}

// similar to dfs_r, updating dependencies instead of checking for cycles
int make_r(NODE **list, int src, int *labels, HMAP* graph, int *time) {
	NODE **p;
	int v;
	int depTime = -1;
	int update = 0;
	labels[src] = GREY;
	p = list[src]->dependencies;
	int count = 0;
	for(count = 0; count < list[src]->numDep; count++) {
		v = hmap_get(graph, p[count]->name);
		if(labels[v] == WHITE) {
			depTime = make_r(list, v, labels, graph, time);
			if(depTime > list[src]->timestamp) {
				update = 1;
			}
		} else {
			if(list[v]->timestamp > list[src]->timestamp) {
				update = 1;
			}
		}
	}
	if(update){
		list[src]->timestamp = (*time)++;
		printf("Making %s...done\n", list[src]->name);
	} else {
		if(!list[src]->basic) {
			printf("%s is up to date\n", list[src]->name);
		}
	}
	labels[src] = BLACK;
	return list[src]->timestamp;
}

void make(NODE **adjList, int size, int src, HMAP* graph, int *time) {
	int *labels = malloc(sizeof(int)*size);

	int v = 0;
	for(v=0;v<size;v++) {
		labels[v] = WHITE;
	}
	make_r(adjList, src, labels, graph, time);

	free(labels);
}

int main(int argc, char **argv) {
	int done = 0;
	char buf[128];
	// adjacency list referenced by array index from hashmap number
	NODE** adjacency = malloc(sizeof(NODE*) * 1000);

	// create hashmap and set function
	HMAP *graph = hmap_create(0, 1.0);

	// does file open
	FILE *f = fopen(argv[1], "r");
	if(f == NULL) {
		perror("ERROR");
		return 1;
	}
	
	// parse info out of file
	char string[100];
	int adj = 0;
	int dep = 0;
	int nodeNum;
	while(fgets(string, 100, f)) {
		// 1st call to strtok; gets first filename
		char *filename = strtok(string, " \n");
		// if nothing on the line, break the loop
		if(filename == NULL) {
			break;
		}

		NODE* nameNode;
		if(!hmap_contains(graph, filename)) {
			// create a new node based on the target value
			nameNode = (NODE*)malloc(sizeof(NODE));
			// allocate space for the name of the file
			nameNode->name = malloc(sizeof(char) * 100);
			// allocate space for array of NODE dependencies
			nameNode->dependencies = malloc(sizeof(NODE*) * 1000);
			// copy the name of the file into the node
			strcpy(nameNode->name, filename);
			// timestamp default value
			nameNode->timestamp = 0;
			// visited field
			nameNode->visited = WHITE;
			// set pending to 0
			nameNode->pending = 0;
			// num dependencies
			nameNode->numDep = 0;
			// add address of nameNode into adjacency list
			adjacency[adj] = nameNode;
			// hash the index value of filename node in the adjacency list
			hmap_set(graph, filename, adj);
			adj++;
		} else {
			nameNode = adjacency[hmap_get(graph, filename)];
			nameNode->pending = 0;
			nameNode->basic = 0;
			// allocate space for array of NODE dependencies
			nameNode->dependencies = malloc(sizeof(NODE*) * 1000);
			nameNode->numDep = 0;
		}

		// 2nd call to strtok; gets sexcond filename
		char* dependFile = strtok(NULL, " :\n");
		// only one file on line; basic file
		if(dependFile == NULL) {
			nameNode->basic = 1;
		}

		dep = 0;
		NODE* dependency;
		while(dependFile != NULL) {
			// if the hmap doesn't have node; add to adj and hmap
			if(!hmap_contains(graph, dependFile)) {
				// allocate memory for node and name
				dependency = (NODE*)malloc(sizeof(NODE));
				dependency->name = malloc(sizeof(char) * 100);
				// copy name of dependency over to new node
				strcpy(dependency->name, dependFile);
				// set timestamp to 0
				dependency->timestamp = 0;
				// set visited to WHITE
				dependency->visited = WHITE;
				// set dependency of nameNode
				nameNode->dependencies[dep] = dependency;
				dep++;
				// update depenencies
				nameNode->numDep++;
				// set as basic for default (if not, will catch when found)
				dependency->basic = 1;
				// set dependency file as pending since not found
				dependency->pending = 1;
				// add dependency to the adjacency list as own file
				adjacency[adj] = dependency;
				// hash the new file (will term later if not a valid file)
				hmap_set(graph, dependFile, adj);
				adj++;
			} else {
				// get address from adjacency list
				dependency = adjacency[hmap_get(graph, dependFile)];
				// set dependency of nameNode
				nameNode->dependencies[dep] = dependency;
				dep++;
				nameNode->numDep++;
			}
			dependFile = strtok(NULL, " :\n");
		}
	}

	int test = 0;
	for(test = 0; test<adj; test++) {
		dfs(adjacency, adj, test, graph);
	}

	// input handling
	char *cmd;
	char *file;
	int time = 1;
	int tmp = -1;
	while(!done) {
		if(fgets(buf, 127, stdin) != NULL) {
			cmd = strtok(buf, " \n");
			file = strtok(NULL, " \n");

			if(strcmp("time", cmd)==0) {
				printf("%d\n", time);
			}
			else if(strcmp("touch", cmd)==0) {
				if(file != NULL && hmap_contains(graph, file)) {
					tmp = hmap_get(graph, file);
					adjacency[tmp]->timestamp = time++;
				} else {
					printf("File name not valid\n");
				}
			}
			else if(strcmp("timestamp", cmd)==0) {
				
				if(file != NULL && hmap_contains(graph, file)) {
					tmp = hmap_get(graph, file);
					printf("%d\n", adjacency[tmp]->timestamp);
				} else {
					printf("File name not valid\n");
				}
			}
			else if(strcmp("timestamps", cmd)==0) {
				// print all timestamps
				int j=0;
				for(j = 0; j < adj; j++) {
					
					printf("%s: %d\n", adjacency[j]->name, adjacency[j]->timestamp);
				}
			}
			else if(strcmp("make", cmd)==0) {
				// make command
				if(file != NULL && hmap_contains(graph, file)) {
					tmp = hmap_get(graph, file);
					make(adjacency, adj, tmp, graph, &time);
				} else {
					printf("File name not valid\n");
				}
			}
			else if(strcmp("quit", cmd)==0) {
				printf("Ending program...\n");
				return 0;
			}
			else if(strcmp("d", cmd)==0) {
				display_list(adjacency, adj);
			} else {
				printf("Invalid command.\n");
			}
		}
	}
	
	return 0;
}
