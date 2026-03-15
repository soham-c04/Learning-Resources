#include<bits/stdc++.h>
#include<ext/random>
using namespace std;
#define f(n) for(int i=0;i<(n);i++)
#define ao(a,n) for(int j=0;j<(n);j++) cout<<a[j]<<" ";
#define pb push_back
#define F first
#define S second
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pi;

using RNG = __gnu_cxx::sfmt19937;
//decltype(RNG()()) x;
ll rand_num(ll a, ll b){
	random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<ll> distrib(a, b);
    ll result = distrib(gen);
    return result;
}

int main(int argc, char* argv[]){
	cout<<"1\n";
	int n = rand_num(2,5);
	cout<<n<<"\n";
	f(n){
		for(int j=0;j<n;j++) cout<<rand_num(0,1);
		cout<<"\n";
	}
	return 0;
}
