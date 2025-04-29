#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 100005
#define M 1000005
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct Node {
    int v;
    struct Node* next;
} Node;

typedef struct {
    int u, v;
} Edge;

typedef struct {
    Edge* data;
    int top;
} EdgeStack;

typedef struct {
	int *list;
	int sz;
} Component;

Node* G[N];
Component bcc[N];
bool cutpts[N];
int in[N], low[N], parent[N];
int bcc_cnt=0, bridges = 0, cnt = 0, n, m, mode;
bool vis[N];
EdgeStack edge_stack;

void add_edge(int u, int v);
void init_stack(EdgeStack* stack);
void push(EdgeStack* stack, int u, int v);
Edge pop(EdgeStack* stack);
bool is_empty(EdgeStack* stack);
void dfs(int u);
int compare_elements(const void* a, const void* b);
int compare_bccs(const void* a, const void* b);

void init_stack(EdgeStack* stack) {
    stack->data = (Edge*)malloc(M*sizeof(Edge));
    stack->top = -1;
}

void push(EdgeStack* stack, int u, int v) {
    stack->top++;
    stack->data[stack->top].u = u;
    stack->data[stack->top].v = v;
}

Edge pop(EdgeStack* stack) {
    Edge e = stack->data[stack->top];
    stack->top--;
    return e;
}

bool is_empty(EdgeStack* stack) {
    return stack->top == -1;
}

void add_edge(int u, int v) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->v = v;
    new_node->next = G[u];
    G[u] = new_node;
}

int compare_elements(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int compare_bccs(const void* a, const void* b) {
    Component *X=(Component *)a;
	Component *Y=(Component *)b;
	int min_size=min(X->sz, Y->sz);
    for(int i=0;i<min_size;i++) {
        if (X->list[i] != Y->list[i]) {
            return X->list[i]-Y->list[i];
        }
    }
    return X->sz-Y->sz;
}
void dfs(int u) {
    vis[u] = true;
    in[u] = low[u] = ++cnt;
	int ch=0;
    for (Node* curr = G[u]; curr != NULL; curr = curr->next) {
        int v = curr->v;
		//printf("%d %d %d\n", v, vis[v], parent[u]);
        if (!vis[v]) {
            parent[v] = u;
            push(&edge_stack, u, v);
			ch++;
			//printf("dfs %d\n", v);
			dfs(v);
			//printf("finish %d\n", v);
            low[u] = min(low[u], low[v]);
            if (low[v] > in[u]) {
                bridges++;
            }
            if ((in[u]==1 && ch>1) || in[u]>1 && low[v]>=in[u]) {
                cutpts[u] = true;
                int* vertices = (int*)malloc((edge_stack.top+1) * sizeof(int)); 
                int vertex_count = 0;
                bool* used=(bool*)calloc(N, sizeof(bool));
				while (!is_empty(&edge_stack)) {
                    Edge e = pop(&edge_stack);
                    if(!used[e.u]) {
						used[e.u]=1;
						vertices[vertex_count++]=e.u;
					}
					if(!used[e.v]) {
						used[e.v]=1;
						vertices[vertex_count++]=e.v;
					}
                    if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) {
                        break;
                    }
                }
				//printf("%d is cut points\n", u);
                qsort(vertices, vertex_count, sizeof(int), compare_elements);
                assert((bcc[bcc_cnt].list=(int *)malloc(vertex_count*sizeof(int)))!=NULL);
				bcc[bcc_cnt].sz=vertex_count;
				for(int i=0;i<vertex_count;i++) {
					bcc[bcc_cnt].list[i]=vertices[i];
				}
				bcc_cnt++;
				free(used);
				free(vertices);
            }
        } 
		else if (v != parent[u]) {
            if (in[v] < in[u]) {
                push(&edge_stack, u, v);
            }
            low[u] = min(low[u], in[v]);
        }
		if(curr->next==NULL) return;
    }
}

int main() {
    scanf("%d %d %d", &n, &m, &mode);
    memset(cutpts, false, sizeof(cutpts));
    memset(vis, false, sizeof(vis));
    for (int i=0;i<N;i++) {
        parent[i]=-1;
		low[i]=-1;
		in[i]=-1;
        G[i]=NULL;
    }
    init_stack(&edge_stack);
    for (int i = 0; i < m; i++) {
        int u, v;
		scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(i);
            if (!is_empty(&edge_stack)) {
                int* vertices = (int*)malloc((edge_stack.top+1) * sizeof(int));
                int vertex_count = 0;
                bool* used=(bool*)calloc(N, sizeof(bool));
				while (!is_empty(&edge_stack)) {
                    Edge e = pop(&edge_stack);
                    if(!used[e.u]) {
						used[e.u]=1;
						vertices[vertex_count++]=e.u;
					}
					if(!used[e.v]) {
						used[e.v]=1;
						vertices[vertex_count++]=e.v;
					}
                }
                qsort(vertices, vertex_count, sizeof(int), compare_elements);
                assert((bcc[bcc_cnt].list=(int *)malloc(vertex_count*sizeof(int)))!=NULL);
                bcc[bcc_cnt].sz=vertex_count;
				for(int i=0;i<vertex_count;i++) {
					bcc[bcc_cnt].list[i]=vertices[i];
				}
				bcc_cnt++;
				free(used);
				free(vertices);
            }
        }
    }
    if (mode == 2) {
        printf("%d\n", bridges);
    } else if (mode == 1) {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (cutpts[i]) {
                ans++;
            }
        }
        printf("%d\n", ans);
        for (int i = 0; i < n; i++) {
            if (cutpts[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");
    } else {
        qsort(bcc, bcc_cnt, sizeof(Component), compare_bccs);
        printf("%d\n", bcc_cnt);
        for (int i = 0; i < bcc_cnt; i++) {
            for (int j = 0; j < bcc[i].sz; j++) {
                printf("%d ", bcc[i].list[j]);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < n; i++) {
        Node* curr = G[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    /*for (int i = 0; i < bcc_cnt; i++) {
        for(int j=0;j<bcc[i].sz;j++) {
			free((void *)bcc[i].list[j]);
		}
    }*/
    free(edge_stack.data);
    return 0;
}