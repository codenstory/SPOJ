/*
 * COURAGE
 * TOPIC: segment tree with updates
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;
#define N (1<<17)
#define Q (N<<5)
#define L(k) ((k)<<1)
#define R(k) (L(k)|1)
#define oo (1LL<<45)
typedef long long i64;

int n;
i64 tr[Q],a[N],mi[Q];

i64 build( int v, int i, int j ) {
	if ( i == j ) 
		return mi[v]= tr[v]= a[i];
	int k= (i+j)>>1;
	tr[v]= build(L(v),i,k)+build(R(v),k+1,j);
	mi[v]= min(mi[L(v)],mi[R(v)]);
	return tr[v];
}

i64 update( int v, int i, int j, int pos, i64 dt ) {
	if ( pos < i || pos > j )
		return tr[v];
	if ( i == j ) {
		mi[v]= (tr[v]+= dt);
		return tr[v];
	}
	int k= (i+j)>>1;
	tr[v]= update(L(v),i,k,pos,dt)+update(R(v),k+1,j,pos,dt);
	mi[v]= min(mi[L(v)],mi[R(v)]);
	return tr[v];
}

i64 sum_query( int v, int i, int j, int qi, int qj ) {
	if ( qi > j || qj < i )
		return 0LL;
	if ( qi <= i && j <= qj )
		return tr[v];
	int k= (i+j)>>1;
	return sum_query(L(v),i,k,qi,qj)+sum_query(R(v),k+1,j,qi,qj);
}

i64 min_query( int v, int i, int j, int qi, int qj ) {
	if ( qi > j || qj < i )
		return +oo;
	if ( qi <= i && j <= qj )
		return mi[v];
	int k= (i+j)>>1;
	return min(min_query(L(v),i,k,qi,qj),min_query(R(v),k+1,j,qi,qj));
}

int main() {
	int i,j,k,qr,pos;
	i64 dt;
	char comm[0x400];
	for ( ;1 == scanf("%d",&n); ) {
		for ( i= 0; i < n; scanf("%lld",&a[i++]) ) ;
		for ( build(1,0,n-1), scanf("%d",&qr); qr-- && 1 == scanf("%s",comm); ) {
			switch ( 0[comm] ) {
				case 'E': scanf("%lld %d",&dt,&pos); update(1,0,n-1,pos,-dt); break ;
				case 'G': scanf("%lld %d",&dt,&pos); update(1,0,n-1,pos,dt);  break ;
				case 'C': scanf("%d %d",&i,&j); printf("%lld\n",sum_query(1,0,n-1,i,j)-min_query(1,0,n-1,i,j)); break ; 
				default: assert( 0 );
			}
		}
	}
	return 0;
}

