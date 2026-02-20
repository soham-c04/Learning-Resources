#include<bits/stdc++.h>
using namespace std;

int main(){
	freopen("Machine_language.txt","r",stdin);
	freopen("Binary.txt","w",stdout);
	while(true){
		string line;
		getline(cin,line);
		if(line=="Hexadecimal:-") break;
		else if(line=="Binary:-") continue;
		else{
			int n=line.size();
			if(line[4]=='E') continue;
			for(int i=4;i<n;i++) if(line[i]!=' ') cout<<line[i];
			cout<<"\n";
		}
	}
}
