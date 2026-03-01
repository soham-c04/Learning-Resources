#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define f(n) for(int i=0;i<(n);i++)

const int N=2e5+1;
int phi[N];

int ETF(int n, vector<int> &prime_factors){
	for(int p:prime_factors) n -= n/p;
	return etf;
}

void ETF(){
	f(N) phi[i]=i;
	for(int i=2;i<N;i++) if(phi[i]==i) for(int j=i;j<N;j+=i) phi[j]-=phi[j]/i;
}

int Phi(int n){
	// Finds in O(sqrt(N)) for a single n.
	int ans=n;
	for(int i=2;i*i<=n;i++){
		if(!(n%i)){
			ans-=ans/i;
			while(!(n%i)) n/=i;
		}
	}
	if(n>1) ans-=ans/n;
	return ans;
}

// Smallest Inverse of ETF(n). Works at 50ms for n <= 1e9
// Problem - https://acm.timus.ru/problem.aspx?space=1&num=1673.   Submission - https://acm.timus.ru/getsubmit.aspx/10888461.cpp
// min(Inv_Phi[n]) <= f*n;     f <= 2.1 (for n <= 1e7)
// max(Inv_Phi[n]) <= f*n;	   f <= 8.25 (for n <= 1e7)
/*
Possible Optimizations (ONLY IF, multiple test cases are present).
map<int,bool> prime; [ If same prime_check() called multiple times]
Using Sieve if n is small for prime_checks.

map<pi,ll> dp; [ dp[{n,min_p}]=ans ]
Preprocessing for *Non-zero* inv_phi[] for smaller n ---> if(!inv_phi[phi[i]]) inv_phi[phi[i]]=i;
inv_phi[n] = 0, can be determined by checking upto phi[2.1*n].
*/

bool is_prime(int n){ // Better prime checkers to be used for tighter constraints
	if(n==2) return true;
	else if(!(n&1)) return false;
	for(int i=3;i*i<=n;i+=2) if(!(n%i)) return false;
	return true;
}
ll Inv_Phi(int n,int min_p){
	if(n==1) return 1;
	else if(n==2) return 3;
	else if(n&1 || n<=min_p) return 0;
	else if(is_prime(n+1)) return n+1;
	
	ll mn=1e18;
	for(int d=2;d*d<=n;d++){ // Use Factorizer:: for tigher constraints
		if(!(n%d)){
			int p=d+1;
			if(p>min_p && is_prime(p)){
				int N=n/(p-1);
				ll x=p,t=Inv_Phi(N,p);
				if(t) mn=min(mn,t*x);
				while(!(N%p)){
					N/=p;
					x*=p;
					t=Inv_Phi(N,p);
					if(t) mn=min(mn,t*x);
				}
			}
			if(d*d!=n){
				p=n/d+1;
				if(p<=min_p) break;
				else if(is_prime(p)){
					int N=n/(p-1);
					ll x=p,t=Inv_Phi(N,p);
					if(t) mn=min(mn,t*x);
					while(!(N%p)){
						N/=p;
						x*=p;
						t=Inv_Phi(N,p);
						if(t) mn=min(mn,t*x);
					}
				}
			}
		}
	}
	if(mn==1e18) mn=0;
	return mn;
}

int main(){
	ETF();
	cout<<Phi(1e9)<<"\n";
	cout<<Inv_Phi(168,2)<<" "<<Inv_Phi(144,2)<<" Edge Cases\n";
	return 0;
}
