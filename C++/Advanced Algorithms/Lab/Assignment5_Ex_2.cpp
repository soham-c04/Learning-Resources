#include <bits/stdc++.h>
using namespace std;

int main(){
	int n;
	cout<<"Enter the number of intervals: ";
	cin>>n;
	pair<int,bool> timestamp[2*n];
	cout<<"Enter the intervals:-\n";
	for(int i=0;i<n;i++){
		cin>>timestamp[i].first;
		timestamp[i].second = false;
		cin>>timestamp[n+i].first;
		timestamp[n+i].second = true;
	}
	
	sort(timestamp,timestamp+2*n);
	int active_intervals = 0;
	int max_clique = 0;
	for(int i=0;i<2*n;i++){
		if(!timestamp[i].second){
			active_intervals++;
			max_clique = max(max_clique,active_intervals);
		}
		else active_intervals--;
	}
	
	cout<<"\nMaximum Clique size = "<<max_clique<<"\n";
	
	return 0;
}
