#include <bits/stdc++.h>
using namespace std;

int main(){
	string line;
	
	map<char,vector<string>> productions;
	char start = 0;
    ifstream fin("GrammarLL.txt");
    while(true){
        getline(fin,line);
        if(line.empty()) break;
        line.push_back('|');
        int n = line.size();
        char left = line[0];
		if(!start) start = left;
        string production = "";
        for(int i=5;i<n;i++){
            if(line[i] == '|'){
                if(production.empty()) continue;
                productions[left].push_back(production);
                production.clear();
            }
            else if(line[i] != ' ') production.push_back(line[i]);
        }
    }
    fin.close();

    map<char,set<char>> FIRST;
    bool change = true;
    while(change){
    	change = false;
    	for(auto [left,vec]:productions){
    		for(string &prod:vec){
	    		for(int i=0;i<prod.size();i++){
		    		if(prod[i]>='A' && prod[i]<='Z'){
		    			bool next = false;
		    			for(char c:FIRST[prod[i]]){
		    				if(c == '0') next = true;
		    				else if(FIRST[left].find(c) == FIRST[left].end()){
								FIRST[left].insert(c);
								change = true;
							}
						}
						if(!next) break;
						else if(i==prod.size()-1){
							if(FIRST[left].find('0') == FIRST[left].end()){
								FIRST[left].insert(prod[i]);
								change = true;
							}
						}
					}
					else{
						if(FIRST[left].find(prod[i]) == FIRST[left].end()){
							FIRST[left].insert(prod[i]);
							change = true;
						}
						break;
					}
				}
			}
		}
    }
    
    map<char,set<char>> FOLLOW;
    FOLLOW[start].insert('$');
    change = true;
    while(change){
    	change = false;
    	for(auto [left,vec]:productions){
    		for(string &prod:vec){
	    		for(int i=0;i<prod.size()-1;i++){
		    		if(prod[i]>='A' && prod[i]<='Z'){
		    			if(prod[i+1]>='A' && prod[i+1]<='Z'){
			    			for(char c:FIRST[prod[i+1]]){
			    				if(c != '0'){
			    					if(FOLLOW[prod[i]].find(c) == FOLLOW[prod[i]].end()){
			    						FOLLOW[prod[i]].insert(c);
			    						change = true;
									}
								}
								else{
									for(char c:FOLLOW[prod[i+1]]){
										if(FOLLOW[prod[i]].find(c) == FOLLOW[prod[i]].end()){
				    						FOLLOW[prod[i]].insert(c);
				    						change = true;
										}
									}
								}
							}
						}
						else{
							if(FOLLOW[prod[i]].find(prod[i+1]) == FOLLOW[prod[i]].end()){
								change = true;
								FOLLOW[prod[i]].insert(prod[i+1]);
							}
						}
					}
				}
				if(prod.back()>='A' && prod.back()<='Z'){
					for(char c:FOLLOW[left]){
						if(FOLLOW[prod.back()].find(c) == FOLLOW[prod.back()].end()){
    						FOLLOW[prod.back()].insert(c);
    						change = true;
						}
					}
				}
			}
		}
    }
    
    ofstream fout("First-Follow.txt");
    fout<<"FIRST\n";
    for(auto [left,f]:FIRST){
    	fout<<left<<" -> ";
		for(char c:f) fout<<c<<" ";
		fout<<"\n";
	}
	fout<<"FOLLOW\n";
	for(auto [left,f]:FOLLOW){
    	fout<<left<<" -> ";
		for(char c:f) fout<<c<<" ";
		fout<<"\n";
	}
    fout.close();
    
    return 0;
}
