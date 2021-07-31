#include<bits/stdc++.h>
using namespace std;
using ll=long long;
#define rep(i,a,b) for(ll i=(a);i<=(b);i++)

int main()
{
    freopen("(you_path)/testdata.txt","w",stdout);
    ll n=(testdata_size),temp;
    srand(time(NULL));
    rep(i,1,n){
        temp=rand()%4;
        if(temp==0)cout<<'A';
        if(temp==1)cout<<'C';
        if(temp==2)cout<<'T';
        if(temp==3)cout<<'G';
    }
}
