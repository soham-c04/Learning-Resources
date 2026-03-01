#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;

// Source - https://cp-algorithms.com/graph/lca.html
// O(NlogN) preprocessing + O(1) query
class LCA{
	private:
		int n;
		vi height, tin;
		vector<vi> table;
		
		void dfs(vi g[], int root, int euler[]){
		    int timer = 0;
		    stack<int> stk;
		    stk.push(root);
		    height[root] = 0;
		    while(!stk.empty()){
		        int u = stk.top();
		        stk.pop();
				euler[timer++] = u;
//				cout<<u<<", ";
		        if(tin[u]==-1){			// Enter u
					tin[u] = timer-1;
					for(int v:g[u]){
		                if(height[v]==-1){
		                	height[v] = height[u]+1;
		                	stk.push(u);
							stk.push(v);
						}
		            }
		        }
		    }
		}
		
		int F(int x, int y){
			return height[x]<height[y]? x:y;
		}
		
		void build(vi g[], int root){
			int m = 2*n-1;
			int euler[m];
			dfs(g, root, euler);

			int k=__lg(m)+1;
			table.resize(k,vi(m));
			for(int i=0;i<m;i++) table[0][i]=euler[i];
			for(int i=1;i<k;i++){
				for(int j=0;j+(1<<i)<=m;j++){
					table[i][j] = F(table[i-1][j], table[i-1][j+(1<<(i-1))]);
				}
			}
		}
		
	public:
		LCA(vi g[], int n, int root){
			this->n = n;
			height.resize(n, -1);
			tin.resize(n, -1);
			build(g, root);
		}
		
		int query(int u, int v){
			int l = tin[u], r = tin[v];
			if(l>r) swap(l,r);
			int i=__lg(r-l+1);
			return F(table[i][l], table[i][r-(1<<i)+1]);
		}
};

int main(){
	int n, root=0, q;
	cin>>n>>root>>q;
	vi g[n];
	for(int i=0;i<n-1;i++){
		int u,v;
		cin>>u>>v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	
	LCA lca(g, n, root);
	while(q--){
		int u,v;
		cin>>u>>v;
		cout<<lca.query(u-1, v-1)<<"\n";
	}
}
