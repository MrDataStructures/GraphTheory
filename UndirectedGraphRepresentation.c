#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
    char letter[85 * sizeof(char)];
    long listIndex; // index of the vertex in the graph
    long size; // number of edges a vertex has after the adjlist has been made and is complete
    struct vertex* next;
}vertex;

void printADJ(vertex** adjList, long num){ // prints out all vertices and its edges in the adjacency list

    for(long i = 0; i < num; i++){
        vertex* current = adjList[i];
        while(current != NULL){
            printf("%s ", current->letter);
            current = current->next;
        }
        printf("\n");
    }

}

long findIndex(vertex** adjList, char v[85 * sizeof(char)], long num){ // find the index that a vertex is at

    for(long i=0; i<num; i++){
        if(strcmp(adjList[i]->letter, v) == 0){
            return i;
        }
    }
    return -1;

}

long degreeVertex(vertex** adjList, char v[85 * sizeof(char)], long num){ // Determines the degree of a vertex

    long count = 0;
    long index = findIndex(adjList, v, num);
    vertex* one = adjList[index]->next;
    while(one != NULL){
        count++;
        one = one->next;
    }
    return count;

}

void adjacentVertices(vertex** adjList, char v[85 * sizeof(char)], long num){ // prints out all the vertices adjacent to a given vertex

    long index = findIndex(adjList, v, num);
    vertex* vert = adjList[index];

    vert = vert->next;
    while(vert != NULL){
        if (vert->next==NULL){
            printf("%s", vert->letter); // this makes sure that there is no trailing space after the last letter[] printed
        }else{
            printf("%s ", vert->letter);
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

int exists(vertex** adjList, long index, char v[85 * sizeof(char)]){ // checks to see if a char exists as an edge of a specific vertex

    for(vertex* current = adjList[index]; current->next != NULL; current = current->next){
        if(strcmp(current->letter, v) == 0){
            return 1; // it exists
        }
    }
    return 0; // it does not exist

}

void addEdges(FILE* fp1, vertex** adjList, long num){ // adds edges to the correct vertices given a text file

    char one[85 * sizeof(char)];
    char two[85 * sizeof(char)];
    while(fscanf(fp1, "%s %s\n", one, two) != EOF){
        vertex* newVertex = malloc(sizeof(vertex));           // this part maps the second vertex as an edge to the first one
        strcpy(newVertex->letter, two);
        newVertex->next = NULL;
        newVertex->listIndex = findIndex(adjList, two, num);
        long index = findIndex(adjList, one, num); // 0
        if(exists(adjList, index, newVertex->letter)==0){
            if(adjList[index]->next==NULL){
                adjList[index]->next = newVertex; 
            }else{
                vertex* current = adjList[index];
                while(current->next != NULL && (strcmp(current->next->letter, newVertex->letter) < 0)){
                    current = current->next;
                }
                newVertex->next = current->next;
                current->next = newVertex;
            }
        }else{
            printf("error");
        }

        vertex* new = malloc(sizeof(vertex));                 // this part maps the first vertex as an edge of the second one. If A->B, then B->A
        strcpy(new->letter, one);
        new->next = NULL;
        new->listIndex = findIndex(adjList, one, num);
        long id = findIndex(adjList, two, num);
        if(exists(adjList, id, new->letter)==0){
            if(adjList[id]->next==NULL){
                adjList[id]->next = new; 
            }else{
                vertex* current = adjList[id];
                while(current->next != NULL && (strcmp(current->next->letter ,new->letter) < 0)){
                    current = current->next;
                }
                new->next = current->next;
                current->next = new;
            }
        }else{
            printf("error");
        }

    }

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

int main(int argc, char* argv[argc+2]){

    long num;
    FILE * fp1 = fopen(argv[1], "r");
    if(fp1 == NULL){
        printf("error\n");
        return 1;
    }else{
        fscanf(fp1, "%li\n", &num);
    }
    vertex** adjList = malloc(num*sizeof(vertex*));
    for(long i=0; i<num; i++){
        char str[81];
        fscanf(fp1, "%s\n", str);
        adjList[i] = malloc(sizeof(vertex)); // this code takes the first number n in graph.txt and iterates n times and populates the vertices in the adj list. 
        strcpy(adjList[i]->letter, str);
        adjList[i]->next = NULL;
        adjList[i]->listIndex = i;
    }

    addEdges(fp1, adjList, num);    // adds all the edges to the adjlist from graph.txt

    for(long f = 0; f < num; f++){
        vertex* c = adjList[f];
        while(c != NULL){
            c->size = outDegree(adjList, c->letter, num);  // after you have added all the edges to the adjlist, this for loop sets the size (outdegree) of every vertex in the list, even if it is an edge
            c = c->next;        // now, if you wanted to get the size (outdegree) of any vertex, you can do something like long out = adjList[2]->next->size;
        }
    }

    //printADJ(adjList, num);

    fclose(fp1);
    
    char query;
    char vertex[85 * sizeof(char)];
    long count;
    FILE * fp2 = fopen(argv[2], "r");
    if(fp2 == NULL){
        printf("error\n");
        return 1;
    }else{
        while(fscanf(fp2, "%c %s\n", &query, vertex) != EOF){
            if(query == 'd'){
                count = degreeVertex(adjList, vertex, num);
                printf("%li\n", count);
            }else{
                adjacentVertices(adjList, vertex, num);
            }
        }
    }

    freeUp(adjList, num);
    fclose(fp2);
    return 0;
}
