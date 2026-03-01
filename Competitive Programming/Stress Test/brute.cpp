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
#define ceil(a,b) (((a)+(b)-1)/(b))

static constexpr int M = 1e9+7;
#define OPR operator
struct Mint{
	int x;
	Mint(int a=0): x(a){}		// Assumes `a` always in range [0,M-1]
	Mint(ll a): x(a%M){}

	Mint OPR+(const Mint &other)const{ int y=x+other.x; return Mint((y<M)? y:y-M);}
	Mint OPR-(const Mint &other)const{ int y=x-other.x; return Mint((y>=0)? y:y+M);}
	Mint OPR*(const Mint &other)const{ return Mint(x*1ll*other.x);}
	Mint OPR^(const Mint &other)const{ int ans=1,b=other.x,a=x; while(b){if(b&1) ans=(ans*1ll*a)%M; a=(a*1ll*a)%M; b>>=1;} return ans;}
	Mint OPR/(const Mint &other)const{ return (*this)*(other^(M-2));}   // Only for Prime MOD
	Mint& OPR+=(const Mint &other){ x+=other.x; if(x>=M) x-=M; return *this;}
	Mint& OPR-=(const Mint &other){ x-=other.x; if(x<0) x+=M; return *this;}
	Mint OPR++(int){ Mint temp=*this; *this+=1; return temp;}
    Mint& OPR++(){ if(++x==M) x=0; return *this; }
	Mint OPR--(int){ Mint temp=*this; *this-=1; return temp;}
    Mint& OPR--(){ if(--x==-1) x=M-1; return *this; }
	Mint& OPR*=(const Mint &other){ x=(x*1ll*other.x)%M; return *this;}
	Mint& OPR^=(const Mint &other){ *this=*this^other; return *this;}
	Mint& OPR/=(const Mint &other){ *this=*this/other; return *this;}
	bool OPR==(const Mint &other)const{ return x==other.x;}
	bool OPR!=(const Mint &other)const{ return x!=other.x;}
	bool OPR<(const Mint &other)const{ return x<other.x;}
	bool OPR>(const Mint &other)const{ return x>other.x;}
	bool OPR<=(const Mint &other)const{ return x<=other.x;}
	bool OPR>=(const Mint &other)const{ return x>=other.x;}

	friend istream &OPR>>(istream &in,Mint &m){ in>>m.x; return in;}
	friend ostream &OPR<<(ostream &out,const Mint &m){ out<<m.x; return out;}
};
inline Mint OPR+(int a, const Mint &b){ return Mint(a)+b;}
inline Mint OPR-(int a, const Mint &b){ return Mint(a)-b;}
inline Mint OPR*(int a, const Mint &b){ return Mint(a)*b;}
inline Mint OPR^(int a, const Mint &b){ return Mint(a)^b;}
inline Mint OPR/(int a, const Mint &b){ return Mint(a)/b;}
inline bool OPR==(int a, const Mint &b){ return a==b.x;}
inline bool OPR!=(int a, const Mint &b){ return a!=b.x;}
inline bool OPR<(int a, const Mint &b){ return a<b.x;}
inline bool OPR>(int a, const Mint &b){ return a>b.x;}
inline bool OPR<=(int a, const Mint &b){ return a<=b.x;}
inline bool OPR>=(int a, const Mint &b){ return a>=b.x;}

bool Regular(string &s){
	int pf=0;
	for(char c:s){
		if(c=='(') pf++;
		else{
			pf--;
   			if(pf<0) return false;
		}
	}
	return true;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
    int T=1,n,m,k;
    cin>>T;
    while(T--){
    	cin>>n;
    	string s;
    	cin>>s;
    	Mint ans=0;
    	f(n){
    		int count=0;
    		if(s[i]=='(') ans+=2^Mint(i);
			else{
				for(int mask=0;mask<(1<<i);mask++){
		    		string p=s;
					vi v;
		    		for(int bit=mask;bit;bit&=bit-1) v.pb(__builtin_ctz(bit));
		    		v.pb(i);
					for(int j=v.size()-1;j;j--) swap(p[v[j]],p[v[j-1]]);
					if(Regular(p)){
						ans++;
						count++;
					}
				}
			}
			cout<<count<<" ";
		}
		cout<<ans<<"\n";
	}
	return 0;
}
