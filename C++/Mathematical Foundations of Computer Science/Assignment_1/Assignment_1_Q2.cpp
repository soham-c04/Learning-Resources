#include <bits/stdc++.h>
using namespace std;

/*
Reference
Source 1: https://cp-algorithms.com/graph/2SAT.html
Source 2: https://cp-algorithms.com/graph/strongly-connected-components.html
*/

typedef vector<int> vi;

void dfs(int u, vi g[], vi &order, bool vis[]) {
    vis[u] = true;
    for(int v:g[u]){
        if(!vis[v]){
            dfs(v, g, order, vis);
		}
	}
    order.push_back(u);
}

bool Kosaraju(vi g[], int n){
    vi order;
    bool vis[n] = {0};

    for(int i=0;i<n;i++){
        if(!vis[i]){
    		dfs(i, g, order, vis);
		}
	}

    vi g_T[n];
    for(int i=0;i<n;i++){
        for(int v:g[i]){
            g_T[v].push_back(i);
		}
	}

    memset(vis, false, sizeof(vis));
    reverse(order.begin(), order.end());

	bool same[n] = {0};
    for(int u:order){
        if(!vis[u]){
            vi comp;
            dfs(u, g_T, comp, vis);
            for(int v:comp){
				same[v] = true;
				if(v<n/2){
					if(same[n/2+v]) return false;
				}
				else{
					if(same[v-n/2]) return false;
				}
        	}
        	for(int v:comp) same[v] = false;
        }
	}

	return true;
}

int main(){
	int n,m;
	cout<<"Enter the number of variables: ";
	cin>>n;
	cout<<"Enter the number of clauses: ";
	cin>>m;
	cout<<"\nNote - All literals should be numbered 1 to n.\n     - Negation of literal 'a' is represented by -a.\n\nEnter clauses:\n";

	vi g[2*n];
	while(m--){
		int u,v;
		cin>>u>>v;
		int U,V;
		if(u<0){
			U = -u;
			u = n-u;
		}
		else U = n+u;
		if(v<0){
			V = -v;
			v = n-v;
		}
		else V = n+v;
		u--; v--;
		U--; V--;
		
		g[U].push_back(v);
		g[V].push_back(u);
	}
	
	bool satisfiable = Kosaraju(g,2*n);
	
	if(satisfiable) cout<<"Satisfiable\n";
	else cout<<"Not Satisfiable\n";
	
	return 0;
}
