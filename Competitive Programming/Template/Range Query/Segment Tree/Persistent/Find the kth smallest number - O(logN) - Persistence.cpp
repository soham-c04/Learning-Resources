#include <bits/stdc++.h>
using namespace std;
#define ai(a,n) for(int j=0;j<n;j++) cin>>a[j];

struct Vertex{
    Vertex *l,*r;
    int sum;
    Vertex(int val) : l(nullptr), r(nullptr), sum(val) {}
    Vertex(Vertex *l, Vertex *r) : l(l), r(r), sum(0) {
        if(l) sum+=l->sum;
        if(r) sum+=r->sum;
    }
};
Vertex* build(int a[],int lo,int hi) {
	if(lo==hi) return new Vertex(a[lo]);
	int mid=(lo+hi)/2;
	return new Vertex(query(a,lo,mid),Vertex(a,mid+1,hi));
}

int query(Vertex* v,int lo,int hi,int qlo,int qhi,int &k){
	if(lo>qhi || hi<qlo) return -1;
	if((v->sum)<k){
		k-=(v->sum);
		return -1;
	}
	if(lo==hi){
		k=0;
		return lo;
	}
	int mid=(lo+hi)/2;
	int c=query(v->l,lo,mid,qlo,qhi,k);
	if(k) return query(v->r,mid+1,hi,qlo,qhi,k);
	else return c;
}

Vertex* update(Vertex* v,int lo,int hi,int pos,int new_val){
    if (lo==hi) return new Vertex(new_val);
    int mid=(lo+hi)/2;
    if (pos<=mid) return new Vertex(update(v->l,lo,mid,pos,new_val),v->r);
    else return new Vertex(v->l,update(v->r,mid+1,hi,pos,new_val));
}


int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	/*
	Merge Sort Tree is used
	*/
	int T,n,q;
	cin>>n>>q;
	ai(a,n);
	build(0,0,n-1);
	while(q--){
		int l,r,k;
		cin>>l>>r>>k; // k<=(r-l+1) assumed
		l--;r--;
		int ans=query(0,0,n-1,l,r,k);
		cout<<ans<<"\n";
	}
	return 0;
}
