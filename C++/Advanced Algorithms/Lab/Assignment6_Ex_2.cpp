#include <bits/stdc++.h>
using namespace std;

string LPSq(string &s) {
    int n = s.size();
    int dp[n][n];
    memset(dp,0,sizeof(dp));

    for(int i=0;i<n;i++) dp[i][i] = 1;
    for(int len=2;len<=n;len++){
        for(int i=0;i+len-1<n;i++){
            int j=i+len-1;
            if(s[i]==s[j]) dp[i][j] = 2+((len==2)? 0:dp[i + 1][j - 1]);
            else dp[i][j] = max(dp[i+1][j],dp[i][j-1]);
        }
    }

    int i=0,j=n-1;
    string left= "",right="";
    while(i<=j) {
        if(s[i]==s[j]){
            if(i == j) left += s[i];
            else{
                left += s[i];
                right = s[j]+right;
            }
            i++;
			j--;
        }
		else if(dp[i+1][j]>=dp[i][j-1]) i++;
        else j--;
    }

    return left+right;
}

int main() {
    string s;
    cout<<"Enter string: ";
    cin>>s;

    string lps = LPSq(s);
    cout<<"Length of longest Palindromic Subsquence: "<<lps.size()<<"\n";
    cout<<"Longest Palindromic Subsequence: "<<lps<<"\n";

    return 0;
}
