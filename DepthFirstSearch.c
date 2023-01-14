#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
    char name[85];
    long weight;
    struct vertex* next;
}vertex;

void printADJ(vertex** adjList, long num){ // prints out all vertices and its edges in the adjacency list

    for(long i = 0; i < num; i++){
        vertex* current = adjList[i];
        while(current != NULL){
            printf("%s ", current->name);
            current = current->next;
        }
        printf("\n");
    }

}

long findIndex(vertex** adjList, char v[85], long num){ // find the index that a vertex is at

    for(long i=0; i<num; i++){
        if(strcmp(adjList[i]->name, v) == 0){
            return i;
        }
    }
    return -1;

}

void freeUp(vertex** adjList, long num){ // frees up the entire adjacency list

    for (int i = 0; i < num; i++){
        vertex* t = adjList[i];
        vertex* p;
        while (t->next != NULL)
        {
            p = t;
            t = t->next;
            free(p);
        }
        free(t);
    }
    free(adjList);
}

int exists(vertex** adjList, long index, char v[85]){ // checks to see if a char exists as an edge of a specific vertex

    for(vertex* current = adjList[index]; current->next != NULL; current = current->next){
        if(strcmp(current->name, v) == 0){
            return 1; // it exists
        }
    }
    return 0; // it does not exist

}

void addEdges(FILE* fp1, vertex** adjList, long num){ // takes input from graph.txt and adds edges accordingly
    char vert[85];
    char edge[85];
    long weight;
    while(fscanf(fp1, "%s %s %li\n", vert, edge, &weight) != EOF){
        vertex* newVertex = malloc(sizeof(vertex));
        strcpy(newVertex->name, edge);
        newVertex->weight = weight;
        newVertex->next = NULL;
        long index = findIndex(adjList, vert, num);
        if(exists(adjList, index, newVertex->name)==0){
            if(adjList[index]->next==NULL){
                adjList[index]->next = newVertex; 
            }else{
                vertex* current = adjList[index];
                while(current->next != NULL && (strcmp(current->next->name, newVertex->name) < 0)){
                    current = current->next;
                }
                newVertex->next = current->next;
                current->next = newVertex;
            }
        }else{
            printf("error");
        }
    }
}

void adjacentVertices(vertex** adjList, char v[85], long num){ // prints out all the vertices adjacent to a given vertex

    long index = findIndex(adjList, v, num);
    vertex* vert = adjList[index];

    vert = vert->next;
    while(vert != NULL){
        if (vert->next==NULL){
            printf("%s", vert->name); // this makes sure that there is no trailing space after the last name[] printed
        }else{
            printf("%s ", vert->name);
        }
        vert = vert->next;
    }
    printf("\n");
    
}

void traverse(vertex* root){ // traverse through the linked list and print out each vertex name
    if(root != NULL){
        for(vertex* printing = root; printing!=NULL; printing = printing->next){
            printf("%s ", printing->name);
        }
    }
    printf("\n");
}

vertex* enqueue(vertex* root, char name[85*sizeof(char)]){
    vertex* newVert = malloc(sizeof(vertex));
    newVert->next = NULL;
    strcpy(newVert->name, name);
    if(root == NULL){
        root = newVert;
    }else{
        vertex* start = root;
        while(start->next != NULL){
            start = start->next;
        }
        start->next = newVert;
    }
    return root;
}

int listContains(vertex* root, char v[85*sizeof(char)]){ // checks to see if a linked list conatains a certain vertex or not
    vertex* current = root;
    while(current != NULL){
        if(strcmp(current->name, v)==0){
            return 1; // it is in the linked list
        }
        current = current->next;
    }
    return 0; // it is not in the linked list
}

vertex* dfs(vertex** adjList, long index, vertex* vis, long num){
    vis = enqueue(vis, adjList[index]->name);
    vertex* curr = adjList[index]->next;
    while((curr != NULL)){
        if(listContains(vis, curr->name)==1){
            curr = curr->next;
        }else{
            long id = findIndex(adjList, curr->name, num);
            dfs(adjList, id, vis, num);
        }
    }
    return vis;
}

void doDFS(vertex** adjList, long num){
    vertex* visited = NULL;
    for(long i=0; i<num; i++){
        if(listContains(visited, adjList[i]->name)==0){
            visited = dfs(adjList, i, visited, num);
        }
    }
    traverse(visited);

    while(visited!=NULL){
        vertex* nex = visited->next;
        free(visited);
        visited = nex;
    }
}

void sort(vertex** adj, long num){
    for(long i=0; i<num-1; i++){
        for(long j=0; j<num-i-1; j++){
            if(strcmp(adj[j]->name, adj[j+1]->name)>0){
                char temp[85*sizeof(char)];
                strcpy(temp, adj[j]->name);
                strcpy(adj[j]->name, adj[j+1]->name);
                strcpy(adj[j+1]->name, temp); 
            }
        }
    }
}

int main(int argc, char* argv[argc+1]){
    long num;
    FILE* fp1 = fopen(argv[1], "r");
    if(fp1 == NULL){
        printf("error");
        return 1;
    }else{
        fscanf(fp1, "%li\n", &num);
    }
    vertex** adjList = malloc(num*sizeof(vertex*));
    for(long i=0; i<num; i++){
        char str[85];
        fscanf(fp1, "%s\n", str);
        adjList[i] = malloc(sizeof(vertex));
        strcpy(adjList[i]->name, str);
        adjList[i]->next = NULL;
        adjList[i]->weight = 0;
    }
    sort(adjList, num);
    addEdges(fp1, adjList, num);
    //printADJ(adjList, num);
    doDFS(adjList, num);
    fclose(fp1);
    freeUp(adjList, num);
    return 0;
}
