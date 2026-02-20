#include<bits/stdc++.h>
#include<ext/random>
using namespace std;
#define f(n) for(int i=0;i<(n);i++)
typedef long long ll;
typedef long double ld;
using RNG = __gnu_cxx::sfmt19937;
decltype(RNG()()) x;
ll rand_num(ll a,ll b){
    random_device rd;
    mt19937_64 gen(rd());
    normal_distribution<ld> dist(a,b);
    ll result = dist(gen);
    while(result<0) result = dist(gen);
    return result;
}

const int N=64,M=8,B=16,mean=1e5,dev=4300; // Gives a hit rate of around 48.9%
int state[N][M];
ll tag[N][M],Data[N][M][B]; // Note that here it will be optimal to declare the arrays as 8*16*64 due to cache-friendliness, however
// It is declared this way for ease of understanding
int main(int argc, char *argv[]){ // To run this random_generator you have to use Ubuntu along with any argument while compilation
//  g++ Cache.cpp -o cache
// ./cache 17
    f(N) for(int j=0;j<M;j++) state[i][j]=0; // Invalid initially
    int REQUEST=1e7;
    int all=REQUEST,hit=0;
    while(REQUEST--){
        bool print=((REQUEST%(all/100))==0);
        bool read=rand()%2;
        ll address=rand_num(mean,dev)<<2;
        ll BA=address/(4*B);
        int index=BA%N;
        ll Tag=BA/N;
        int BO=address%(4*B);
        int found=-1;
        f(M) if(state[index][i]==1 && tag[index][i]==Tag) found=i;
        if(print) cout<<"\nRemaining: "<<REQUEST<<"\n";
        if(found!=-1){
            hit++;
            if(print) cout<<"Hit!!\n";
        }
        else if(print) cout<<"Miss :(\n";
        if(print) cout<<"Address: "<<address<<"\n";
        if(found!=-1){
            if(read){
                if(print) cout<<"Found Data: "<<Data[index][found][BO]<<"\n";
            }
            else{
                if(print) cout<<"Written Data: "<<(Data[index][found][BO]=rand_num(100,10))<<"\n";
            }
        }
        else{
            int free=-1;
            f(M) if(!state[index][i]) free=i;
            while(free<0) free=rand_num(M/2,M/2)%M;
            state[index][free]=1;
            tag[index][free]=Tag;
            f(B) Data[index][free][i]=rand_num(100,10); // Whatever Data was present in main memory or newly written data.  
            if(print){
                cout<<"Written/Replaced Block Data:\n";
                f(B) cout<<(Data[index][found][i]=rand_num(100,10))<<", ";
                cout<<"\n";
            }
        }
    }
    cout<<fixed<<setprecision(6)<<"Hit rate: "<<(ld)hit*100/all<<"\n";
    return 0;
}
