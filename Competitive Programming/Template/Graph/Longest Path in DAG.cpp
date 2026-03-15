#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<ll> vll;
typedef pair<int,int> pi;
#define f(n) for(int i=0;i<(n);i++)

// https://www.geeksforgeeks.org/dsa/find-longest-path-directed-acyclic-graph/
// Unverified - Should stk be reversed after topo sort ?
// O(n+m)
vll longestPath(vector<pi> g[], int n, int source){
	stack<int> stk;
	bool vis[n]={0};
	function<void(int)> topo = [&](int u){
		vis[u] = true;
		for(auto [v,w]:g[u]){
			if(!vis[v]) topo(v);
		}
		stk.push(u);
	};
	f(n){
		if(!vis[i]) topo(i);
	}
	
	ll INF = 1e18;
	vll dis(n,-INF);
	dis[source] = 0;
	while(stk.size()){
		int u = stk.top();
		stk.pop();
		if(dis[u]!=-INF){
			for(auto [v,w]:g[u]){
				if(dis[v]<dis[u]+w) dis[v] = dis[u]+w;
			}
		}
	}
	return dis;
}

int main(){
	return 0;
}
