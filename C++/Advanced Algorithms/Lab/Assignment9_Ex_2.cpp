#include <bits/stdc++.h>
using namespace std;

int main(){
	int k;
	cin>>k;
	vector<int> g[7*k+1];

	for(int i=1;i<=6*k;i+=2){
		g[0].push_back(i);
		g[0].push_back(i+1);
		g[i].push_back(0);
		g[i].push_back(i+1);
		g[i+1].push_back(0);
		g[i+1].push_back(i);
	}

	for(int i=1;i<=k;i++){
		int l;
		int C = 6*k+i;
		for(int j=0;j<3;j++){
			cin>>l;
			if(l>0){
				g[2*l-1].push_back(C);
				g[C].push_back(2*l-1);
			}
			else{
				g[-2*l].push_back(C);
				g[C].push_back(-2*l);
			}
		}
	}

	cout<<"\nEquivalent Graph G[]:-\n";
	for(int i=0;i<=7*k;i++){
		if(g[i].size()){
			cout<<i<<": ";
			for(int j:g[i]) cout<<j<<" ";
			cout<<"\n";
		}
	}
	return 0;
}
