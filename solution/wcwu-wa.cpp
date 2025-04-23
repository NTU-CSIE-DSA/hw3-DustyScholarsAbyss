
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<ll,ll> pll;
typedef pair<int, int> pii;
const ll INF=4e18;
#define IOS ios::sync_with_stdio(false);cin.tie(0);
#define rep(i,n) for(ll i=0;i<n;i++)
#define rep1(i,n) for(ll i=1;i<=n;i++)
#define F first
#define S second
#define pb push_back
const int N=1e5+5;
vector<int> G[N];
bool cutpts[N];
vector<vector<int> > bccs;
int in[N], low[N], parent[N];
int bridges=0, cnt=0, n, m, mode;
bool vis[N];
stack<pair<int, int> > edge_stack;

void dfs(int u) {
	vis[u]=1;
	in[u]=low[u]=++cnt;
	int ch=0;
	for(int v:G[u]) {
		if(!vis[v]) {
			parent[v]=u;
			ch++;
			edge_stack.push({u, v});
			dfs(v);
			low[u]=min(low[u], low[v]);
			if(low[v]>in[u]) {
				bridges++;
			}
			if ((parent[u] == -1 && ch > 1) || (parent[u] != -1 && low[v] >= in[u])) {
                cutpts[u]=1;
				vector<int> component;
                set<int> vertices;               
                while(!edge_stack.empty()) {
                    auto [x, y] = edge_stack.top();
                    edge_stack.pop();
                    
                    vertices.insert(x);
                    vertices.insert(y);
                    
                    if((x == u && y == v) || (x == v && y == u)) {
                        break;
                    }
                }
				for(int vertex : vertices) {
                    component.push_back(vertex);
                }
                bccs.push_back(component);
            }
        } 
		else if(v != parent[u]) {
            if(in[v]<in[u]) edge_stack.push({u, v});
            low[u] = min(low[u], in[v]);
        }	
	}
}

signed main() {
	cin>>n>>m>>mode;
	memset(cutpts, false, sizeof(cutpts));
	memset(vis, false, sizeof(vis));
	fill(parent, parent+N, -1);
	rep(i, m) {
		int u, v;
		cin>>u>>v;
		G[u].pb(v);
		G[v].pb(u);
	}
	rep(i, n) {
		if(!vis[i]) {
			dfs(i);
			if(!edge_stack.empty()) {
                vector<int> component;
                set<int> vertices;
                while(!edge_stack.empty()) {
                    auto [x, y] = edge_stack.top();
                    edge_stack.pop();
                    
                    vertices.insert(x);
                    vertices.insert(y);
                }
                for(int vertex : vertices) {
                    component.push_back(vertex);
                }
                bccs.push_back(component);
            }
		}
	}
	if(mode==2) {
		cout<<bridges<<"\n";
		cout<<bccs.size()<<"\n";
		reverse(bccs.begin(), bccs.end());
		for(auto &component:bccs) {
			for(int v:component) {
				cout<<v<<" ";
			}
			cout<<"\n";
		}
	}
	else {
		int ans=0;
		rep(i, n) {
			if(cutpts[i]==1) {
				ans++;
			}
		}
		cout<<ans<<"\n";
		rep(i, n) {
			if(cutpts[i]==1) {
				cout<<i<<" ";
			}
		} cout<<"\n";
	}
}
