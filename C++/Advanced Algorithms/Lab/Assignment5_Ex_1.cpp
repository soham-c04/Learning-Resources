#include<bits/stdc++.h>
using namespace std;

// Only Enter values x from 0 to n-1 in the makeSET(x) operations.
// README.pdf for more instructions.

struct Node{
	int parent;
	int size;
	
	// Now below add required parameters attached to each node
	int value; // Note:- value is not being used in the code but if needed can be used.
};

vector<Node> info;

void makeSET(int v){
	info[v].parent = v;
	info[v].size = 1;
}

int findSET(int v){
    if(v == info[v].parent) return v;
    return info[v].parent = findSET(info[v].parent);
}

void unionSET(int a, int b) {
    a = findSET(a);
    b = findSET(b);
    if(a != b){
        if(info[a].size < info[b].size) swap(a, b);
        info[b].parent = a;
        info[a].size += info[b].size;
    }
}

int main(){
	int n;
	cout<<"Enter the number of makeSET(x) operations: ";
	cin>>n;
	info.resize(n);
	
	cout<<"Enter queries:-\n\n";
	while(true){
		int type;
		cin>>type;
		if(type==1){
			int x;
			cin>>x;
			makeSET(x);
		}
		else if(type==2){
			int x,y;
			cin>>x>>y;
			unionSET(x,y);
		}
		else if(type==3){
			int x;
			cin>>x;
			if(x<0 || x>=n){
				cout<<"Out of bounds: Input to findSET() should be in the range [0,n-1].\n";
				continue;
			}
			cout<<findSET(x)<<"\n";
		}
		else break;
		cout<<"\n";
	}
	
	return 0;
}
