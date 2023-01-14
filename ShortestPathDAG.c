#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex{
    char name[85];
    long weight;
    int idx;
    struct vertex* next;
}vertex;


void freeUp(vertex** adjList, long num){
    for(int i = 0; i < num; i++){
        vertex *t = adjList[i], *p;
        while(t->next != NULL){
            p = t;
            t = t->next;
            free(p);
        }
        free(t);
    }
    free(adjList);
}


void merge(char a[][85], int l, int m, int r){
    int n1 = m - l + 1, n2 = r - m;
    char left[n1][85], right[n2][85];
 
    for(int i = 0; i < n1; i++) strcpy(left[i], a[l + i]);
    for(int j = 0; j < n2; j++) strcpy(right[j], a[m + 1 + j]);
 
    int i = 0, j = 0,  k = l;
    while(i < n1 && j < n2)
        if(strcmp(left[i], right[j]) <= 0) strcpy(a[k++], left[i++]);
        else strcpy(a[k++], right[j++]);
 
    while(i < n1) strcpy(a[k++], left[i++]);
    while(j < n2) strcpy(a[k++], right[j++]);
}
 

void sort(char arr[][85], int l, int r){
    if(l < r){
        int m = l + (r - l) / 2;
        sort(arr, l, m);
        sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


vertex * add(vertex *first, char u[], int idx, long w){
    if(first != NULL && strcmp(first->name, u) == 0) return first;

    vertex *node = malloc(sizeof(vertex));
    node->idx = idx;
    node->next = NULL;
    strcpy(node->name, u);
    node->weight = w;
   
    if(first == NULL || strcmp(first->name, u) > 0){
        node->next = first;
        first = node;
        return first;
    }

    vertex *cur = first;
    while(cur->next != NULL && strcmp(cur->next->name, u) < 0) cur = cur->next;
    if(cur->next != NULL && !strcmp(cur->next->name, u)){
        free(node);
        return first;
    }
    
    node->next = cur->next;
    cur->next = node;
    return first;
}


typedef struct node{
    int key;
    struct node* next;
} node;

typedef struct Queue{
    node *front, *rear;
} Queue;


node * newNode(int k){
    node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = k;
    temp->next = NULL;
    return temp;
}


Queue* getQueue(){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void push(Queue* q, int k){
    node* temp = newNode(k);
    if(q->rear == NULL){
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

void pop(Queue* q){
    if(q->front == NULL) return;
    node* temp = q->front;
    q->front = q->front->next;
    if(q->front == NULL) q->rear = NULL;
    free(temp);
}

int toposort(vertex **adjList, int ts[], int n){
    int in[n];
    for(int i = 0; i < n; i++) in[i] = 0;

    for(int u = 0; u < n; u++){
        vertex *c = adjList[u]->next;
        while(c != NULL)
            in[c->idx]++,
            c = c->next;
    }

    Queue *q = getQueue();
    for(int i = 0; i < n; i++) if(in[i] == 0) push(q, i);
 
    int cnt = 0, i = 0;
    while(q->rear != NULL){
        int u = q->front->key;
        pop(q);
        ts[i++] = u;
        vertex *cur = adjList[u]->next;
        while(cur != NULL){
            if(--in[cur->idx] == 0) push(q, cur->idx);
            cur = cur->next;
        }
        cnt++;
    }
 
    free(q);
    if(cnt != n) return 1;
    return 0;
}

const long oo = 2147483640;

void go(vertex **adj, long d[], int ts[], int src, int n){
    for(int i = 0; i < n; i++) d[i] = oo;
    d[src] = 0;
    for(int i = 0; i < n; i++){
        int u = ts[i];
        vertex *c = adj[u]->next;
        while(c != NULL){
            int v = c->idx;
            if(d[u] != oo && d[v] > (c->weight + d[u])) d[v] = d[u] + c->weight;
            c = c->next;
        }
    }
}

int main(int argc, char* argv[argc+1]){
    int n;
    FILE *graphf = fopen(argv[1], "r");
    FILE *queriesf = fopen(argv[2], "r");

    if(graphf == NULL || queriesf == NULL){
        printf("error");
        exit(EXIT_SUCCESS);
    }

    fscanf(graphf, "%d\n", &n);
    
    char vertices[n][85];
    vertex** adjList = malloc(n * sizeof(vertex*));
    
    for(long i = 0; i < n; i++) fscanf(graphf, "%s", vertices[i]);
    
    sort(vertices, 0, n - 1);

    for(long i = 0; i < n; i++){
        adjList[i] = malloc(sizeof(vertex));
        strcpy(adjList[i]->name, vertices[i]);
        adjList[i]->next = NULL;
        adjList[i]->weight = 0;
    }

    char u[85], v[85];
    long w;
    while(fscanf(graphf, "%s %s %li", u, v, &w) != EOF){
        int j = -1, k = -1;
        for(int i = 0; i < n; i++){
            if(!strcmp(adjList[i]->name, u)) j = i;
            if(!strcmp(adjList[i]->name, v)) k = i;
        }
        adjList[j]->next = add(adjList[j]->next, v, k, w);
    }

    int topological_order[n];
    int c = toposort(adjList, topological_order, n);
    long d[n];
    if(c)
        printf("CYCLE\n");
    
    while(fscanf(queriesf, "%s", u) != EOF){
        if(!c){
            printf("\n");
            int s;
            for(int i = 0; i < n; i++)
                if(!strcmp(adjList[i]->name, u)){
                    s = i;
                    break;
                }
            go(adjList, d, topological_order, s, n);
            for(int i = 0; i < n; i++)
                if(d[i] != oo) printf("%s %li\n", adjList[i]->name, d[i]);
                else printf("%s INF\n", adjList[i]->name);
        }
    }
    if(!c) printf("\n");

    fclose(graphf);
    fclose(queriesf);
    freeUp(adjList, n);
    return 0;
}
