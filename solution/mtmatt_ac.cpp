#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ll long long
#define MAX_N 100010
#define MAX_E 2000005

#define DEBUG 0
#define debug(...)                                                             \
  do {                                                                         \
    if (DEBUG == 1)                                                            \
      fprintf(stderr, __VA_ARGS__);                                            \
  } while (0)

typedef struct cds_array {
  char *data;
  ll size, capacity, element_size;
} CdsArray;

CdsArray ca_new(const ll element_size) {
  CdsArray new_array = {.data = (char *)malloc(element_size * 4),
                        .size = 0,
                        .capacity = 4,
                        .element_size = element_size};
  if (new_array.data == NULL) {
    new_array.data = (char *)malloc(element_size);
    new_array.capacity = 1;
    if (new_array.data == NULL) {
      fprintf(stderr, "Fatal error: Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
  }
  return new_array;
}

void ca_delete(CdsArray *array) {
  if (array->data != NULL)
    free(array->data);
  array->data = NULL;
  array->size = array->capacity = 0;
  array->element_size = 0;
}

int ca_push_back(CdsArray *array, const void *new_element) {
  if (array->size == array->capacity) {
    array->capacity <<= 1;
    array->data =
        (char *)realloc(array->data, array->capacity * array->element_size);
    if (array->data == NULL) {
      return -1;
    }
  }
  memmove(array->data + array->size * array->element_size, new_element,
          array->element_size);
  array->size++;
  return 0;
}

int ca_pop_back(CdsArray *array) {
  if (array->size == 0) {
    return -1;
  }
  array->size--;
  return 0;
}

void *ca_get(const CdsArray *array, const ll index) {
  return (void *)(array->data + index * array->element_size);
}

ll ca_size(const CdsArray *array) { return array->size; }
bool ca_empty(const CdsArray *array) { return array->size == 0; }

ll N, M, mode;

int head[MAX_N], to[MAX_E], nxt[MAX_E], ec;

static inline void addEdge(int u, int v) {
  nxt[ec] = head[u];
  to[ec] = v;
  head[u] = ec++;
}

ll ap_idx, ap[MAX_N];

ll depth[MAX_N], low[MAX_N], dfs_count;
bool is_ap[MAX_N];

void dfs(ll u, ll parent) {
  ll ch = 0;
  low[u] = depth[u];

  for (int e = head[u]; e != -1; e = nxt[e]) {
    ll v = to[e];
    if (depth[v] == 0) {
      depth[v] = depth[u] + 1;
      ch++;
      dfs(v, u);

      if (low[v] >= depth[u])
        is_ap[u] = true;
      low[u] = low[u] < low[v] ? low[u] : low[v];
    } else if (v != parent) {
      low[u] = low[u] < depth[v] ? low[u] : depth[v];
    }
  }

  if (parent == u && ch <= 1)
    is_ap[u] = false;
}

void find_articulation_points() {
  dfs_count = 0;
  ap_idx = 0;

  for (ll i = 0; i < N; i++) {
    low[i] = 0;
    depth[i] = 0;
  }

  for (ll i = 0; i < N; i++) {
    if (depth[i] == 0) {
      depth[i] = 1;
      dfs(i, i);
    }
  }

  for (ll i = 0; i < N; i++)
    if (is_ap[i])
      ap[ap_idx++] = i;
}

CdsArray bcc_stack;
CdsArray bcc_list[MAX_N];
ll bcc_idx, bridge_count;

void dfs_bcc(ll u, ll parent) {
  low[u] = depth[u];
  ca_push_back(&bcc_stack, &u);

  for (int e = head[u]; e != -1; e = nxt[e]) {
    ll v = to[e];
    if (v == parent)
      continue;
    if (depth[v] == 0) {
      depth[v] = depth[u] + 1;
      dfs_bcc(v, u);
      low[u] = low[u] < low[v] ? low[u] : low[v];

      if (low[v] > depth[u]) {
        bridge_count++;
      }
      if (low[v] >= depth[u]) {
        bcc_list[bcc_idx] = ca_new(sizeof(ll));

        ll w = -1;
        while (!ca_empty(&bcc_stack)) {
          w = *(ll *)ca_get(&bcc_stack, ca_size(&bcc_stack) - 1);
          ca_push_back(&bcc_list[bcc_idx], &w);
          ca_pop_back(&bcc_stack);
          debug("%lld ", w);
          if (w == v)
            break;
        }

        ca_push_back(&bcc_list[bcc_idx], &u);

        bcc_idx++;
      }
    } else {
      low[u] = low[u] < depth[v] ? low[u] : depth[v];
    }
  }
}

void find_bcc() {
  bcc_stack = ca_new(sizeof(ll));
  bcc_idx = 0;
  bridge_count = 0;

  for (ll i = 0; i < N; ++i) {
    low[i] = depth[i] = 0;
    is_ap[i] = false;
  }

  ec = 0;
  for (ll i = 0; i < N; ++i) {
    if (depth[i] == 0) {
      depth[i] = 1;
      dfs_bcc(i, -1);

      if (!ca_empty(&bcc_stack))
        while (!ca_empty(&bcc_stack))
          ca_pop_back(&bcc_stack);
    }
  }
  ca_delete(&bcc_stack);
}

int ll_cmp(const void *a, const void *b) { return (*(ll *)a - *(ll *)b); }

int ca_cmp(const void *a, const void *b) {
  CdsArray *ca_a = (CdsArray *)a;
  CdsArray *ca_b = (CdsArray *)b;

  if (ca_a->data == NULL && ca_b->data == NULL)
    return 0;
  if (ca_a->data == NULL)
    return -1;
  if (ca_b->data == NULL)
    return 1;

  ll size_a = ca_size(ca_a);
  ll size_b = ca_size(ca_b);
  ll min_size = size_a < size_b ? size_a : size_b;

  for (ll i = 0; i < min_size; ++i) {
    ll val_a = *(ll *)ca_get(ca_a, i);
    ll val_b = *(ll *)ca_get(ca_b, i);
    if (val_a != val_b)
      return (val_a < val_b) ? -1 : 1;
  }

  if (size_a != size_b)
    return (size_a < size_b) ? -1 : 1;

  return 0;
}

int main(void) {
  scanf("%lld%lld%lld", &N, &M, &mode);

  ec = 0;
  memset(head, -1, sizeof(head));
  for (int i = 0; i < M; i++) {
    ll u, v;
    scanf("%lld%lld", &u, &v);
    addEdge(u, v);
    addEdge(v, u);
  }

  if (mode == 1) {
    find_articulation_points();
    printf("%lld\n", ap_idx);
    for (ll i = 0; i < ap_idx; ++i)
      printf("%lld ", ap[i]);
    printf("\n");
  } else {
    find_bcc();
    if (mode == 2){
        printf("%lld\n", bridge_count);
        return 0;
    } else {
        printf("%lld\n", bcc_idx);

        ll valid_bcc_count = 0;
        for (ll i = 0; i < bcc_idx; ++i) {
        CdsArray *comp = &bcc_list[i];
        if (comp->data != NULL && comp->size > 0) {
            if (comp->size > 1) {
            qsort(comp->data, comp->size, sizeof(ll), ll_cmp);
            }

            ll unique_idx = 0;
            if (comp->size > 0) {
            unique_idx = 1;
            for (ll j = 1; j < comp->size; ++j) {
                if (*(ll *)ca_get(comp, j) != *(ll *)ca_get(comp, j - 1)) {
                if (j != unique_idx) {
                    memcpy(ca_get(comp, unique_idx), ca_get(comp, j), sizeof(ll));
                }
                unique_idx++;
                }
            }
            comp->size = unique_idx;
            }

            if (comp->size >= 2) {
            if (i != valid_bcc_count) {
                bcc_list[valid_bcc_count] = bcc_list[i];
                comp->data = NULL;
            }
            valid_bcc_count++;
            } else {
            ca_delete(comp);
            }
        } else {
            if (comp->data != NULL)
            ca_delete(comp);
        }
        }
        bcc_idx = valid_bcc_count;

        if (bcc_idx > 1) {
        qsort(bcc_list, bcc_idx, sizeof(CdsArray), ca_cmp);
        }

        for (ll i = 0; i < bcc_idx; ++i) {
        CdsArray *comp = &bcc_list[i];
        for (ll j = 0; j < ca_size(comp); ++j) {
            printf("%lld%c", *(ll *)ca_get(comp, j),
                (j == ca_size(comp) - 1) ? '\n' : ' ');
        }
        ca_delete(comp);
        }
    }
  }
}