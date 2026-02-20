#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

// Reference Source: https://cp-algorithms.com/graph/strongly-connected-components.html

void dfs(int u, vi g[], vi &order, bool vis[]) {
    vis[u] = true;
    for(int v:g[u]){
        if(!vis[v]){
            dfs(v, g, order, vis);
		}
	}
    order.push_back(u);
}

vector<vi> Kosaraju(vi g[], int n){
	vector<vi> SCC = {};

    vi order; // Order of vertices by exit time in Euler Tour
    bool vis[n] = {0};

    for(int i=0;i<n;i++){
        if(!vis[i]){
    		dfs(i, g, order, vis);
		}
	}

    // Transpose of g[]
    vi g_T[n];
    for(int i=0;i<n;i++){
        for(int v:g[i]){
            g_T[v].push_back(i);
		}
	}

    memset(vis, false, sizeof(vis));
    reverse(order.begin(), order.end());

    for(int u:order){
        if(!vis[u]){
            vi comp;   // SCC reachable from given node
            dfs(u, g_T, comp, vis);
   			SCC.push_back(comp);
        }
	}
	
	return SCC;
}

int main(){
	int n;
	cin>>n;
	vi g[n];
	for(int i=0;i<n;i++){
		int k;
		cin>>k;
		while(k--){
			int v;
			cin>>v;
			g[i].push_back(v);
		}
	}
	
	vector<vi> SCC = Kosaraju(g, n);
	
	cout<<"\nMinimum number of Groups = "<<SCC.size()<<"\n";
	for(int i=0;i<SCC.size();i++){
		cout<<"Group "<<i+1<<": [";
		for(int v:SCC[i]) cout<<v<<", ";
		cout<<"]\n";
	}

	return 0;
}
