#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> tree,dp;

int rec(int u, bool take, int par){
	if(tree[u].empty()) return 1;
	else if(dp[u][take]!=-1) return dp[u][take];
	dp[u][take] = take;
	if(!take){
		for(int v:tree[u]) if(v!=par) dp[u][0] += rec(v,true,u);
	}
	else{
		for(int v:tree[u]) if(v!=par) dp[u][1] += min(rec(v,true,u),rec(v,false,u));
	}
	return dp[u][take];
}

int main(){
	int n;
	cout<<"Enter the number of nodes: ";
	cin>>n;
	tree.resize(n);
	cout<<"Enter edges (1-based indexing):\n";
	for(int i=0;i<n-1;i++){
		int u,v;
		cin>>u>>v;
		u--; v--;
		tree[u].push_back(v);
		tree[v].push_back(u);
	}

	dp.resize(n);
	while(n--) dp[n] = {-1,-1};
	cout<<"Min surveillance zones required: "<<min(rec(0,true,-1),rec(0,false,-1))<<"\n";
	stack<tuple<int,bool,int>> dfs;
	dfs.push({0,true,-1});
	vector<int> surveillance_zones;
	while(dfs.size()){
		auto [u,free,par] = dfs.top();
		dfs.pop();
		if(!free || dp[u][1]<=dp[u][0]){
			surveillance_zones.push_back(u+1);
			for(int v:tree[u]) if(v!=par) dfs.push({v,true,u});
		}
		else{
			for(int v:tree[u]) if(v!=par) dfs.push({v,false,u});
		}
	}
	cout<<"Surveillance zones are: ";
	for(int u:surveillance_zones) cout<<u<<" ";
	cout<<"\n";
	return 0;
}
