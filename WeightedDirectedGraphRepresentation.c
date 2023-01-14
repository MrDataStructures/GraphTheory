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

long outDegree(vertex** adjList, char v[85], long num){ // Determines the out-degree of a given vertex

    long count = 0;
    long index = findIndex(adjList, v, num);
    vertex* one = adjList[index]->next;
    while(one != NULL){
        count++;
        one = one->next;
    }
    return count;

}

long inDegree(vertex** adjList, char v[85], long num){ // Determines the in-degree of a given vertex
    long count = 0;
    for(long i=0; i<num; i++){

        vertex* current = adjList[i];
        while(current != NULL){
            if(strcmp(current->name,v)==0){
                count++;
            }
            current = current->next;
        }

    }
    return count-1;
}

int main(int argc, char* argv[argc+2]){
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
    addEdges(fp1, adjList, num);
    //printADJ(adjList, num);
    fclose(fp1);

    char query;
    char title[85];
    FILE* fp2 = fopen(argv[2], "r");
    if(fp2 == NULL){
        printf("error");
        return 1;
    }else{
        while(fscanf(fp2, "%c %s\n", &query, title) != EOF){
            if(query == 'o'){
                long out = outDegree(adjList, title, num);
                printf("%li\n", out);
            }else if(query == 'a'){
                adjacentVertices(adjList, title, num);
            }else{
                long in = inDegree(adjList, title, num);
                printf("%li\n", in);
            }
        }
    }

    freeUp(adjList, num);
    fclose(fp2);
    return 0;
}
