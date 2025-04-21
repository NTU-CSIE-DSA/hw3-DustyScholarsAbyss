#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

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
int timeIn[MAXN];           // Record the time when the vertex is visited 
int low[MAXN];              // The lowest timeIn of the vertex in the DFS tree, low[u] = min(timeIn[u], low[v], timeIn[w]) for all children v of u and back edges (u, w)
int parent[MAXN];           // The parent of the vertex in the DFS tree, -1 if it is the root
int time = 0;
Edge edgeStack[MAXM];       // Stack to store the edges in the DFS tree
int edgeStackTop = -1;
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

void pushEdgeStack(int u, int v){
    edgeStackTop++;
    edgeStack[edgeStackTop].u = u;
    edgeStack[edgeStackTop].v = v;
}

void constructBcc(int u, int v){
    int *tmpArr = (int *) malloc((edgeStackTop + 1) * sizeof(int));
    assert(tmpArr != NULL);
    int tmpCt = 0;
    bool *used = (bool *) calloc(MAXN, sizeof(bool));
    assert(used != NULL);

    // All the vertices in the stack above the edge (u, v) are part of the same BCC
    // So we can just pop the stack until we find it
    while (edgeStackTop >= 0){
        Edge e = edgeStack[edgeStackTop--];
        if (!used[e.u]){
            used[e.u] = true;
            tmpArr[tmpCt++] = e.u;
        }
        if (!used[e.v]){
            used[e.v] = true;
            tmpArr[tmpCt++] = e.v;
        }
        if ((e.u == u && e.v == v) || (e.u == v && e.v == u)){
            break;
        }
    }

    // Sort the vertices in the BCC
    qsort(tmpArr, tmpCt, sizeof(int), cmpInt);
    assert((components[componentCt].list = (int *) malloc(tmpCt * sizeof(int))) != NULL);
    components[componentCt].size = tmpCt;
    for (int i=0; i<tmpCt; i++){
        components[componentCt].list[i] = tmpArr[i];
    }
    componentCt++;
    free(tmpArr);
    free(used);

    return ;
}

void DFSVisit(Graph *G, Graph *ST, int u){
    visited[u] = true;
    time++;
    timeIn[u] = time;
    low[u] = time;          // Case 1: timeIn[u]
    Node *tmp = G -> adj[u] -> next;
    while (tmp){
        int v = tmp -> id;
        if (!visited[v]){
            parent[v] = u;
            addEdge(ST, u, v);
            pushEdgeStack(u, v);
            DFSVisit(G, ST, v);
            // Case 2: low[v]
            low[u] = min(low[u], low[v]);
            if (low[v] >= timeIn[u]){
                // BCC found
                constructBcc(u, v);
            }
        }
        else if (v != parent[u]){
            // Case 3: timeIn[w]
            low[u] = min(low[u], timeIn[v]);
            if (timeIn[v] < timeIn[u]){
                // Back edge found
                pushEdgeStack(u, v);
            }
        }
        tmp = tmp -> next;
    }

    return ;
}


Graph *DFS(Graph *G){
    memset(visited, false, n * sizeof(bool));
    memset(timeIn, -1, n * sizeof(int));
    memset(low, -1, n * sizeof(int));
    memset(parent, -1, n * sizeof(int));

    // Create a new graph for the DFS tree
    Graph *ST = createGraph();

    bool flag = false;
    for (int i=0; i<n; i++){
        if (!visited[i]){
            if (flag){
                perror("Error: Graph is not connected");
                exit(1);
            }
            DFSVisit(G, ST, i);
            flag = true;
        }
    }

    return ST;
}

void Bridge(Graph *G){
    // An edge (u, v) is a bridge iff low[v] > timeIn[u]
    int bridgeCt = 0;
    for (int u=0; u<n; u++){
        Node *tmp = G -> adj[u] -> next;
        while (tmp){
            int v = tmp -> id;
            if (low[v] > timeIn[u]){
                bridgeCt++;
            }
            tmp = tmp -> next;
        }
    }

    printf("%d\n", bridgeCt);

    return ;
}

int findArtiCt(Graph *ST){
    int articulationCt = 0;
    memset(isArti, false, n * sizeof(bool));
    // The root of the DFS tree is an AP iff it has more than one child
    if (ST -> adj[0] -> next != NULL && ST -> adj[0] -> next -> next != NULL){
        isArti[0] = true;
        articulationCt++;
    }

    // The non-root vertex u is an AP iff 
    // it has at least one child v such that low[v] >= timeIn[u] in the DFS tree
    for (int u=1; u<n; u++){
        Node *tmp = ST -> adj[u] -> next;
        while (tmp && !isArti[u]){
            int v = tmp -> id;
            if (low[v] >= timeIn[u]){
                isArti[u] = true;
                articulationCt++;
            }
            tmp = tmp -> next;
        }
    }

    return articulationCt;
}

void ArtiPoints(Graph *ST){
    // Print the articulation points
    int artiCt = findArtiCt(ST);
    printf("%d\n", artiCt);
    for (int i=0; i<n; i++){
        if (isArti[i]){
            printf("%d ", i);
        }
    }
    printf("\n");

    return ;
}

void Bcc(){
    // Print the info of BCCs
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

    Graph *ST = DFS(G);

    if (type == 1){
        ArtiPoints(ST);
    }
    else if (type == 2){
        Bridge(G);
        Bcc();
    }
    else {
        perror("Error: Invalid type");
        exit(1);
    }

    return 0;

}