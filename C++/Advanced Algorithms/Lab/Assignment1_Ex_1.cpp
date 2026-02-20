#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define f(n) for(int i=0;i<(n);i++)
typedef vector<int> vi;
typedef pair<int,int> pi;

vector<pi> MaximalMatching(vi g[], int n){  // Time Complexity - O(n+m)
	vector<pi> maximal_matching;
	bool matched[n] = {0};          		// Maintains if vertex_i is in matching
	bool vis[n] = {0};              		// Ensures after putting into dfs stack. no element u traversed twice.
	stack<int> dfs;
	dfs.push(0);
	while(!dfs.empty()){
		int u = dfs.top();
		dfs.pop();
		if(vis[u]) continue;
		vis[u] = true;
		for(int v:g[u]){
			if(!matched[v]){
				if(!matched[u]){
					matched[u] = matched[v] = true;
					maximal_matching.pb({u,v});
				}
				dfs.push(v);
			}
		}
	}
	return maximal_matching;
}

vi MaximalIndependentSet(vi g[], int n){    // Time Complexity - O(n+m)
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
		vi g[n];
		f(m){			// Constructing adjacency list from input - O(m)
			int u,v;
			cin>>u>>v;
			g[u].pb(v);
			g[v].pb(u);
		}
		
		vector<pi> maximal_matching = MaximalMatching(g,n);    	// Vector of edges containing a maximal matching.
		
		vi maximal_independent_set = MaximalIndependentSet(g,n); // Vector of nodes containing a maximal independent set.
		
		// Output
		
		// Testcase
		cout<<"\nTestCase "<<tt<<": Vertices = "<<n<<" Edges = "<<m<<"\n";
		
		// Adjacency List
		cout<<"Adjacency List:\n";
		for(int u=0;u<n;u++){
			cout<<u<<": ";
			for(int v:g[u]) cout<<v<<" ";
			cout<<"\n";
		}
		
		// Maximal matching
		cout<<"Maximal Matching:\n";
		for(auto [u,v]:maximal_matching) cout<<"("<<u<<","<<v<<")\n";
		
		// Maximal Independent Set
		cout<<"Maximal Independent Set:\n";
		for(int u:maximal_independent_set) cout<<u<<" ";
		cout<<"\n";
	}
}
