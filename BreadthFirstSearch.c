#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
    char letter[85 * sizeof(char)];
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

long numberOfEdges(vertex** adjList, char v[85 * sizeof(char)], long num){ // returns the number of edges a given vertex has

    long count = 0;
    long index = findIndex(adjList, v, num);
    vertex* vert = adjList[index];

    vert = vert->next;
    while(vert != NULL){
        count++;
        vert = vert->next;
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
}

vertex* enqueue(vertex* root, char name[85*sizeof(char)]){
    vertex* newVert = malloc(sizeof(vertex));
    newVert->next = NULL;
    strcpy(newVert->letter, name);
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

vertex* dequeue(vertex* root){
    if(root != NULL){
        vertex* curr = root;

        if(root->next == NULL){
            free(root);
            return NULL;
        }else{
            root = root->next;
            free(curr);
        }
    }
    return root;
}

int listContains(vertex* root, char v[85*sizeof(char)]){ // checks to see if a linked list conatains a certain vertex or not
    vertex* current = root;
    while(current != NULL){
        if(strcmp(current->letter, v)==0){
            return 1; // it is in the linked list
        }
        current = current->next;
    }
    return 0; // it is not in the linked list
}

void traverse(vertex* root){ // traverse through the linked list and print out each vertex name
    if(root != NULL){
        for(vertex* printing = root; printing!=NULL; printing = printing->next){
            printf("%s ", printing->letter);
        }
    }
    printf("\n");
}

void bfs(vertex** adjList, char v[85*sizeof(char)], long num){
    vertex* visited = NULL;
    visited = enqueue(visited, v);

    vertex* queue = NULL;

    long index = findIndex(adjList, v, num);
    vertex* curr = adjList[index]->next;
    while(curr != NULL){
        queue = enqueue(queue, curr->letter);
        curr = curr->next;
    }
    
    while(queue != NULL){
        long id = findIndex(adjList, queue->letter, num);
        visited = enqueue(visited, queue->letter);
        queue = dequeue(queue);
        vertex* after = adjList[id]->next;
        while(after != NULL){
            if((listContains(queue, after->letter)==0) && (listContains(visited, after->letter)==0)){
                queue = enqueue(queue, after->letter);
            }
            after = after->next;
        }
    }

    traverse(visited);
    //traverse(queue);
    
    while(queue!=NULL){
        vertex* next = queue->next;
        free(queue);
        queue = next;
    }

    while(visited!=NULL){
        vertex* nex = visited->next;
        free(visited);
        visited = nex;
    }
    
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
        adjList[i] = malloc(sizeof(vertex));
        strcpy(adjList[i]->letter, str);
        adjList[i]->next = NULL;
    }
    addEdges(fp1, adjList, num);
    //printADJ(adjList, num);

    fclose(fp1);

    char title[85*sizeof(char)];
    FILE* fp2 = fopen(argv[2], "r");
    if(fp2 == NULL){
        printf("error");
    }else{
        while(fscanf(fp2, "%s\n", title) != EOF){
            bfs(adjList, title, num);
        }
    }

    fclose(fp2);
    freeUp(adjList, num);
    return EXIT_SUCCESS;
}
