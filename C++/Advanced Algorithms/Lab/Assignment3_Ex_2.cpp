#include <bits/stdc++.h>
using namespace std;

vector<int> Assign(vector<int> graph[], int n){
	vector<int> objective(n,0);
	// Iterative dfs
	stack<int> dfs;
	objective[0] = 1;
	dfs.push(0);
	while(!dfs.empty()){
		int u = dfs.top();
		dfs.pop();
		for(int v:graph[u]){
			if(!objective[v]){
				objective[v] = 3-objective[u];
				dfs.push(v);
			}
			else if(objective[v] == objective[u]){
				// non-conflicting Assignment is not possible.
				return {};
			}
		}
	}
	return objective;
}

int main(){
	int n,p;
	cin>>n>>p;
	vector<int> graph[n];
	while(p--){
		int u,v;
		cin>>u>>v;
		u--;v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	vector<int> objective = Assign(graph,n);
	if(objective.empty()) cout<<"\nAssignment of non-conflicting objectives is not possible.\n";
	else{
		cout<<"\nPossible Assignment:-\n";
		cout<<"Objective1: ";
		for(int i=0;i<n;i++){
			if(objective[i] <= 1) cout<<i+1<<", ";
		}
		cout<<"\n";
		cout<<"Objective2: ";
		for(int i=0;i<n;i++){
			if(objective[i] == 2) cout<<i+1<<", ";
		}
		cout<<"\n";
	}
	
	return 0;
}
