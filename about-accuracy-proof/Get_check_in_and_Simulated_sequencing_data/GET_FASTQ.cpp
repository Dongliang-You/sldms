#include<bits/stdc++.h>
using namespace std;
using ll=long long;
#define rep(i,a,b) for(ll i=(a);i<=(b);i++)

int main()
{
    freopen("(you_path)/testdata.txt","r",stdin);
    freopen("(you_path)/testdata_corr_1x.fastq","w",stdout);
    ofstream fout("(you_path)/getans_corr_1x_fastq_in_1.txt");
    int depth=(How much genome coverage you want);
    ll now=0,len=0;
    string s;cin>>s;
    srand(time(NULL));
    ll n=s.size();
    ll num=1;
    rep(z,1,depth){
        now=0;
        while(1){
            len=(The length of the data you need);
            if(now+len>=n)len=n-now;
            cout<<"@SRRXXXX."<<num<<".1 length="<<len<<'\n';
            cout<<string(s.begin()+now,s.begin()+now+len)<<'\n';
            fout<<now<<" "<<now+len-1<<'\n';
            cout<<"+SRRXXXX."<<num<<".1 length="<<len<<'\n';
            rep(i,1,len)cout<<"9";cout<<'\n';
	    num++;
            if(now+len==n)break;
            now+=len;
            ll premove=(The length of the overlap you need);
            if(premove>len)premove=len;
            now-=premove;
        }
    }
}
