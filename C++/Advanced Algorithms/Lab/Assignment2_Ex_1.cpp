#include <bits/stdc++.h>
using namespace std;

// Time Complexity - O(NlogN)
// If all elements a[i] <= N, then it can be solved in O(N) using hashing.

// Space Complexity - O(1) [no extra space used apart from initial array]

pair<int,int> CompatiblePair(int a[], int n, int T){
	sort(a,a+n);
	for(int l=0, r=n-1; l<r;){     // Two pointer approach. Classic Problem.
		if(a[l]+a[r] == 2*T) return {a[l],a[r]};
		else if(a[l]+a[r] < 2*T) l++;
		else r--;
	}
	return {-1,-1};
}

int main(){
	int n;
	cin>>n;
	int a[n];
	for(int i=0;i<n;i++) cin>>a[i];
	int T;
	cin>>T;
	
	pair<int,int> ans = CompatiblePair(a, n, T);
	if(ans.first == -1) cout<<"-1\n";
	else cout<<ans.first<<" "<<ans.second<<"\n";

	return 0;
}
