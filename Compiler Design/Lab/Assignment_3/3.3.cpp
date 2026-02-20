#include <bits/stdc++.h>
using namespace std;

int main(){
    cout<<"Input should only contains LOWERcase and/or symbols i.e. terminals.\n\n";
    cout<<"Enter input: ";
    string input;
    cin>>input;
    input.push_back('$');
    
    string line;
    set<char> terminals;
    bool follow = false;
    map<char,string> FIRST,FOLLOW;
    ifstream fin("First-Follow.txt");
    while(true){
        getline(fin,line);
        if(line.empty()) break;
        int n = line.size();
        char left = line[0];
        string right = "";
        if(line == "FOLLOW") follow = true;
        else if(!follow){
            for(int i=5;i<n;i++){
                if(line[i] != ' '){
                    right.push_back(line[i]);
                    if(line[i] != '0') terminals.insert(line[i]);
                }
            }
            FIRST[left] = right;
        }
        else{
            for(int i=5;i<n;i++){
                if(line[i] != ' '){
                    right.push_back(line[i]);
                    if(line[i] != '0') terminals.insert(line[i]);
                }
            }
            FOLLOW[left] = right;
        }
    }
    fin.close();

    map<char,map<char,string>> ParseTable;
    char start=0;	// This is the start symbol for the grammar
    
    ifstream fin1("GrammarLL.txt");
    while(true){
        getline(fin1,line);
        if(line.empty()) break;
        line.push_back('|');
        int n = line.size();
        char left = line[0];
        if(!start) start = left;
        string production = "";
        char first = 0;
        for(int i=5;i<n;i++){
            if(line[i] == '|'){
                if(production.empty()) continue;
                if(first == '0'){
                    for(char b:FOLLOW[left]){
                    	if(ParseTable[left].find(b) != ParseTable[left].end()){
                    		cout<<"Given grammar is not LL(1).\n";
                    		return 1;
						}
						ParseTable[left][b] = production;
					}
                }
                else if(first >= 'A' && first <= 'Z'){
                    for(char f:FIRST[first]){
                        if(f != '0'){
	                        if(ParseTable[left].find(f) != ParseTable[left].end()){
	                    		cout<<"Given grammar is not LL(1).\n";
	                    		return 1;
							}
							ParseTable[left][f] = production;
						}
                        else{
                            for(char b:FOLLOW[left]){
                            	if(ParseTable[left].find(b) != ParseTable[left].end()){
		                    		cout<<"Given grammar is not LL(1).\n";
		                    		return 1;
								}
								ParseTable[left][b] = production;
							}
                        }
                    }
                }
                else{
                	if(ParseTable[left].find(first) != ParseTable[left].end()){
                		cout<<"Given grammar is not LL(1).\n";
                		return -1;
					}
                    ParseTable[left][first] = production;
                }
                first = 0;
                production.clear();
            }
            else if(line[i] != ' '){
                production.push_back(line[i]);
                if(!first) first = line[i];
            }
        }
    }
    fin1.close();

    cout<<"\nPARSE TABLE:\n\n";
    cout<<setw(5)<<left<<" ";
    for(char c:terminals) cout<<setw(5)<<left<<c;
    cout<<"\n";
    for(int i=0;i<=terminals.size()*5;i++) cout<<"-";
    cout<<"--\n";
    
    for(auto [S,mp]: ParseTable){
        cout<<setw(5)<<left<<S;
        for(char c:terminals){
        	string s = mp[c];
        	if(s.empty()) s = "-";
			cout<<setw(5)<<left<<s;
		}
        cout<<"\n";
    }
    cout<<"\n";

	int ip = 0; // Pointer to input
	int n = input.size();
	vector<int> stk;
	stk.push_back('$');
	stk.push_back(start);
	vector<pair<int,pair<string,string>>> sequence;
	while(!stk.empty() && ip<n){
		int t = stk.back();
		int a = input[ip];
		if(t>='A' && t<='Z'){   // t is a non-terminal
			string production = ParseTable[t][a];
			if(production.empty()){
				cout<<"Error: "<<(char)t<<" and "<<(char)a<<" are not compatible.\n";
				return 1;
			}
			stk.pop_back();
			string prod = "";
			prod.push_back(t);
			prod += " -> ";
			prod += production;
			sequence.push_back({ip,{prod,""}});
			if(production != "0"){    // Not epsilon
				for(int i=production.size()-1;i>=0;i--){
					stk.push_back(production[i]);
				}
			}
			for(int i=stk.size()-1;i>=0;i--) sequence.back().second.second.push_back(stk[i]);
		}
		else{   // t is a terminal
			if(t==a){
				ip++;
				stk.pop_back();
			}
			else{
				cout<<"Error: Terminal mismatch\n";
				return 2;
			}
		}
	}
	
	if(stk.empty() && ip==n){
		input.pop_back();
		
		cout<<"Accepted\n";
		cout<<"Sequence of Productions:\n\n";
		cout<<start<<"\n";
		cout<<input<<"\n\n";
		int cnt = 1;
		for(auto [ip,pr]:sequence){
			cout<<cnt++<<". "<<pr.first<<"\n   ";
			for(int i=0;i<ip;i++) cout<<input[i];
			pr.second.pop_back();
			cout<<pr.second<<"\n\n";
		}
		cout<<"\n";
	}
	else{
		cout<<"Error\n";
		return 3;
	}
	
    return 0;
}
