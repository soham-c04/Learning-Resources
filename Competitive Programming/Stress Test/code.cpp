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

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
    int T,n,m,k;
    cin>>T;
    while(T--){
    	cin>>n;
    	bool a[n][n], pos = true;
        f(n){
			string s;
			cin>>s;
			for(int j=0;j<n;j++) a[i][j]=(s[j]=='1');
			if(!a[i][i]) pos = false;
		}
		vector<pi> ans;
		if(pos){
			vi ug[n];
			f(n){
				for(int j=0;j<n && pos;j++){
					if((i==j) || (!a[i][j])) continue;
					ug[i].pb(j);
					ug[j].pb(i);
					bool direct = true;
					for(int k=0;k<n;k++){
						if(a[j][k] && (((!a[i][k]) || a[k][i]))){
							pos=false;
							break;
						}
						if((k!=i) && (k!=j) && a[i][k] && a[k][j]) direct = false;
					}
					if(direct) ans.pb({i+1,j+1});
				}
				if(!pos) break;
			}
			if(pos){
				bool vis[n]={0};
				vis[0] = true;
				stack<int> stk;
				stk.push(0);
				while(stk.size()){
					int u = stk.top();
					stk.pop();
					for(int v:ug[u]){
						if(!vis[v]){
							vis[v] = true;
							stk.push(v);
						}
					}
				}
				f(n){
					if(!vis[i]) pos = false;
				}
			}
		}
		if(!pos) cout<<"ok\n";
		else{
			cout<<"ok\n";
//			for(auto [a,b]:ans) cout<<a<<" "<<b<<"\n";
			if(ans.size()!=n-1) return 1;
		}
	}
	return 0;
}
