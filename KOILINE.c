/*
 * KOILINE
 * TOPIC: fenwick tree, BIT, binary search, recovering permutation, invariants, increasing functions
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define L(k) ((k)&((~(k))+1ULL))
#define N (1<<17)
typedef long long i64;

int n,K,m;
i64 tr[N<<1],c[N],e[N],x[N];

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( *x == *y )
		return 0;
	return *x<*y?-1:1;
}

void update( unsigned int i ) {
	for ( ;i <= (1<<K); ++tr[i], i += L(i) ) ;
}

i64 prefix( unsigned int u ) {
	i64 s = 0;
	for ( ;u; s += tr[u], u &= ~L(u) ) ;
	return s;
}

i64 sum( unsigned int i, unsigned int j ) {
	if ( i > j ) return 0;
	return prefix(j)-prefix(i-1);
}

i64 f( unsigned int x ) {
	return x-1-sum(1,x-1);
}

int main() {
	int i,j,k,low,high,mid;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%lld",c+i);
		for ( K = 0; (1<<K) < n; ++K ) ;
		for ( i = 0; i <= (1<<K); tr[i++] = 0 ) ;
		qsort(c+1,n,sizeof *c,cmp);
		for ( i = 1; i <= n-1; ++i )
			assert( c[i] <= c[i+1] ) ;
		for ( i = 1; i <= n; scanf("%lld",&x[i++]) ) ;
		for ( i = n; i >= 1; --i ) {
			if ( f(high = n) == x[i] ) {
				update(e[i] = n);
				continue ;
			}
			assert( f(high) > x[i] );
			for ( low = 1; low+1 < high; f(mid=(low+high)/2)<=x[i]?(low=mid):(high=mid) ) ;
			update(e[i] = low);
		}
		for ( i = 1; i <= n; ++i )
			printf("%lld\n",c[e[i]]);
	}
	return 0;
}

