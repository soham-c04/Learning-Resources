#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<ll> vll;
#define f(n) for(int i=0;i<(n);i++)

typedef unsigned long long ull;
typedef __int128_t _int;
typedef __uint128_t _uint;

static inline ll mul_M61(ll a, ll b){         // Mersenne Prime only
    static constexpr ll M61 = (1LL<<61)-1;
    _int t = (_int)a*b;
    ll res = (ll)(t>>61)+(ll)(t&M61);
    if(res>=M61) res-=M61;
    return res;
}
static inline ll mul_mod2(ll a, ll b, const ll MOD){	// Assumes MOD = 2^63 - c
	const _int c = (1ULL<<63)-MOD;                		// Condition: c(c+1) < MOD
	static constexpr _int MASK=(1ULL<<63)-1;

    _int t = (_int)a*b;
    // First fold
    t = (t>>63)*c + (t&MASK);
    // Second fold
    t = (t>>63)*c + (t&MASK);

    if(t>=MOD) t-=MOD;
    return t;
}

/*
--------------- Horner's Method Used (sometimes slower performance, rare) ---------------

===== Do - #define mul_M61(a,b) mul_mod2(a,b,mod[0]) [If mod[0]!=M61 needs to be necessarily included] =======
*/

typedef class StringHash1{
	private:
		inline static const ll mod = (1ll<<61)-1, base = 31;	// Mersenne Prime := M61
		inline static vll power;                                // Use `inline static ll [N]` for possible performance boost.

		static void precompute(int N){
			if(N<=power.size()) return;
			power.resize(N);
			ll p=1;
			f(N){
				power[i] = p;
				p = mul_M61(p,base);
			}
	    }

		int n;
		string s;
		vll hashval;            			// Change to `inline static ll [N]` for better performance.

		void compute_hash(){
			hashval.resize(n+1);
			hashval[0] = 0;
			ll val=0;
			f(n){            				
				val = mul_M61(val,base)+(s[i]-'a'+1);
				if(val>=mod) val-=mod;
				hashval[i+1] = val;
			}
		}
	public:
		StringHash1(const string &s){
			n = s.size();
			this->s = s;
			precompute(s.size());
			compute_hash();
		}

		static inline ll hash(const string &p){
			ll hv=0;
			for(char ch:p){
				hv = mul_M61(hv,base)+(ch-'a'+1);
				if(hv>=mod) hv-=mod;
			}
			return hv;
		}

		inline ll substring_hash(const int l,const int r){
			ll val = hashval[r+1] - mul_M61(hashval[l],power[r-l+1]);
			if(val<0) val+=mod;
			return val;
		}
} SHA1;

typedef class StringHash{       // Multiple mods + base (slower). If Using single mod,base. Use SHA1 (much faster).
	private:
		static constexpr ll LIMIT = (1ULL<<63)-1;		// LIMIT < sqrt(range(_int))
		/* --- Use "static" only if all instances of StringHash use the same values. Otherwise remove precompute() --- */
		/* --- If "N, mod, base" are runtime variables then remove "const" keywords. Use Vector<> instead of int []. --- */
		static constexpr int N = 1e5+10;
		static constexpr int nmod = 3, nbase = 2;
		static constexpr ll mod[nmod] = {(1ll<<61)-1, LIMIT-24, LIMIT-164};	 // mod[0] should be always be M61
		static constexpr int base[nbase] = {31, 37};
		inline static ll power[nmod][nbase][N];			// power[m][b][i] = (b^i) % mod[m] --- Required for substring match

		inline static bool precompute_ = false;    	// Ensures precompute() called only once.
		static void precompute(){
			precompute_ = true;
			for(int b_i=0; b_i<nbase; b_i++){
				ll p=1, b=base[b_i];
				f(N){
					power[0][b_i][i] = p;
					p = mul_M61(p,b);
				}
			}
   			for(int m_i=1; m_i<nmod; m_i++){
   				const ll m=mod[m_i];
				for(int b_i=0; b_i<nbase; b_i++){
					ll p=1, b=base[b_i];
					f(N){
						power[m_i][b_i][i] = p;
						p = mul_mod2(p,b,m);
					}
				}
			}
	    }

		int n;
		string s;
		inline static ll hashval[N][nmod*nbase];

		void compute_hash(){
			int j=0;
			for(int b_i=0; b_i<nbase; b_i++, j++){
				ll val=0, b=base[b_i];
				hashval[0][j] = 0;
				f(n){            				
					val = mul_M61(val,b)+(s[i]-'a'+1);
					if(val>=mod[0]) val-=mod[0];
					hashval[i+1][j] = val;
				}
			}
			for(int m_i=1; m_i<nmod; m_i++){
				const ll m=mod[m_i];
				for(int b_i=0; b_i<nbase; b_i++, j++){
					ll val=0, b=base[b_i];
					hashval[0][j] = 0;
	 				f(n){
						val = mul_mod2(val,b,m)+(s[i]-'a'+1);
						if(val>=m) val-=m;
						hashval[i+1][j] = val;
					}
				}
			}
		}
	public:
		StringHash(string &s){
			n = s.size();
			this->s = s;
			if(!precompute_) precompute();
			compute_hash();
		}

		static array<ll, nmod*nbase> hash(string &p){
			array<ll, nmod*nbase> hv{};
			int j=0;
			for(int b_i=0; b_i<nbase; b_i++, j++){
				ll val=0, b=base[b_i];
				for(char ch:p){
					val = mul_M61(val,b)+(ch-'a'+1);
	 				if(val>=mod[0]) val-=mod[0];
				}
				hv[j] = val;
			}
			for(int m_i=1; m_i<nmod; m_i++){
				const ll m=mod[m_i];
				for(int b_i=0; b_i<nbase; b_i++, j++){
					ll val=0, b=base[b_i];
					for(char ch:p){
						val = mul_mod2(val,b,m)+(ch-'a'+1);
						if(val>=m) val-=m;              	// Can be skipped if [val modM+26<M]. But, Risky (probabilistically).
					}
					hv[j] = val;
				}
			}
			return hv;
		}

		array<ll, nmod*nbase> substring_hash(int l, int r){
			array<ll, nmod*nbase> hv{};
			int j=0;
			for(int bi=0; bi<nbase; bi++, j++){
				ll val = hashval[r+1][j] - mul_M61(hashval[l][j],power[0][bi][r-l+1]);
				if(val<0) val+=mod[0];
				hv[j] = val;
			}
			for(int mi=1; mi<nmod; mi++){
				const ll m=mod[mi];
				for(int bi=0; bi<nbase; bi++, j++){
					ll val = hashval[r+1][j] - mul_mod2(hashval[l][j],power[mi][bi][r-l+1],m);
					if(val<0) val+=m;
					hv[j] = val;
				}
			}
			return hv;
		}
} SHA;

int main(){
	return 0;
}
