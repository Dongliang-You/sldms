#include "utils.h"
#include <stdio.h>
#include <string.h>
//int max(int a,int b){return a>b?a:b;}
typedef struct queue
{
    int head,tail,have;
    int size;
    int *data;
}queue;
typedef struct pair
{
    int first;
    struct queue second;
}pair;
typedef struct vector
{
    int now;
    int size;
    struct pair *data;
}vector;

typedef struct pair_raw
{
    int first;
    int second;
    int third;
}pair_raw;
typedef struct vector_raw
{
    int now;
    int size;
    struct pair_raw *data;
}vector_raw;
typedef struct heap_raw
{
    int have;
    int size;
    int *id;
    int *len;
    int *pre;
    int *suf;
}heap_raw;
/**********************************************************************/

void time_start(time_t *t_time, clock_t *c_clock){

    *t_time = time(NULL);
    *c_clock =  clock();
}

double time_stop(time_t t_time, clock_t c_clock){

    double aux1 = (clock() - c_clock) / (double)(CLOCKS_PER_SEC);
    double aux2 = difftime (time(NULL),t_time);

    printf("CLOCK = %lf TIME = %lf\n", aux1, aux2);

    return aux1;
}


/**********************************************************************/
void die(const char* where) {

    printf("Error at: %s: %s.\n",where,errno ? strerror(errno) : "errno not set");
    exit(errno?errno:1);
}

void dies(const char* where, char* format, ...) {

    int_t err = errno;
    va_list val;

    printf("Error at %s: %s.\n",where,errno ? strerror(errno) : "errno not set");
    va_start(val,format);
    vprintf(format,val);
    va_end(val);
    printf("\n");

    exit(err ? err : 1);
}
/**********************************************************************/
int_t print_char(char* A, int_t n){

    int_t i;
    for(i=0; i<n; i++)
        printf("%" PRIdN ") %c\n", i, A[i]);

    printf("\n");

    return 0;
}
/**********************************************************************/
int_t print_int(int_t* A, size_t n){

    size_t i;
    for(i=0; i<n; i++)
        printf("%zu) %" PRIdN "\n", i, A[i]);

    printf("\n");

    return 0;
}

/******************************************************************************/

unsigned char* cat(unsigned char* s1, unsigned char* s2, size_t *n){

    (*n) = (strlen((char*)s1)+1)+(strlen((char*)s2)+1)+1; //add 0 at the end

    size_t j, l=0;
    unsigned char *str = (unsigned char*) malloc((*n)*sizeof(unsigned char));

    /**/
    {
        int_t m = strlen((char*)s1);
        for(j=0; j<m; j++) if(s1[j]<255) str[l++] = s1[j]+1;
        str[l++] = 1; //add 1 as separator
    }

    {
        int_t m = strlen((char*)s2);
        for(j=0; j<m; j++) if(s2[j]<255) str[l++] = s2[j]+1;
        str[l++] = 1; //add 1 as separator
    }
    /**/

    str[l++]=0;
    if(*n>l){
        str = (unsigned char*) realloc(str, (l)*sizeof(unsigned char));
    }
    *n = l;

    return str;
}

/******************************************************************************/


unsigned char* cat_all(unsigned char** R, int k, size_t *n, int verbose, int upper){

    (*n)++; //add 0 at the end
    int_t i, j;
    size_t l=0;
    unsigned char *str = (unsigned char*) malloc((*n)*sizeof(unsigned char));

    int_t max=0;

    if(upper){

        int diff=0;
        char lb, rb;
        if(upper==1){//lowercase
            diff = 32;
            lb='A'; rb='Z';
        }
        if(upper==2){//uppercase
            diff = -32;
            lb='a'; rb='z';
        }

        for(i=0; i<k; i++){
            int_t m = strlen((char*)R[i]);
            if(m>max) max=m;
            for(j=0; j<m; j++){
                if(R[i][j]<255){
                    str[l++] = R[i][j]+1;
                    if(R[i][j]>=lb && R[i][j]<=rb) str[l-1]+=diff;
                }
            }
            if(m) str[l++] = 1; //add 1 as separator (ignores empty entries)
        }
    }
    else{
        for(i=0; i<k; i++){
            int_t m = strlen((char*)R[i]);
            if(m>max) max=m;
            for(j=0; j<m; j++){
                if(R[i][j]<255) str[l++] = R[i][j]+1;
            }
            if(m) str[l++] = 1; //add 1 as separator (ignores empty entries)
        }
    }

    str[l++]=0;
    if(*n>l){
        str = (unsigned char*) realloc(str, (l)*sizeof(unsigned char));
    }
    *n = l;

    if(verbose) printf("longest string = %" PRIdN "\n", max);

    return str;
}


/**********************************************************************/

int print_array(unsigned char *T, int_da *DA, int_t *SA, int_t *ISA, int_t *LCP,
                size_t n, size_t m, int last_end,int dnum,FILE *out,int temptopk){

    /*size_t i,j;
    fprintf(fp,"%d\t",dnum);
    fprintf(fp,"%zu\t",m);
    fprintf(fp,"\n");*/
    // output
    //printf("i\t");
    printf("-corr print OK!  topk=%d\n",temptopk);
    int nlen=0,d=0,tsa=0,tda=0,tlcp=0,tx=temptopk,i;
    d=dnum;nlen=m;
    int *fi=(int *)malloc((d+7)*sizeof(int));
    int *se=(int *)malloc((d+7)*sizeof(int));
    fi[0]=0;
    se[0]=SA[1];
    int mx=se[0]+1;
    for(i=2; i<=d; i++)
    {
        /*fscanf(in,"%d",&tsa);
        fscanf(in,"%d",&tda);
        fscanf(in,"%d",&tlcp);*/
        tsa=SA[i];
        tda=DA[i];
        fi[tda]=se[tda-1]+1;
        se[tda]=tsa;
        mx=max(mx,se[tda]-fi[tda]+1);
    }
    vector v;
    v.size=mx;
    v.now=0;
    v.data=(pair*)malloc((v.size+7)*sizeof(pair));
    for(i=d+1; i<nlen; i++)
    {
        tsa=SA[i];
        tda=DA[i];
        tlcp=LCP[i];
        while(v.now>0)
        {
            if(v.data[v.now].first>tlcp)
            {
                free(v.data[v.now].second.data);
                v.now--;
            }
            else
                break;
        }
        int num=se[tda]-tsa;//num 后缀的长度
        if(tsa==fi[tda])//如果这是一个完整的字符串。
        {
            int base=tx;
            fprintf(out,"%d",tda);
            int rit;
            for(rit=v.now; rit>0; rit--)
            {
                int temp;
                for(temp=v.data[rit].second.tail;;)
                {
                    fprintf(out," %d",v.data[rit].second.data[temp]);
                    fprintf(out," %d",v.data[rit].first);
                    base--;
                    if(base<=0)
                        break;
                    if(temp==v.data[rit].second.head)
                        break;
                    if(--temp<0)
                        temp+=tx;
                }
                if(base<=0)
                    break;
            }
            fprintf(out,"\n");
        }
        if(v.now>0&&v.data[v.now].first==num)
        {
            if(v.data[v.now].second.have==tx)
            {
                v.data[v.now].second.head++;
                if(v.data[v.now].second.head>=tx)
                    v.data[v.now].second.head=0;
                v.data[v.now].second.tail++;
                if(v.data[v.now].second.tail>=tx)
                    v.data[v.now].second.tail=0;
                v.data[v.now].second.data[v.data[v.now].second.tail]=tda;
            }
            else
            {
                v.data[v.now].second.tail++;
                if(v.data[v.now].second.tail>=tx)
                    v.data[v.now].second.tail=0;
                v.data[v.now].second.data[v.data[v.now].second.tail]=tda;
                v.data[v.now].second.have++;
            }
        }
        else
        {
            v.now++;
            v.data[v.now].first=num;
            v.data[v.now].second.have=1;
            v.data[v.now].second.size=tx;
            v.data[v.now].second.head=0;
            v.data[v.now].second.tail=0;
            v.data[v.now].second.data=(int*)malloc((tx+7)*sizeof(int));
            v.data[v.now].second.data[0]=tda;
        }
    }

    return 0;
}

int print_array_raw(unsigned char *T, int_da *DA, int_t *SA, int_t *ISA, int_t *LCP,
                    size_t n, size_t m,
                    int last_end,int dnum,FILE *out,int temptopk,int tempFault_tolerance){

    /*size_t i,j;
    fprintf(fp,"%d\t",dnum);
    fprintf(fp,"%zu\t",m);
    fprintf(fp,"\n");*/
    // output
    //printf("i\t");
    printf("-raw print OK!  topk=%d Fault_tolerance=%d\n",temptopk,tempFault_tolerance);
    int Fault_tolerance=tempFault_tolerance;//容错程度
    int nlen=0,d=0,tsa=0,tda=0,tlcp=0,i,topk=temptopk;
    d=dnum;nlen=m;
    int *fi=(int *)malloc((d+7)*sizeof(int));
    int *se=(int *)malloc((d+7)*sizeof(int));
    int *ans=(int *)malloc((d+7)*sizeof(int));
    int *len=(int *)malloc((d+7)*sizeof(int));
    int *pre=(int *)malloc((d+7)*sizeof(int));
    int *suf=(int *)malloc((d+7)*sizeof(int));
    heap_raw *headans=(heap_raw *)malloc((d+7)*sizeof(heap_raw));
    for(int i=0;i<d+7;i++){
        headans[i].size=topk;
        headans[i].have=0;
        headans[i].id=(int *)malloc((topk*2+7)*sizeof(int));
        headans[i].len=(int *)malloc((topk*2+7)*sizeof(int));
        headans[i].pre=(int *)malloc((topk*2+7)*sizeof(int));
        headans[i].suf=(int *)malloc((topk*2+7)*sizeof(int));
        for(int j=0;j<=topk*2+5;j++){
            headans[i].id[j]=-1;
            headans[i].len[j]=1e9+7;
        }
    }
    memset(ans,-1,(d+6)*sizeof(int));
    //memset(len,0,sizeof(len));
    fi[0]=0;
    se[0]=SA[1];
    int mx=se[0]+1;
    for(i=2; i<=d; i++)
    {
        /*fscanf(in,"%d",&tsa);
        fscanf(in,"%d",&tda);
        fscanf(in,"%d",&tlcp);*/
        tsa=SA[i];
        tda=DA[i];
        fi[tda]=se[tda-1]+1;
        se[tda]=tsa;
        mx=max(mx,se[tda]-fi[tda]+1);
    }

    vector_raw v;
    v.size=mx;
    v.now=0;
    v.data=(pair_raw*)malloc((v.size+7)*sizeof(pair_raw));
    int tempnode_i,tempnode_ErrNum,nownode_ErrNum;
    for(i=d+1; i<nlen; i++)
    {
        tsa=SA[i];
        tda=DA[i];
        tlcp=LCP[i];
        tempnode_i=-1;
        while(v.now>0)
        {
            if(v.data[v.now].first>=tlcp)
            {
                if(DA[v.data[v.now].second]==tda)
                {
                    v.now--;
                    continue;
                }
                nownode_ErrNum=v.data[v.now].third+v.data[v.now].first-tlcp;
                if(nownode_ErrNum>Fault_tolerance)
                {
                    v.now--;
                    continue;
                }

                if(tempnode_i==-1)
                {
                    tempnode_i=v.data[v.now].second;
                    tempnode_ErrNum=nownode_ErrNum;
                }
                else
                {
                    //nownode_ErrNum=v.data[v.now].third+v.data[v.now].first-tlcp;
                    if(nownode_ErrNum<=tempnode_ErrNum){
                        tempnode_ErrNum=nownode_ErrNum;
                        tempnode_i=v.data[v.now].second;
                    }
                }
                v.now--;
            }
            else
                break;
        }
        if(tempnode_i!=-1){
            v.now++;
            v.data[v.now].first=tlcp;
            v.data[v.now].second=tempnode_i;
            v.data[v.now].third=tempnode_ErrNum;
        }
        int num=se[tda]-tsa;//num 后缀的长度
        if(tsa-fi[tda]<=Fault_tolerance&&v.now>0)//如果这是在容错范围内的字符串。
        {
            //fprintf(out,"%d",tda);
            int nowpre=tsa-fi[tda];
            int worknow=v.now;
            while(worknow>0&&DA[v.data[worknow].second]==tda)worknow--;
            if(worknow>0){
                if(ans[tda]==-1)
                {
                    len[tda]=v.data[worknow].first;
                    ans[tda]=v.data[worknow].second;
                    suf[tda]=v.data[worknow].third;
                    pre[tda]=nowpre;
                    headans[tda].have=1;
                    headans[tda].id[1]=ans[tda];
                    headans[tda].len[1]=len[tda];
                    headans[tda].pre[1]=pre[tda];
                    headans[tda].suf[1]=suf[tda];
                }
                else
                {
                    if(v.data[worknow].first>=len[tda]&&(DA[v.data[worknow].second]!=tda))
                    {
                        len[tda]=v.data[worknow].first;
                        ans[tda]=v.data[worknow].second;
                        suf[tda]=v.data[worknow].third;
                        pre[tda]=nowpre;
                    }
                    if(headans[tda].have<headans[tda].size&&(DA[v.data[worknow].second]!=tda))
                    {
                        headans[tda].have++;
                        headans[tda].id[headans[tda].have]=v.data[worknow].second;
                        headans[tda].len[headans[tda].have]=v.data[worknow].first;
                        headans[tda].pre[headans[tda].have]=nowpre;
                        headans[tda].suf[headans[tda].have]=v.data[worknow].third;
                        int now=headans[tda].have;
                        while(now/2)
                        {
                            if(headans[tda].len[now]<headans[tda].len[now/2])
                            {
                                int temp=headans[tda].len[now];
                                headans[tda].len[now]=headans[tda].len[now/2];
                                headans[tda].len[now/2]=temp;
                                temp=headans[tda].id[now];
                                headans[tda].id[now]=headans[tda].id[now/2];
                                headans[tda].id[now/2]=temp;
                                temp=headans[tda].pre[now];
                                headans[tda].pre[now]=headans[tda].pre[now/2];
                                headans[tda].pre[now/2]=temp;
                                temp=headans[tda].suf[now];
                                headans[tda].suf[now]=headans[tda].suf[now/2];
                                headans[tda].suf[now/2]=temp;
                            }else
                            {
                                break;
                            }
                            now/=2;
                        }
                    }else if(v.data[worknow].first>=headans[tda].len[1]&&(DA[v.data[worknow].second]!=tda))
                    {
                        headans[tda].id[1]=v.data[worknow].second;
                        headans[tda].len[1]=v.data[worknow].first;
                        headans[tda].pre[1]=nowpre;
                        headans[tda].suf[1]=v.data[worknow].third;
                        int now=1;
                        while(now<=headans[tda].size)
                        {
                            int nowl=now*2,nowr=now*2+1;
                            if(headans[tda].len[nowl]>headans[tda].len[now] &&
                               headans[tda].len[nowr]>headans[tda].len[now])
                            {
                                break;
                            }
                            else
                            {
                                if(headans[tda].len[nowl]<headans[tda].len[nowr])
                                {
                                    int temp=headans[tda].len[now];
                                    headans[tda].len[now]=headans[tda].len[nowl];
                                    headans[tda].len[nowl]=temp;
                                    temp=headans[tda].id[now];
                                    headans[tda].id[now]=headans[tda].id[nowl];
                                    headans[tda].id[nowl]=temp;
                                    temp=headans[tda].pre[now];
                                    headans[tda].pre[now]=headans[tda].pre[nowl];
                                    headans[tda].pre[nowl]=temp;
                                    temp=headans[tda].suf[now];
                                    headans[tda].suf[now]=headans[tda].suf[nowl];
                                    headans[tda].suf[nowl]=temp;
                                    now=nowl;
                                }else
                                {
                                    int temp=headans[tda].len[now];
                                    headans[tda].len[now]=headans[tda].len[nowr];
                                    headans[tda].len[nowr]=temp;
                                    temp=headans[tda].id[now];
                                    headans[tda].id[now]=headans[tda].id[nowr];
                                    headans[tda].id[nowr]=temp;
                                    temp=headans[tda].pre[now];
                                    headans[tda].pre[now]=headans[tda].pre[nowr];
                                    headans[tda].pre[nowr]=temp;
                                    temp=headans[tda].suf[now];
                                    headans[tda].suf[now]=headans[tda].suf[nowr];
                                    headans[tda].suf[nowr]=temp;
                                    now=nowr;
                                }
                            }
                        }
                    }
                }
            }
            //fprintf(out,"\n");
        }

        if(v.now>0&&v.data[v.now].first==num)
        {
            v.data[v.now].second=i;
            v.data[v.now].third=0;
        }
        else
        {
            v.now++;
            v.data[v.now].first=num;
            v.data[v.now].second=i;
            v.data[v.now].third=0;
        }
    }

    for(i=0;i<d;i++)
    {
        if(ans[i]!=-1)
        {
            fprintf(out,"%d ",i);
            tda=DA[ans[i]];
            fprintf(out,"%d %d %d %d ",tda,len[i],pre[i],suf[i]);
            for(int j=1;j<=headans[i].have;j++)
            {
                fprintf(out," %d %d %d %d",DA[headans[i].id[j]],headans[i].len[j],headans[i].pre[j],headans[i].suf[j]);
                //fprintf(out," %d",(int)(se[DA[headans[i].id[j]]]-SA[headans[i].id[j]]));
            }
            fprintf(out,"\n");
        }
    }
    return 0;
}
/**********************************************************************/
