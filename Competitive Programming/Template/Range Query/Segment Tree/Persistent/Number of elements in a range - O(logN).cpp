#include <bits/stdc++.h>
using namespace std;

typedef class PersistentSegmentTree{
	typedef int D;          // Datatype of a[]
	typedef long long T;    // Datatype of Node
	private:
		const int N = 2e5;
		int n;
		inline static int a[N];
		
		struct Node{
		    Node *l, *r;
		    T sum;

		    Node(T val) : l(nullptr), r(nullptr), sum(val) {}
		    Node(Node *l, Node *r) : l(l), r(r), sum(0) {
		        if(l) sum += l->sum;
		        if(r) sum += r->sum;
		    }
		};

		Node* build(D a[], int lo, int hi) {
		    if(lo==hi) return new Node(a[lo]);
		    int mid = (lo+hi)/2;
		    return new Node(build(a, tl, tm), build(a, tm+1, tr));
		}

	public:
		PersistentSegmentTree(int arr[], int n){
			this->n=n;
			memcpy(a, arr, sizeof(arr));
			Build(0,0,n-1);
		}
		PersistentSegmentTree(vi &v){
			this->n=v.size();
			copy(v.begin(), v.end(), a);
			Build(0,0,n-1);
		}
		
		int query(Node* v, int tl, int tr, int l, int r) {
		    if (l > r)
		        return 0;
		    if (l == tl && tr == r)
		        return v->sum;
		    int tm = (tl + tr) / 2;
		    return get_sum(v->l, tl, tm, l, min(r, tm))
		         + get_sum(v->r, tm+1, tr, max(l, tm+1), r);
		}

		Node* update(Node* v, int tl, int tr, int pos, int new_val) {
		    if (tl == tr)
		        return new Node(new_val);
		    int tm = (tl + tr) / 2;
		    if (pos <= tm)
		        return new Node(update(v->l, tl, tm, pos, new_val), v->r);
		    else
		        return new Node(v->l, update(v->r, tm+1, tr, pos, new_val));
		}
	
} PST;
