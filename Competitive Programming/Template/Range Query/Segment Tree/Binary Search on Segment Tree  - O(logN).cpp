#include<bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
#define ai(a,n) for(int j=0;j<(n);j++) cin>>a[j];
#define f(n) for(int i=0;i<(n);i++)

// Binary Search + SegTree - combined only into SegTree - O(logN) instead of O(logN*logN)
// Other Query variants are given below

typedef class SegmentTree{
	#define sum first
	#define count second
	typedef pair<long long,int> T;    			// datatype of seg[].
	private:
		static const int N=2e5;
		/* --- For reusing SegmentTree use `vector<int> a,seg` and remove "inline static" ---*/
		int n;
		inline static int a[N];
		inline static T seg[4*N];

		static constexpr T IDENTITY={0,0};      // Defined as: combine(a,IDENTIY) = a;
		inline static T combine(T a, T b){
			return {a.sum+b.sum, a.count+b.count};
		}
		inline T leaf(int ind){         		// count = no. of elements > some_fixed_value in the range.
			return {a[ind], (a[ind]>0)? 1:0};
		}

		void Build(int v,int lo,int hi){
			if(lo==hi) seg[v]=leaf(lo);
			else{
				int mid=(lo+hi)/2;
				int l=2*v+1,r=2*v+2;
				Build(l,lo,mid);
				Build(r,mid+1,hi);
				seg[v]=combine(seg[l],seg[r]);
			}
		}
		T Query(int v,int lo,int hi,int qlo,int qhi, int x){        // Query for sum for first x non-zero elements. Verification - https://codeforces.com/contest/2200/submission/365482610
			if(qlo>hi || qhi<lo) return IDENTITY;
			if(x<=0) return IDENTITY;
			if(seg[v].count<x) return seg[v];
			if(lo==hi) return leaf(lo);
			int mid=(lo+hi)/2;
			int l=2*v+1,r=2*v+2;
			T c=Query(l,lo,mid,qlo,qhi,x);
			if(c.count==x) return c;
			return combine(c,Query(r,mid+1,hi,qlo,qhi,x-c.count));
		}
		void Update(int v,int lo,int hi,int pos){
			if(lo==hi) seg[v]=leaf(pos);
			else{
				int mid=(lo+hi)/2;
				int l=2*v+1,r=2*v+2;
				if(pos<=mid) Update(l,lo,mid,pos);
				else Update(r,mid+1,hi,pos);
				seg[v]=combine(seg[l],seg[r]);
			}
		}

	public:
  		SegmentTree(int arr[], int n){
			this->n=n;
			f(n) a[i]=arr[i];
			Build(0,0,n-1);
		}
		SegmentTree(vi &v){
			this->n=v.size();
			f(n) a[i]=v[i];
			Build(0,0,n-1);
		}

		T query(int l, int r, int x){ return Query(0,0,n-1,l,r,x); }
		void update(int ind, int value){
			a[ind] = value;
			Update(0,0,n-1,ind);
		}
} ST;

/*
Searching for the 'index' of first element > x:

int Query(int v,int lo,int hi,int qlo,int qhi,int x){
	if(qlo>hi || qhi<lo) return -1;
	if(seg[v]<=x) return -1;
	if(lo==hi) return lo;
	int mid=(lo+hi)/2;
	int l=2*v+1,r=2*v+2;
	int c=Query(l,lo,mid,qlo,qhi,x); // If you want to search for the last element > x query for the right child first.
	if(c!=-1) return c;
	return Query(r,mid+1,hi,qlo,qhi,x);
}
*/

int main(){
	int n,q;
	cin>>n>>q;
	int a[n];
	ai(a,n);
	ST seg(a,n);
	while(q--){
		int l,r,x;
		cin>>l>>r>>x;
		l--;r--;
		int ans=seg.query(l,r,x).sum;
		cout<<ans<<"\n"; // -1 means no elements >=x found in the given range
	}
	return 0;
}
