#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100005
#define MAXM 1000005

typedef struct Edge {
    int to, next;
} Edge;

Edge edges[MAXM * 2];
int head[MAXN], edge_cnt = 0;

int low[MAXN], tin[MAXN], timer = 0;
int is_articulation[MAXN];
int parent[MAXN];

int stack[MAXM * 2], top = -1;

typedef struct {
    int *nodes;
    int size;
    int cap;
} Component;

Component components[MAXN];
int component_cnt = 0;

void add_edge(int u, int v) {
    edges[edge_cnt].to = v;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt++;
}

int min(int a, int b) { return a < b ? a : b; }

void push(int u, int v) {
    stack[++top] = u;
    stack[++top] = v;
}

void add_to_component(int v, Component *comp) {
    if (comp->size == comp->cap) {
        comp->cap *= 2;
        comp->nodes = (int*)realloc(comp->nodes, comp->cap * sizeof(int));
    }
    comp->nodes[comp->size++] = v;
}

int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compare_component(const void *a, const void *b) {
    Component *ca = (Component *)a;
    Component *cb = (Component *)b;
    for (int i = 0; i < ca->size && i < cb->size; ++i) {
        if (ca->nodes[i] != cb->nodes[i])
            return ca->nodes[i] - cb->nodes[i];
    }
    return ca->size - cb->size;
}

void dfs(int u, int p) {
    low[u] = tin[u] = ++timer;
    int children = 0;

    for (int i = head[u]; i != -1; i = edges[i].next) {
        int v = edges[i].to;
        if (v == p) continue;
        if (!tin[v]) {
            push(u, v);
            parent[v] = u;
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= tin[u]) {
                if (p != -1 || ++children > 1)
                    is_articulation[u] = 1;

                Component *comp = &components[component_cnt++];
                comp->cap = 8;
                comp->size = 0;
                comp->nodes = (int*)malloc(comp->cap * sizeof(int));

                int x, y;
                do {
                    y = stack[top--];
                    x = stack[top--];
                    add_to_component(x, comp);
                    add_to_component(y, comp);
                } while (!(x == u && y == v));
            }
        } else if (tin[v] < tin[u]) {
            push(u, v);
            low[u] = min(low[u], tin[v]);
        }
    }
}

int bridge_cnt = 0;

void count_bridges(int u, int p) {
    tin[u] = low[u] = ++timer;
    for (int i = head[u]; i != -1; i = edges[i].next) {
        int v = edges[i].to;
        if (v == p) continue;
        if (!tin[v]) {
            count_bridges(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridge_cnt++;
        } else {
            low[u] = min(low[u], tin[v]);
        }
    }
}

int main() {
    int N, M, mode;
    scanf("%d %d %d", &N, &M, &mode);
    memset(head, -1, sizeof(head));

    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }

    if (mode == 1) {
        for (int i = 0; i < N; ++i)
            if (!tin[i])
                dfs(i, -1);

        int result[MAXN], cnt = 0;
        for (int i = 0; i < N; ++i)
            if (is_articulation[i])
                result[cnt++] = i;

        printf("%d\n", cnt);
        for (int i = 0; i < cnt; ++i)
            printf("%d%c", result[i], i + 1 == cnt ? '\n' : ' ');
    } else if (mode == 2){
        memset(tin, 0, sizeof(tin));
        timer = 0;
        count_bridges(0, -1);
        memset(tin, 0, sizeof(tin));
        timer = 0;
        dfs(0, -1);

        printf("%d\n", bridge_cnt);
    } else {
        memset(tin, 0, sizeof(tin));
        timer = 0;
        count_bridges(0, -1);
        memset(tin, 0, sizeof(tin));
        timer = 0;
        dfs(0, -1);
        
        for (int i = 0; i < component_cnt; ++i) {
            Component *c = &components[i];
            qsort(c->nodes, c->size, sizeof(int), compare_int);
            // remove duplicates
            int unique = 0;
            for (int j = 0; j < c->size; ++j) {
                if (j == 0 || c->nodes[j] != c->nodes[j - 1])
                    c->nodes[unique++] = c->nodes[j];
            }
            c->size = unique;
        }

        qsort(components, component_cnt, sizeof(Component), compare_component);
        printf("%d\n", component_cnt);
        for (int i = 0; i < component_cnt; ++i) {
            for (int j = 0; j < components[i].size; ++j)
                printf("%d%c", components[i].nodes[j], j + 1 == components[i].size ? '\n' : ' ');
        }
    }

    return 0;
}
