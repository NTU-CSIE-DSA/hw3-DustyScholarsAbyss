#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAXN 100005
#define MAXM 1000005

typedef struct node {
    int id;
    struct node *next;
} Node;

typedef struct edge {
    int u, v;
} Edge;

typedef struct graph{
    int n, m;
    Node **adj;
} Graph;

typedef struct component {
    int *list;
    int size;
} Component;

int n, m;
bool visited[MAXN];         // True if the vertex is visited
int artiCt = 0;           // The number of articulation points
bool isArti[MAXN];          // True if the vertex is an articulation point
Component components[MAXN]; // Store the components of the BCCs
int componentCt = 0;

int min(int a, int b){ return (a < b) ? a : b; }

int max(int a, int b){ return (a > b) ? a : b; }

int cmpInt(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

int cmpComponent(const void *a, const void *b){
    Component *A = (Component *)a;
    Component *B = (Component *)b;
    int i;
    for (i=0; i < A -> size && i < B -> size; i++){
        if (A -> list[i] != B -> list[i]){
            return A -> list[i] - B -> list[i];
        }
    }
    return A -> size - B -> size;
}

Graph *createGraph(){
    Graph *G = (Graph *) malloc(sizeof(Graph));
    assert(G != NULL);
    assert((G -> adj = (Node **) malloc(n * sizeof(Node *))) != NULL);
    for (int i=0; i<n; i++){
        assert((G -> adj[i] = (Node *) malloc(sizeof(Node))) != NULL);
        G -> adj[i] -> next = NULL;
        G -> adj[i] -> id = i;
    }
    return G;
}

void addEdge(Graph *G, int u, int v){
    Node *newNode = (Node *) malloc(sizeof(Node));
    assert(newNode != NULL);
    newNode -> id = v;
    newNode -> next = G -> adj[u] -> next;
    G -> adj[u] -> next = newNode;
}

void DFSVisit(Graph *G, int u){
    visited[u] = true;
    Node *tmp = G -> adj[u] -> next;
    while (tmp){
        int v = tmp -> id;
        if (!visited[v]){
            DFSVisit(G, v);
        }
        tmp = tmp -> next;
    }

    return ;
}

void ArtiPoints(Graph *G){
    // Print the articulation points
    memset(isArti, false, n * sizeof(bool));
    for (int u=0; u<n; u++){
        memset(visited, false, n * sizeof(bool));
        visited[u] = true;
        DFSVisit(G, (u + 1) % n);
        for (int j=0; j<n; j++){
            if (!visited[j]){
                artiCt++;
                isArti[u] = true;
                break;
            }
        }
    }
    
    return ;
}

void DFSVisitBridge(Graph *G, int u, const int blockU, const int blockV){
    visited[u] = true;
    Node *tmp = G -> adj[u] -> next;
    while (tmp){
        int v = tmp -> id;
        if ((u == blockU && v == blockV) || (u == blockV && v == blockU)){
            tmp = tmp -> next;
            continue;
        }
        if (!visited[v]){
            DFSVisitBridge(G, v, blockU, blockV);
        }
        tmp = tmp -> next;
    }

    return ;
}

void Bridge(Graph *G){
    int bridgeCt = 0;
    for (int u=0; u<n; u++){
        Node *tmp = G -> adj[u] -> next;
        while (tmp){
            memset(visited, false, n * sizeof(bool));
            int v = tmp -> id;
            if (u > v){
                tmp = tmp -> next;
                continue;
            }
            DFSVisitBridge(G, u, u, v);
            for (int j=0; j<n; j++){
                if (!visited[j]){
                    bridgeCt++;
                    if (isArti[u] && isArti[v]){
                        components[componentCt].list = (int *) malloc(2 * sizeof(int));
                        assert(components[componentCt].list != NULL);
                        components[componentCt].size = 2;
                        components[componentCt].list[0] = u;
                        components[componentCt].list[1] = v;
                        componentCt++;
                    }
                    break;
                }
            }
            tmp = tmp -> next;
        }
    }

    printf("%d\n", bridgeCt);
}

void DFSVisitBcc(Graph *G, int u, int *tmpArr, int *tmpCt){
    Node *tmp = G -> adj[u] -> next;
    while (tmp){
        int v = tmp -> id;
        if (!visited[v]){
            visited[v] = true;
            tmpArr[(*tmpCt)++] = v;
            if (!isArti[v]){
                DFSVisitBcc(G, v, tmpArr, tmpCt);
            }
        }
        tmp = tmp -> next;
    }

    return ;
}

void Bcc(Graph *G){
    // Print the BCCs
    memset(visited, false, n * sizeof(bool));
    int *tmpArr = (int *) malloc(n * sizeof(int));
    assert(tmpArr != NULL);
    int tmpCt = 0;
    for (int u=0; u<n; u++){
        if (!visited[u] && !isArti[u]){
            memset(tmpArr, -1, n * sizeof(int));
            tmpCt = 1;
            tmpArr[0] = u;
            visited[u] = true;
            DFSVisitBcc(G, u, tmpArr, &tmpCt);
            for (int i=0; i<tmpCt; i++){
                if (isArti[tmpArr[i]]){
                    visited[tmpArr[i]] = false;
                }
            }

            qsort(tmpArr, tmpCt, sizeof(int), cmpInt);
            components[componentCt].list = (int *) malloc(tmpCt * sizeof(int));
            assert(components[componentCt].list != NULL);
            components[componentCt].size = tmpCt;
            for (int i=0; i<tmpCt; i++){
                components[componentCt].list[i] = tmpArr[i];
            }
            componentCt++;
        }
    }

    printf("%d\n", componentCt);
    qsort(components, componentCt, sizeof(Component), cmpComponent);
    for (int i=0; i<componentCt; i++){
        for (int j=0; j<components[i].size; j++){
            printf("%d%c", components[i].list[j], (j == components[i].size - 1) ? '\n' : ' ');
        }
    }

    return ;
}

int main(){
    int type;
    assert(scanf("%d %d %d", &n, &m, &type) == 3);
    Graph *G = createGraph();
    for (int i=0; i<m; i++){
        int u, v;
        assert(scanf("%d %d", &u, &v) == 2);
        addEdge(G, u, v);
        addEdge(G, v, u);
    }
    ArtiPoints(G);
    if (type == 1){
        printf("%d\n", artiCt);
        for (int i=0; i<n; i++){
            if (isArti[i]){
                printf("%d ", i);
            }
        }
        printf("\n");
    }
    else if (type == 2){
        Bridge(G);
        Bcc(G);
    }
    else {
        perror("Error: Invalid type");
        exit(1);
    }

    return 0;
}