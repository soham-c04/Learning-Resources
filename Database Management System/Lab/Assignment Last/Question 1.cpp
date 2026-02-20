#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
#define pb push_back
#define F first
#define S second

// The following reference is used for implementing the algorithm - https://github.com/kris-brown/chase
// Youtube video link :- https://www.youtube.com/watch?v=YNsvuPA_vf0&t=740s

/*
Example input 1:
3
2
2 1 2
2 1 3
1
1 1 2
- Lossless Decomposition

Example input 2:
3
2
2 1 2
2 2 3
1
1 1 2
- Lossy Decomposition
*/

int main(){
	cout<<"Enter the number of columns in R: ";
	int k;
	cin>>k;
	cout<<"\nNote that columns will be termed as 1,2,...,k instead of A,B,C.\n\n";
	cout<<"Enter the number of tables after final decomposition D: ";
	int n;
	cin>>n;
	cout<<"For each decomposition input the columns as - (number of columns in R_i) [j_th attribute of R_i...].\n";
	vi D[n];
	for(int i=0;i<n;i++){
		cout<<"R_"<<i+1<<": ";
		int a,c;
		cin>>c;
		while(c--){
			cin>>a;
			D[i].pb(a-1);
		}
		sort(D[i].begin(),D[i].end());
	}
	cout<<"\nEnter the number of functional dependencies: ";
	int m;
	cin>>m;
	cout<<"\nEnter functional dependencies as: (number of attributes on left side [3 here]) 4 1 2 3, meaning (4,1,2) -> 3.\nOnly one column should be on right side.\n";
	pair<vi,int> FD[m];
	for(int i=0;i<m;i++){
		cout<<"FD["<<i+1<<"]: ";
		int a,c;
		cin>>c;
		while(c--){
			cin>>a;
			FD[i].F.pb(a-1);
		}
		cin>>a;
		FD[i].S = a-1;
	}

	// Algorithm start
	int color = 1;
	int chase[n][k];
	memset(chase,0,4*n*k);

	for(int i=0;i<n;i++) for(int a:D[i]) chase[i][a]=color;
	color++;

	bool change = true, lossless = false;
	while(change && !lossless){
		change = false;
		for(int t=0;t<m && !change;t++){
			vi l = FD[t].F;
			int r = FD[t].S;
			vi match;
			for(int i=0;i<n;i++){
				bool subset=true;
				for(int a:l){
					if(!chase[i][a]){
						subset = false;
						break;
					}
				}
				if(subset) match.pb(i);
			}
			int lowest = 1e9;
			for(int i:match) if(chase[i][r]) lowest=min(lowest,chase[i][r]);
			if(lowest==1e9){
				change = true;
				for(int i:match) chase[i][r]=color;
				color++;
			}
			else{
				for(int i:match){
					if(chase[i][r]!=lowest){
						change=true;
						chase[i][r]=lowest;
					}
				}
			}
		}
		cout<<"\n";

		for(int i=0;i<n;i++){
			for(int j=0;j<k;j++) cout<<chase[i][j]<<" ";
			cout<<"\n";
		}
		cout<<"\n";

		// Lossless check
		for(int i=0;i<n && !lossless;i++){
			lossless=true;
			for(int j=0;j<k;j++){
				if(chase[i][j]!=1){
					lossless = false;
					break;
				}
			}
		}
	}

	cout << ((lossless)? "Lossless decomposition.\n":"Lossy Decompostion.\n");

	return 0;
}
