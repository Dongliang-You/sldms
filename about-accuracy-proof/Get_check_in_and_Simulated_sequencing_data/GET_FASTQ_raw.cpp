#include<bits/stdc++.h>
using namespace std;
using ll=long long;
#define rep(i,a,b) for(ll i=(a);i<=(b);i++)

int main()
{
    freopen("(you_path)/testdata.txt","r",stdin);
    freopen("(you_path)/testdata_raw_1x.fastq","w",stdout);
    ofstream fout("(you_path)/getans_raw_1x_fastq_in_1.txt");
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
            string out=string(s.begin()+now,s.begin()+now+len);
            fout<<now<<" "<<now+len-1<<'\n';
            rep(i,0,(Data header error range)){
                if(rand()%3==0){
                    int temp=rand()%4;
                    if(temp==0)out[i]='A';
                    if(temp==1)out[i]='C';
                    if(temp==2)out[i]='T';
                    if(temp==3)out[i]='G';
                }
            }
            rep(i,len-(Data tail error range),len-1){
                if(rand()%3==0){
                    int temp=rand()%4;
                    if(temp==0)out[i]='A';
                    if(temp==1)out[i]='C';
                    if(temp==2)out[i]='T';
                    if(temp==3)out[i]='G';
                }
            }
            cout<<out<<'\n';
            cout<<"+SRRXXXX."<<num<<".1 length="<<len<<'\n';
            rep(i,1,len)cout<<"9";cout<<'\n';
            if(now+len==n)break;
            now+=len;
            ll premove=(The length of the overlap you need);
            if(premove>len)premove=len;
            now-=premove;
            num++;
        }
    }
}
