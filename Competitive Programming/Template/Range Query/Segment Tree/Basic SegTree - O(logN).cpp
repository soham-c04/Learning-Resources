#include<bits/stdc++.h>;
using namespace std;
typedef vector<int> vi;
#define ai(a,n) for(int j=0;j<n;j++) cin>>a[j];
#define f(n) for(int i=0;i<(n);i++)

typedef class SegmentTree{
	typedef long long T;    // datatype of seg[].
	private:
		static const int N=2e5;
		/* --- For reusing SegmentTree use `vector<int> a,seg` and remove "inline static" ---*/
		int n;
		inline static int a[N];
		inline static T seg[4*N];

		static constexpr T IDENTITY=0;      // Defined as: combine(a,IDENTIY) = a;
		inline static T combine(T a, T b){
			return a+b;
		}
		inline T leaf(int ind){
			return a[ind];
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
		T Query(int v,int lo,int hi,int qlo,int qhi){
			if(qlo>hi || qhi<lo) return IDENTITY;
			if(lo>=qlo && hi<=qhi) return seg[v];
			int mid=(lo+hi)/2;
			int l=2*v+1,r=2*v+2;
			return combine(Query(l,lo,mid,qlo,qhi),Query(r,mid+1,hi,qlo,qhi));
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

		/*--- RangeUpdate() + lazy[] incomplete ---*/
		/*--- Assumes sum() by default ---*/
		bool is_lazy=false;
		inline static T lazy[4*N];          // Set it to `lazy[1]` if lazy not used
		void RangeUpdate(int ind,int lo,int hi,int qlo,int qhi,int delta){
			int lChild=2*ind+1,rChild=2*ind+2;
			if(hi==lo) seg[ind]+=lazy[ind];
			else{
				lazy[lChild]+=lazy[ind];
				lazy[rChild]+=lazy[ind];
			}
			lazy[ind]=0;
			if(lo>qhi || hi<qlo) return;
			if(qhi>=hi && qlo<=lo){
				if(hi==lo) seg[ind]+=delta;
				else{
					lazy[rChild]+=delta;
					lazy[lChild]+=delta;
				}
				return;
			}
			int mid=(hi+lo)/2;
			RangeUpdate(lChild,lo,mid,qlo,qhi,delta);
			RangeUpdate(rChild,mid+1,hi,qlo,qhi,delta);
		}
		T RangeQuery(int v,int lo,int hi,int qlo,int qhi){
			return 'L'+'A'+'Z'+'Y';
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

		T query(int l, int r){ return is_lazy? RangeQuery(0,0,n-1,l,r):Query(0,0,n-1,l,r); }
		void update(int ind, int value){
			a[ind] = value;
			Update(0,0,n-1,ind);
		}
		void update(int l, int r, int delta){
			if(is_lazy) RangeUpdate(0,0,n-1,l,r,delta);
			else if(l==r) update(l,a[l]+delta);
			else{
				memset(lazy,0,4*n*sizeof(T));
				is_lazy=true;
				RangeUpdate(0,0,n-1,l,r,delta);
			}
		}
} ST;

int main(){
	int n,q;
	cin>>n>>q;
	int a[n];
	ai(a,n);
	SegmentTree seg(a,n);
	while(q--){
		int l,r;
		cin>>l>>r;
		l--;r--;
		int ans=seg.query(l,r);
		// seg.update(index, value) to update a[index]=value
		cout<<ans<<"\n";
	}
	return 0;
}
