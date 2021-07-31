#include<bits/stdc++.h>
using namespace std;
using ll=long long;
using ull=unsigned long long;
#define rep(i,a,b) for(ll i=(a);i<=(b);i++)
#define rrep(i,a,b) for(ll i=(b);i>=(a);i--)
#define IOS ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#define PII pair<int,int>
#define PLL pair<ll,ll>
#define xx first
#define yy second
#define pb push_back

int main()
{
    freopen("(you_path)/testdata.txt","r",stdin);
    freopen("(you_path)/testdata_corr_40x.fasta","w",stdout);
    ofstream fout("(you_path)/getans_corr_40x_fasta_in_1.txt");
    int depth=(How much genome coverage you want);
    ll now=0;
    ll len=0;
    string s;
    cin>>s;
    srand(time(NULL));
    ll n=s.size();
    ll num=1;
    rep(z,1,depth){
        now=0;
        while(1){
            len=(The length of the data you need);
            if(now+len>=n)len=n-now;
            cout<<">SRRXXXX."<<num<<".1 length="<<len<<'\n';
            num++;
            cout<<string(s.begin()+now,s.begin()+now+len)<<'\n';
            fout<<now<<" "<<now+len-1<<'\n';
            if(now+len==n)break;
            now+=len;
            ll premove=(The length of the overlap you need);
            if(premove>len)premove=len;
            now-=premove;
        }
    }
}
