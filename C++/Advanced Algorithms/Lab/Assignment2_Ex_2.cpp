#include <bits/stdc++.h>
using namespace std;

const int INF = 2e9;    // Set INF > max(a_i) . This is just for ease of implementation

// Time Complexity - O(NlogN)
// Space Complexity - O(N) [for creating tempL and tempR arrays]

void Inversions(pair<pair<int,int>,int> output[], int l, int r){
	if(l>=r) return;
	int mid = (l+r)/2;
	Inversions(output, l, mid);
	Inversions(output, mid+1, r);

	int Lsize = mid-l+1, Rsize = r-mid;
	pair<pair<int,int>,int> tempL[Lsize+1], tempR[Rsize+1];

	for(int i=0;i<Lsize;i++) tempL[i] = output[i+l];
	for(int i=0;i<Rsize;i++) tempR[i] = output[i+mid+1];
	tempL[Lsize] = tempR[Rsize] = {{-1,0},-INF};     // -INF appended to last for ease of implementation
	
	for(int i=l, L=0, R=0; i<=r; i++){
		if(tempL[L].second < tempR[R].second){
			output[i] = tempR[R];
			R++;
		}
		else{
			output[i] = tempL[L];
			output[i].first.second += Rsize-R;
			L++;
		}
		
		// For counting j < i, such that a[i] < a[j] :-

		/*
		// Change last value of tempL, tempR to INF, instead of -INF.

		if(tempL[L].second < tempR[R].second){
			output[i] = tempL[L];
			L++;
		}
		else{
			output[i] = tempR[R];
			output[i].first.second += Lsize-L;
			R++;
		}
		*/
	}
}

int main(){
	int n;
	cin>>n;
	int a[n];
	for(int i=0;i<n;i++) cin>>a[i];

	pair<pair<int,int>,int> output[n];  // output[i] = {{original_index, disorder_of_a[i]}, value_a[i]}
	for(int i=0;i<n;i++) output[i] = {{i,0},a[i]};
	
	Inversions(output, 0, n-1);
	sort(output, output+n);     // Sorted because output[n] will be in descending order of a[i]. sorting it arranges them back in original order.
	for(int i=0;i<n;i++) cout<<output[i].first.second<<" ";
	cout<<"\n";

	return 0;
}
