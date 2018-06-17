/*
 * ANDROUND
 * TOPIC: segment tree
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (1<<21)
#define Q N
#define BIT(k) (1LL<<(k))
#define MASK(k) (BIT(k)-1LL)
typedef long long i64;
using namespace std;

int n,m;
i64 K,a[N],tr[Q];

i64 build( int v, int i, int j ) {
	if ( i == j )
		return tr[v]= a[i];
	int k= (i+j)>>1;
	return tr[v]= (build(v<<1,i,k)&build((v<<1)+1,k+1,j));
}

i64 query( int v, int i, int j, int qi, int qj ) {
	if ( qi > j || qj < i )
		return MASK(45);
	if ( qi <= i && j <= qj )
		return tr[v];
	int k= (i+j)>>1;
	return query(v<<1,i,k,qi,qj)&query(1+(v<<1),k+1,j,qi,qj);
}

int main() {
	int i,j,k,cs,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %lld",&n,&K);
		for ( i= 0; i < n; ++i )
			scanf("%lld",&a[i]);
		for ( j= n, k= 1; k <= 4; ++k )
			for ( i= 0; i < n; a[j++]= a[i++] ) ;
		assert( j == 5*n );
		int ll= 2*n, rr= 3*n;
		if ( !(K%=(2*n)) )
			K= 2*n;
		n*= 5, build(1,0,n-1);
		for ( i= ll; i < rr-1; ++i )
			printf("%lld ",query(1,0,n-1,i-K,i+K));
		printf("%lld\n",query(1,0,n-1,i-K,i+K));
	}
	return 0;
}

