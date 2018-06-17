/*
 * POSTERS
 * TOPIC: segment tree with lazy propagation
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <set>
#include <map>
typedef long long i64;
using namespace std;
#define N (1<<16)
#define Q (N<<4)

int n,x[N],y[N],nn,val[Q],h[Q];
map<int,int> m;
set<int> s;

void push_down( int v, int i, int j ) {
	if ( !h[v] )
		return ;
	if ( i == j ) {
		val[v]= h[v], h[v]= 0;
		return ;
	}
	int k= (i+j)>>1;
	h[v<<1]= h[(v<<1)+1]= h[v], val[v]= h[v], h[v]= 0;
}

void build( int v, int i, int j ) {
	if ( i == j ) {
		val[v]= h[v]= 0LL;
		return ;
	}
	int k= (i+j)>>1;
	build(2*v,i,k), build(2*v+1,k+1,j);
}

void update( int v, int i, int j, int qi, int qj, int newval) {
	if ( qi > j || qj < i ) 
		return ;
	push_down(v,i,j);
	if ( qi <= i && j <= qj ) {
		h[v]= newval, push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(v<<1,i,k,qi,qj,newval), update((v<<1)+1,k+1,j,qi,qj,newval);
}

int query( int v, int i, int j, int qi ) {
	if ( qi > j || qi < i )
		return 0LL;
	push_down(v,i,j);
	if ( qi <= i && j <= qi )
		return val[v];
	int k= (i+j)>>1;
	return qi<=k?query(v<<1,i,k,qi):query((v<<1)+1,k+1,j,qi);
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ){
		for ( nn= 0, m.clear(), s.clear(), i= 0; i < n; ++i )
			scanf("%d %d",x+i,y+i), s.insert(x[i]), s.insert(y[i]);
		for ( set<int> :: iterator it= s.begin(); it != s.end(); m[*it++]= nn++ ) ;
		for ( i= 0; i < n; x[i]= m[x[i]], y[i]= m[y[i]], ++i ) ;
		for ( build(1,0,nn-1), i= 0; i < n; ++i )
			update(1,0,nn-1,x[i],y[i],i+1);
		for ( s.clear(), i= 0; i < nn; ++i )
			s.insert(query(1,0,nn-1,i));
		printf("%lu\n",s.size());
	}
	return 0;
}

