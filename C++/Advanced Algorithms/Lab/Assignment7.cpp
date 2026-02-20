#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define F first
#define S second

vector<vector<int>> graph;
vector<vector<int>> capacity;

void printResidualGraph(){
	int n = graph.size();
	cout<<"\nResidual Graph:-\n";
	for(int i=0;i<n;i++){
		cout<<i+1<<": ";
		for(int j:graph[i]) cout<<"("<<j+1<<","<<capacity[i][j]<<") ";
		cout<<"\n";
	}
}

void printEdges(){
	int n = graph.size();
	cout<<"\nResidual Graph:-\n";
	for(int i=0;i<n;i++) for(int j:graph[i]) cout<<i+1<<" "<<j+1<<" "<<capacity[i][j]<<"\n";
}

vector<int> get_path(int s, int t){
	vector<int> path;
	int n = graph.size();
	int par[n];
	memset(par, -1, sizeof(par));
	queue<int> bfs;
	par[s] = -2;
	bfs.push(s);
	while(bfs.size()){
		int u = bfs.front();
		bfs.pop();
		if(u == t){
			int p = t;
			while(p>=0){
				path.push_back(p);
				p = par[p];
			}
			reverse(path.begin(),path.end());
			break;
		}
		for(int v:graph[u]){
			if(capacity[u][v] && par[v]==-1){
				par[v] = u;
				bfs.push(v);
			}
		}
	}
	return path;
}

int augment(int s, int t){
	vector<int> path = get_path(s,t);
	if(path.empty()) return 0;
	int bottleneck = capacity[path[0]][path[1]];
	for(int i=1;i<path.size()-1;i++) bottleneck = min(bottleneck, capacity[path[i]][path[i+1]]);
	for(int i=0;i<path.size()-1;i++){
		capacity[path[i]][path[i+1]] -= bottleneck;
		capacity[path[i+1]][path[i]] += bottleneck;
	}
	return bottleneck;
}

ll maxflow(int s, int t){
	ll flow = 0;
	int bottleneck;
	while(bottleneck = augment(s,t)){
		flow += bottleneck;
		printResidualGraph();
//		printEdges();
	}
	return flow;
}

vector<int> minCut(int s){
	vector<int> S = {s};
	stack<int> dfs;
	int n = graph.size();
	bool vis[n] = {0};
	dfs.push(s);
	vis[s] = true;
	while(dfs.size()){
		int u = dfs.top();
		dfs.pop();
		for(int v:graph[u]){
			if(capacity[u][v] && !vis[v]){
				S.push_back(v);
				vis[v] = true;
				dfs.push(v);
			}
		}
	}
	return S;
}

int main(){
	int n,m;
	cout<<"Enter the number of vetices and edges: ";
	cin>>n>>m;
	graph.resize(n);
	capacity.resize(n, vector<int>(n,0));
	cout<<"Enter Directed Weighted Graph:-\n";
	while(m--){
		int u,v,w;
		cin>>u>>v>>w;
		u--; v--;
		if(!capacity[u][v] && !capacity[v][u]){
			graph[u].push_back(v);
			graph[v].push_back(u);	// Add reverse edge with capacity = 0;
		}
		capacity[u][v] += w;
	}
	int s,t;
	cout<<"Enter source and sink: ";
	cin>>s>>t;
	s--; t--;
	int max_flow = maxflow(s,t);
	cout<<"\nMaximum Flow in the Graph = "<<max_flow<<"\n";
	vector<int> S = minCut(s);
	cout<<"\nSet of vertices reachable from source (min-Cut partition): ";
	for(int a:S) cout<<a+1<<" "; cout<<"\n";
	return 0;
}
