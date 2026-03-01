#include <bits/stdc++.h>
using namespace std;
typedef pair<int,int> pi;
#define F first
#define S second

#define OPR operator
istream& OPR>>(istream& in,pi &p){ in>>p.F>>p.S; return in;}
ostream& OPR<<(ostream& out,const pi &p){ out<<"("<<p.F<<","<<p.S<<")"; return out;}
pi OPR+(const pi &p,const pi &q){return {p.F+q.F,p.S+q.S};}
pi OPR-(const pi &p,const pi &q){return {p.F-q.F,p.S-q.S};}
pi OPR-(const pi &p){return {-p.F,-p.S};}
pi OPR+=(pi &p,const pi &q){return p=p+q;}
pi OPR-=(pi &p,const pi &q){return p=p-q;}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    pi p1={1,6},p2={4,5};
    cout<<"Pairs overleading: "<<p1+p2<<" "<<p1-p2<<"\n\n";
    
	return 0;
}
