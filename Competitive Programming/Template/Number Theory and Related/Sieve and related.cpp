#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int,int> pi;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef map<int,int> mi;
typedef map<ll,ll> mll;
typedef set<int> si;
typedef set<ll> sll;
typedef multiset<int> msi;
typedef multiset<ll> msll;
#define F first
#define S second
#define pb push_back
#define ai(a,n) for(int j=0;j<(n);j++) cin>>a[j]
#define ao(a,n) for(int j=0;j<(n);j++) cout<<a[j]<<" "
#define f(n) for(int i=0;i<(n);i++)
#define unique(v) sort(v.begin(),v.end()); v.resize(unique(v.begin(),v.end())-v.begin());
#define mp(a,b) make_pair((a),(b)) 
#define YN(flag) cout<<(((flag))? "YES\n":"NO\n");
#define ceil(a,b) ((a+b-1)/b)

const int N=2e5+1;
bool not_prime[N];
int hp[N];
vi divisors[N],primes;
void divisor(){ 
    for(int i=1;i<N;i++) for(int j=i;j<N;j+=i) divisors[j].pb(i);
}
void sieve(){
    not_prime[0]=not_prime[1]=true;
    for(int j=4;j<N;j+=2) not_prime[j]=true;
    int i=3;
    primes.pb(2);
    for(;i*i<N;i+=2){
		if(!not_prime[i]){
			primes.pb(i);
			for(int j=i*i;j<N;j+=(i<<1)) not_prime[j]=true;
		}
	}
    for(;i<N;i+=2) if(!not_prime[i]) primes.pb(i);
}
void HP(){
	hp[1]=1;
	for(int i=2;i<N;i++){
		if(!hp[i]){
			for(int j=i;j<N;j+=i) hp[j]=i;
		}
	}
}
vector<pi> prime_factors(int n){
	// Initialize hp[] First if not defined earlier.
	vector<pi> primef;
	while(n>1){
		int pf=hp[n],cnt=0;
		while(!(n%pf)){
			n/=pf;
			cnt++;
		}
		primef.pb({pf,cnt});
	}
	return primef;
}
vector<pi> prime_factor(int n){
	// Use Factorizer Template for faster factorization
	vector<pi> &primef;
	for(int i=2;i*i<=n;i++){
		int cnt=0;
		while((n%i)==0){
			cnt++;
			n/=i;
		}
		if(cnt) primef.pb({i,cnt});
	}
	if(n>1) primef.pb({n,1});
	return primef;
}
int main(){
	divisor();
	sieve();
	HP();
	return 0;
}
