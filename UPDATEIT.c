/*
 * UPDATEIT
 * TOPIC: BIT, prefix sums, difference array
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define L(k) ((k)&((~(k))+1ULL))
typedef long long i64;

int n,qr,K,m;
i64 A[N],C[N],tr[N<<1];

void update( unsigned int i, int dt ) {
	for ( ;i < m; tr[i] += dt, i += L(i) ) ;
}

i64 prefix( unsigned int i ) {
	i64 ans = 0;
	for ( ;i; ans += tr[i], i &= ~L(i) ) ;
	return ans;
}

int main() {
	int i,j,k,ts,l,r,val;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&qr);
		for ( K = 0; (m=(1<<K)) <= n; ++K ) ;
		for ( i = 1; i <= n; C[i++] = 0 ) ;
		for ( i = 1; i <= n; A[i] = C[i]-C[i-1], ++i ) ;
		for ( i = 0; i < m; tr[i++] = 0 ) ;
		for ( i = 1; i <= n; ++i )
			update(i,A[i]);
		for ( ;qr--; ) {
			scanf("%d %d %d",&l,&r,&val), ++l, ++r;
			update(l,val), update(r+1,-val);
		}
		for ( scanf("%d",&qr); qr--; ) {
			scanf("%d",&i);
			printf("%lld\n",prefix(i+1));
		}
	}
	return 0;
}

