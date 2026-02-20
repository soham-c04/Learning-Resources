#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> X,Y;
vector<int> revX,revY;
vector<int> matched;
vector<bool> used;

int partition(vector<int> graph[], int n){
	int col[n] = {0};
	int mp[n];
	memset(mp,-1,sizeof(mp));
	for(int i=0;i<n;i++){
		if(!col[i]){
			stack<int> stk;
			stk.push(i);
			col[i] = 1;
			while(stk.size()){
				int u = stk.top();
				if(mp[u]==-1){
					if(col[u]==1){
						mp[u] = X.size();
						revX.push_back(u);
						X.push_back({});
					}
					else{
						mp[u] = Y.size();
						revY.push_back(u);
						Y.push_back({});
					}
				}
				stk.pop();
				for(int v:graph[u]){
					if(!col[v]){
						col[v] = 3-col[u];
						stk.push(v);
					}
					else if(col[u]==col[v]) return 0;
					
					if(col[u]==1){
						if(mp[v]==-1){
							mp[v] = Y.size();
							Y.push_back({});
							revY.push_back(v);
						}
						X[mp[u]].push_back(mp[v]);
					}
					else{
						if(mp[v]==-1){
							mp[v] = X.size();
							X.push_back({});
							revX.push_back(v);
						}
						Y[mp[u]].push_back(mp[v]);
					}
				}
			}
		}
	}
	return X.size();
}

bool kuhn(int u){
    if(used[u]) return false;
    used[u] = true;
    for(int v:X[u]){
        if(matched[v]==-1 || kuhn(matched[v])){
        	matched[v]=u;
            return true;
        }
    }
    return false;
}

int main(){
	int n,m;
	cout<<"Enter the number of vetices and edges: ";
	cin>>n>>m;
	vector<int> graph[n];
	cout<<"Enter Graph (1-based indexing):-\n";
	while(m--){
		int u,v;
		cin>>u>>v;
		u--; v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	
	int p = partition(graph,n);
	if(!p){
		cout<<"Input graph is not bipartite\n";
		return 1;
	}
	int q=n-p;
	matched.assign(p,-1);
	for(int i=0;i<p;i++){
		used.assign(p,false);
		kuhn(i);
	}
	
	cout<<"Set of matched edges:\n";
	for(int i=0;i<q;i++) if(matched[i]!=-1) cout<<revX[matched[i]]+1<<" "<<revY[i]+1<<"\n";
            
	return 0;
}
