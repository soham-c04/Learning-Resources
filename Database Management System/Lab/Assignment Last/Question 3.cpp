#include <bits/stdc++.h>
using namespace std;
#define F first
#define S second

set<char> func(set<char> &R_i, vector<pair<set<char>,set<char>>> &fds) {
    set<char> result = R_i;
    bool change = true;
    while (change) {
        change = false;
        for (auto &fd : fds) {
            if (includes(result.begin(), result.end(), fd.F.begin(), fd.F.end())) {
                int before = result.size();
                result.insert(fd.S.begin(), fd.S.end());
                if (result.size() > before) change = true;
            }
        }
    }
    return result;
}

// BCNF check
bool check(set<char> &R_i, pair<set<char>,set<char>> &fd, vector<pair<set<char>,set<char>>> &fds) {
    set<char> fdClosure = func(fd.F, fds);
    return includes(R_i.begin(), R_i.end(), fd.F.begin(), fd.F.end()) &&
           (!includes(fdClosure.begin(), fdClosure.end(), R_i.begin(), R_i.end()));
}

vector<set<char>> bcnfDecompose(set<char> R_i, vector<pair<set<char>,set<char>>> &fds) {
    for (auto &fd : fds) {
        if (includes(R_i.begin(), R_i.end(), fd.F.begin(), fd.F.end())) {
            set<char> func_fd = func(fd.F, fds);

            if (!includes(func_fd.begin(), func_fd.end(), R_i.begin(), R_i.end())) {
                set<char> R1 = fd.F;
                R1.insert(fd.S.begin(), fd.S.end());

                set<char> R2;
                set<char> temp = R_i;
                for (char c : fd.S) temp.erase(c);
                R2 = temp;
                R2.insert(fd.F.begin(), fd.F.end());

                if (R1 == R_i || R2 == R_i) continue;

                vector<set<char>> result;
                auto left = bcnfDecompose(R1, fds);
                auto right = bcnfDecompose(R2, fds);
                result.insert(result.end(), left.begin(), left.end());
                result.insert(result.end(), right.begin(), right.end());
                return result;
            }
        }
    }
    return {R_i};
}

int main() {
    set<char> R = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

	vector<pair<set<char>, set<char>>> fds = {
	    {{'A'}, {'B'}},
	    {{'A'}, {'C'}},
	    {{'B'}, {'D'}},
	    {{'C'}, {'E'}},
	    {{'D', 'E'}, {'F'}},
	    {{'F'}, {'G'}},
	    {{'G'}, {'H'}}
	};

    vector<set<char>> D = bcnfDecompose(R, fds);

    cout << "BCNF Decomposition:\n";
    for (auto &R_i : D) {
        cout << "{ ";
        for (char c : R_i) cout << c << " ";
        cout << "}\n";
    }

    return 0;
}
