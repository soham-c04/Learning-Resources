#include <bits/stdc++.h>
using namespace std;

int StableCommunity(vector<vector<int>> &C){
	int n = C.size();
	// Finding probable leader
	int leader = 0;                 // Initially leader set to 0
	for(int i=1;i<n;i++){
		if(C[i][leader] != 1){  	// i doesn't follow 'leader'. Hence, 'leader' can't be actual leader.
			leader = i;         	// new probable leader
		}
		else if(C[leader][i] != -1){// 'leader' is not followed by i. Hence, This can't be actual leader.
			leader = i;             // new probable leader
		}
	}
	
	// Leader check
	for(int i=0;i<n;i++){
		if(i!=leader){
			if(C[i][leader] != 1) return 0;      // Invalid leader. Since, i doesn't follow 'leader'.
			else if(C[leader][i] != -1) return 0;// Invalid leader. Since, 'leader' is not followed by i.
		}
	}
	
	// Leader found
	return leader+1;
}

int main(){
	int n;
	cin>>n;
	vector<vector<int>> C(n,vector<int>(n));
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cin>>C[i][j];
		}
	}
	
	int leader = StableCommunity(C);
	if(leader) cout<<"\nGiven Community has leader = "<<leader<<" and is hence stable.\n";
	else cout<<"\nCommunity doesn't have a leader and is not stable.\n";

	return 0;
}
