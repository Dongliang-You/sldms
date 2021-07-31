#include<bits/stdc++.h>
using namespace std;
using ll=long long;
#define rep(i,a,b) for(ll i=(a);i<=(b);i++)
#define rrep(i,a,b) for(ll i=(b);i>=(a);i--)
bool check(ll l1,ll r1,ll l2,ll r2){
    if(
            (l1<=r2&&r1>=l2)||
            (l1<=r2&&l1>=l2)||
            (r1<=r2&&r1>=r2)
            )return 1;
    else return 0;
}
int main()
{
    freopen("(you_path)/(you_check_in_file)","r",stdin);
    ifstream fin("(you_path)/(you_runout_file)");
    vector<ll>l,r;
    int nowl,nowr;
    while(cin>>nowl>>nowr){
        l.push_back(nowl);
        r.push_back(nowr);
    }
    int begin,to,len,allnum=0,Ynum=0,temp;
    while(fin>>begin>>to>>len){
        rep(i,1,10)fin>>temp;
        if(len<100)continue;
        allnum++;
        if(check(l[begin],r[begin],l[to],r[to])){
            Ynum++;
        }
    }
    cout<<1.0*Ynum/allnum<<endl;
	
}
