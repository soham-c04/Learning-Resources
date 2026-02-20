#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

// Reference Source: https://cp-algorithms.com/graph/cutpoints.html

void dfs(int u, int par, vi g[], bool vis[], int tin[], int low[], int &timer, bool cut_vertex[]) {
    vis[u] = true;
    tin[u] = low[u] = timer++;
    int children=0;
    for(int v:g[u]) {
        if(v!=par){
	        if(vis[v]){
	            low[u] = min(low[u], tin[v]);
	        }
			else{
	            dfs(v, u, g, vis, tin, low, timer, cut_vertex);
	            low[u] = min(low[u], low[v]);
	            if(low[v] >= tin[u] && par!=-1){
	                cut_vertex[u] = true;
				}
	            children++;
	        }
		}
    }
    
    if(par == -1 && children > 1){
        cut_vertex[u] = true;
	}
}

int cut_vertices(vi g[], int n){
    int timer = 0;
	bool vis[n] = {0}, cut_vertex[n] = {0};
	int tin[n], low[n];
	memset(tin, -1, sizeof(tin));
	memset(low, -1, sizeof(low));
	
    for(int i=0;i<n;i++){
        if(!vis[i]){
            dfs(i, -1, g, vis, tin, low, timer, cut_vertex);
		}
    }
    
    int cut_points = 0;

    for(int i=0;i<n;i++){
    	if(cut_vertex[i]){
    		cut_points++;
		}
	}

	return cut_points;
}

int main(){
	int n, m;
	cin>>n>>m;
	vi g[n];
	while(m--){
		int u,v;
		cin>>u>>v;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	int cut_points = cut_vertices(g, n);

	cout<<"\nCritical Tourist attractions = "<<cut_points<<"\n";

	return 0;
}
