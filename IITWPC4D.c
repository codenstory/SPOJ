/*
 * IITWPC4D -- Arrangement Validity
 * TOPIC: fenwick tree, binary search, classics, very neat, inversions, recovering permutation, interview
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define L(k) ((k)&((~(k))+1ULL))
typedef long long i64;
#define N (1<<17)
#define oo (1<<60)

int n,c[N],K,x[N];
i64 tr[N<<1];

void update( unsigned int i, int dt ) {
	for ( ;i <= (1<<K); tr[i] += dt, i += L(i) );
}

i64 prefix( unsigned int i ) {
	i64 s = 0;
	for ( ;i; s += tr[i], i &= ~L(i) ) ;
	return s;
}

i64 sum( unsigned int i, unsigned int j ) {
	return prefix(j)-prefix(i-1);
}

i64 f( int k ) {
	return k+sum(k,n);
}

int main() {
	int i,j,k,ts,cs = 0,ok,low,high,mid;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		printf("Test : %d\n",++cs);
		for ( K = 0; (1<<K) < n; ++K );
		for ( i = 0; i <= (1<<K); tr[i++] = 0 ) ;
		for ( i = 1; i <= n; scanf("%d",&c[i++]) ) ;
		for ( i = 1; i <= n; x[i++] = -1 ) ;
		for ( ok = 1, i = n; i >= 1 && ok; --i ) {
			if ( f(high = n) < n-c[i] ) { 
				ok = 0;
				break ;
			}
			assert( f(high) >= n-c[i] ) ;
			if ( f(high) == n-c[i] ) {
				if ( sum(high,high) == 1 ) {
					ok = 0;
					break ;
				}
				update(x[i]=high,1);
				continue ;
			}
			if ( f(low=1) > n-c[i] ) {
				ok = 0;
				break ;
			}
			assert( f(low) <= n-c[i] && f(high) > n-c[i] );
			for ( ;low+1 < high; f(mid=(low+high)/2)>(n-c[i])?(high=mid):(low=mid) ) ;
			if ( f(low) == n-c[i] ) {
				if ( sum(low,low) == 1 ) {
					ok = 0;
					break ;
				}
				update(x[i]=low,1);
			}
		}
		if ( !ok ) { puts("-1"); continue ; }
		for ( i = 1; i <= n-1; printf("%d ",x[i++]) ) ;
		printf("%d\n",x[n]);
	}
	return 0;
}

