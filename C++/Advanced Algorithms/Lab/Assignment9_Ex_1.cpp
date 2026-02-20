#include <bits/stdc++.h>
using namespace std;

int main(){
	int k;
	cin>>k;
	vector<int> g[3*k];
	vector<int> pos[3*k+1], neg[3*k+1];
	for(int i=0;i<3*k;){
		g[i].push_back(i+1);
		g[i].push_back(i+2);
		g[i+1].push_back(i);
		g[i+1].push_back(i+2);
		g[i+2].push_back(i);
		g[i+2].push_back(i+1);
		int c[3];
  		for(int j=0;j<3;j++,i++){
  			cin>>c[j];
  			if(c[j]>0) pos[c[j]].push_back(i);
  			else neg[-c[j]].push_back(i);
		}
	}
	for(int i=1;i<=3*k;i++){
		for(int p:pos[i]){
			for(int n:neg[i]){
				g[p].push_back(n);
				g[n].push_back(p);
			}
		}
	}

 	cout<<"\nEquivalent Graph G[]:-\n";
	for(int i=0;i<3*k;i++){
		if(g[i].size()){
			cout<<i+1<<": ";
   			for(int j:g[i]) cout<<j+1<<" ";
   			cout<<"\n";
		}
	}
	return 0;
}
