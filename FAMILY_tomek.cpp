#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<=(b);++i)
#define FORD(i,a,b) for(int i=(a);i>=(b);--i)
#define REP(i,n) for(int i=0;i<(n);++i)
#define VAR(v,x) __typeof(x) v=x
#define FOREACH(i,c) for(VAR(i,(c).begin());i!=(c).end();++i)

template<class T>
inline int size(const T&a) { return a.size(); }

////////

const int NDIGS=40;
const int BASE = 100000000;
const int BASEDIGS = 8;

class Num {
  int a[NDIGS];
  void popraw() {
    int p=0;
    FORD(i,NDIGS-1,0) {
      a[i]+=p;
      p=a[i]/BASE;
      a[i]%=BASE;
    }
  }
  public:
  void set0() { REP(i,NDIGS) a[i]=0; }
  void set1() { REP(i,NDIGS) a[i]=0; a[0]=1; }
  void dodaj(const Num&x,const Num&y) {
    REP(i,NDIGS) a[i]=x.a[i]+y.a[i];
    popraw();
  }
  void div2() {
    int p=0;
    REP(i,NDIGS) {
      a[i]+=p;
      p=(a[i]&1)==0?0:BASE;
      a[i]>>=1;
    }
  }
/*  void dodajIloczyn(const Num&x,const Num&y) {
    REP(i,NDIGS) REP(j,NDIGS-i)
      a[i+j] += x.a[i]*y.a[j];
    popraw();
  } */
  void wypisz() {
    int res[BASEDIGS*(NDIGS-1)+1];
    int p=BASEDIGS*(NDIGS-1)+1;
    FORD(i,NDIGS-1,1) {
      int x=a[i];
      REP(j,BASEDIGS) { res[--p]=x%10; x/=10; }
    }
    res[0]=a[0];
    int aa=0,bb=(NDIGS-1)*BASEDIGS;
    while(aa<2 && res[aa]==0) ++aa;
    while(bb>2 && res[bb]==0) --bb;
    FOR(i,aa,bb) {
      putchar('0'+res[i]);
      if(i==2 && i<bb) putchar('.');
    }
    printf("%%\n");
  }
};


const int MAXN = 301;

int n;
int par[MAXN][2];
bool done[MAXN][MAXN];
Num pr[MAXN][MAXN];
int dnr[MAXN];
int ddd;

void dfs(int x) {
  if(dnr[x]!=-1) return;
  if(par[x][0]!=-1) REP(i,2) dfs(par[x][i]);
  dnr[x]=ddd++;
}

void read() {
  scanf("%d",&n);
  REP(i,n) par[i][0]=par[i][1]=-1;
  int k; scanf("%d",&k);
  REP(i,k) {
    int a,b,c; scanf("%d%d%d",&a,&b,&c); --a; --b; --c;
    par[a][0]=b;
    par[a][1]=c;
  }
}

void calc(int a,int b) {
  if(done[a][b]) return;
  if(a==b) { pr[a][b].set1(); done[a][b]=true; return; }
  if(dnr[a]<dnr[b]) swap(a,b);
  if(par[a][0]==-1) swap(a,b);
  if(par[a][0]==-1) {
    pr[a][b].set0();
  }
  else {
    calc(par[a][0],b);
    calc(par[a][1],b);
    pr[a][b].dodaj(pr[par[a][0]][b],pr[par[a][1]][b]);
    pr[a][b].div2();
  }
  pr[b][a]=pr[a][b];
  done[a][b]=done[b][a]=true;
}

int main() {
	int ts;
	for ( scanf("%d",&ts); ts--; ) {
  read();
  REP(i,n) dnr[i]=-1;
  ddd=0;
  REP(i,n) dfs(i);
  REP(a,n) REP(b,n) done[a][b]=false;
  int m; scanf("%d",&m);
  REP(i,m) {
    int a,b;
    scanf("%d%d",&a,&b);
    --a; --b;
    calc(a,b);
    pr[a][b].wypisz();
  }
	}
}
