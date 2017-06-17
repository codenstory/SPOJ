/*
 * TREENUM: 
 * TOPIC: dp, combinatorics, binary
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1LL<<32)
#define N 0x40
#define TST(u,k) (((u)>>(k))&1LL)
#define BIT(k) (1LL<<(k))
#define MASK(k) (BIT(k)-1LL)
typedef long long i64;
typedef unsigned long long u64;

i64 L,R,C[N][N],deg[N],two[N],pref[N];

i64 pw( i64 x, i64 n ) {
	i64 ax = 1LL;
	for ( ;n; n >>= 1, x *= x, x %= MOD )
		if ( n&1 ) ax *= x, ax %= MOD;
	return ax;
}

i64 mult( i64 x, i64 y ) {
	return (((u64)x)*((u64)y))%MOD;
}

i64 f( i64 n ) {
	i64 ans = 0,i,j,k;
	for ( k = N-1; k >= 1; --k ) 
		if ( TST(n,k) ) {
			ans += mult(pref[k-1],two[k-1]), ans %= MOD;
			ans += mult(deg[k],1+(n&MASK(k))), ans %= MOD;
			/*printf("n = %lld, k = %lld, deg[%lld] = %lld, rest = %lld\n",n,k,k,deg[k],1+(n&MASK(k)));*/
		}
	assert( k == 0 );
	if ( TST(n,0) ) ans += deg[0], ans %= MOD;
	/*printf("n = %lld, ans = %lld\n",n,ans);*/
	return ans;
}

int main() {
	int i,j,k,ts;
	i64 ans;
	for ( deg[0] = two[0] = 1LL, i = 1; i < N; ++i )
		for ( deg[i] = (deg[i-1]*3)%MOD, two[i] = (two[i-1]<<1)%MOD; deg[i] < 0; deg[i] += MOD ) ;
	for ( pref[0] = 1LL, i = 1; i < N; ++i )
		for ( pref[i] = (pref[i-1]+deg[i])%MOD; pref[i] < 0; pref[i] += MOD ) ;
	for ( i = 0; i < N; ++i ) C[i][0] = 1LL;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			C[i][j] = C[i-1][j]+C[i-1][j-1], C[i][j] %= MOD;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%lld %lld",&L,&R); ) {
		for ( ans = f(R)-f(L-1)+MOD; ans < 0; ans += MOD );
		printf("%lld\n",(ans%MOD));
	}
	return 0;
}

