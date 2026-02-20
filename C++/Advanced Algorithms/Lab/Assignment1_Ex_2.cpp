#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define f(n) for(int i=0;i<(n);i++)
typedef vector<int> vi;
typedef pair<int,int> pi;

vector<vi> LineGraph(vector<pi> g[], int n, int m){	// Time Complexity - O(n + m^2) or O(n + sum(m_i^2))
	vector<vi> L_G(m);
	f(n){
		int m_i = g[i].size();
		vector<pi> edge = g[i];
		for(int j=0;j<m_i-1;j++){
			for(int k=j+1;k<m_i;k++){
				L_G[edge[j].second].pb(edge[k].second);
				L_G[edge[k].second].pb(edge[j].second);
			}
		}
	}
	return L_G;
}

vi MaximalIndependentSet(vector<vi> &g){    // Time Complexity - O(|V(G)| + |E(G)|)
	int n = g.size();
	bool keep[n];							// Whether i is in indepedent_set vector
	f(n) keep[i] = true;
	bool vis[n] = {0};              		// Ensures after putting into dfs stack. no element u traversed twice.
	stack<int> dfs;
	dfs.push(0);
	while(!dfs.empty()){
		int u = dfs.top();
		dfs.pop();
		for(int v:g[u]){
			if(!vis[v]){
				if(keep[u]) keep[v] = false;
				vis[v] = true;
				dfs.push(v);
			}
		}
	}
	vi maximal_independent_set;
	f(n) if(keep[i]) maximal_independent_set.pb(i);
	return maximal_independent_set;
}

int main(){
	// Disable fast_io if immediate output is required after each testcase
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int T,n,m;
	cin>>T;
	for(int tt=1;tt<=T;tt++){
		// Taking input
		cin>>n>>m;
		vector<pi> g[n];
		pi edges[m];
		f(m){
			int u,v;
			cin>>u>>v;
			edges[i] = {u,v};
			g[u].pb({v,i}); // Constructing adjacency list with data about edge_id to create Line Graph
			g[v].pb({u,i});
		}

		vector<vi> L_G = LineGraph(g,n,m); // Note that L(G) is not in sorted order because it will take extra O(m^2 * log(m)) time.
		
		vi maximal_independent_set = MaximalIndependentSet(L_G);	// Maximal independent set of L(G)
		
		vector<pi> maximal_matching;
		for(int s:maximal_independent_set)  // Independent set in L(G) corresponds to Matching in G.
			maximal_matching.pb(edges[s]);

		// Output

		// Testcase
		cout<<"\nCase #"<<tt<<":\n";
		cout<<"Vertices: "<<n<<"\nEdges: "<<m<<"\n";

		// Adjacency List
		cout<<"\nLine Graph L(G) Adjacency List:\n";
		for(int u=0;u<m;u++){
			cout<<u<<": ";
			for(int v:L_G[u]) cout<<v<<" ";
			cout<<"\n";
		}

		// Maximal matching
		cout<<"\nMaximal Matching from L(G):\n";
		for(auto [u,v]:maximal_matching) cout<<"("<<u<<","<<v<<")\n";
	}
}
