#include <bits/stdc++.h>
using namespace std;

struct Exception{
	static void TLE(bool cond=true){
		if(!cond) return;
		long long x=0;
		while(true) x+=2026;
	}

	static void MLE(bool cond=true){
		if(!cond) return;
		const int N=1e5, M=1e5;
		vector<vector<long long>> mle(M, vector<long long>(N,M+N));
	}

	static void RE(bool cond=true){
		if(!cond) return;
		exit('R'+'E');
	}
};
