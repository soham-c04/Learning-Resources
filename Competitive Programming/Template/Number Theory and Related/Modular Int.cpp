#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef vector<int> vi;
typedef pair<int,int> pi;
#define F first
#define S second

// For modulo
static constexpr int M = 1e9+7;

int mod_mul(vi v, int MOD=M){int ans=1; for(int i:v){ ans=(ans*1ll*i)%MOD;} return ans;}
int expo(int a, int b,int MOD=M){int ans=1; while(b){if(b&1) ans=(ans*1ll*a)%MOD; a=(a*1ll*a)%MOD; b>>=1;} return ans;}
//Only for prime MOD:
int mod_div(int a, int b,int MOD=M){return (a*1ll*expo(b, MOD-2, MOD))%MOD;}
// If MOD is not prime -> Replace `expo(b, c, MOD-1)` with `expo(b, c, ETF(MOD))`
int super_expo(int a, int b, int c, int MOD=M) {int res=expo(b, c, MOD-1); res=expo(a,res,MOD); return res;}

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    Mint a=9,b=M-2;
    cout<<"Modulo -  "<<a<<" "<<b<<":-\n";
    cout<<a+b<<" add\n";
    cout<<a-b<<" sub\n";
    cout<<a*b<<" mul\n";
    cout<<(a^b)<<" power\n";
    cout<<a/b<<" div\n";
    a/=b;
    cout<<a;
    
	return 0;
}
