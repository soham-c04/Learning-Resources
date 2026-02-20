#include <bits/stdc++.h>
using namespace std;
#define F first
#define S second

set<char> func(set<char> &R_i,vector<pair<set<char>,set<char>>> &fds) {
    set<char> result = R_i;
    bool change = true;
	while(change){
		change=false;
        for(auto &fd : fds) {
            set<char> &l = fd.F;
            set<char> &r = fd.S;
            if(includes(result.begin(), result.end(), l.begin(), l.end())) {
                int before = result.size();
                result.insert(r.begin(), r.end());
                if(result.size() > before) change = true;
            }
        }
    }

    return result;
}

// Dependency preservation
bool isDependencyPreserved( vector<pair<set<char>,set<char>>> &fds, vector<set<char>> &D) {
    for (auto &fd : fds) {
        set<char> lClosure;
        for(auto &R_i : D) {
            if (includes(R_i.begin(), R_i.end(), fd.F.begin(), fd.F.end())) {
                vector<pair<set<char>,set<char>>> v;

                for ( auto &f : fds) {
                    if (includes(R_i.begin(), R_i.end(), f.F.begin(), f.F.end()) &&
                        includes(R_i.begin(), R_i.end(), f.S.begin(), f.S.end())) {
                        v.push_back(f);
                    }
                }

                lClosure = func(fd.F, v);
                if (includes(lClosure.begin(), lClosure.end(), fd.S.begin(), fd.S.end())) break;
            }
        }

        if (!includes(lClosure.begin(), lClosure.end(), fd.S.begin(), fd.S.end())) return false;
    }

    return true;
}

// lossless join check
bool isLosslessJoin( vector<set<char>> &R_is,  vector<pair<set<char>,set<char>>> &fds) {
	set<char> intersection = R_is[0];
	for ( auto &R_i : R_is)
	    set_intersection(intersection.begin(), intersection.end(),
	                     R_i.begin(), R_i.end(),
	                     inserter(intersection, intersection.begin()));

	for(auto &fd : fds) {
	    if (includes(fd.F.begin(), fd.F.end(), intersection.begin(), intersection.end()))
	        return true;
	}

	return false;
}

int main() {
	set<char> R = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

	vector<pair<set<char>, set<char>>> fds = {
	    {{'A'}, {'B'}},
	    {{'A'}, {'C'}},
	    {{'B', 'C'}, {'D'}},
	    {{'D'}, {'E'}},
	    {{'E'}, {'F'}},
	    {{'F'}, {'G'}}
	};

	vector<set<char>> D;
	for(auto &fd : fds) {
	    set<char> R_i = fd.F;
	    R_i.insert(fd.S.begin(), fd.S.end());
	    D.push_back(R_i);
	}

	set<char> allAttrs;
	for(auto &R_i : D) allAttrs.insert(R_i.begin(), R_i.end());

	if (allAttrs != R) D.push_back(R);

	// Output decomposed R_is
	cout << "Decomposed Schemas (3NF):\n";
	for(auto &R_i : D) {
		cout << "{ ";
	    for (char c : R_i) cout << c << " ";
	    cout << "}\n";
	}
	cout<<"\n";

	cout << "\nLossless Join: " << (isLosslessJoin(D, fds) ? "Yes" : "No") << "\n";

	cout << "Dependency Preserving: " << (isDependencyPreserved(fds, D) ? "Yes" : "No") << "\n";

	return 0;
}
