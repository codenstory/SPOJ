/*
 * HC12
 * TOPIC: combinatorics
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000007LL)
#define N (1<<14)
typedef long long i64;

int n,m;
i64 a[N],fac[N],ifac[N];

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( *x == *y )
		return 0;
	return *x<*y?-1:1;
}

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) {
	!y?(*a=1,*b=0,*d=x):(ext_euclid(y,x%y,b,a,d),*b-=(*a)*(x/y));
}

i64 inv( i64 x ) {
	i64 a,b,d;
	for ( ext_euclid(x,MOD,&a,&b,&d); a < 0; a += MOD ) ;
	return a%MOD;
}

i64 C( int n, int k ) {
	i64 c = fac[n]*ifac[k];
	c %= MOD;
	return (c*ifac[n-k])%MOD;
}

int main() {
	int i,j,k,ts,cs = 0;
	i64 ans;
	for ( i = 0, fac[i] = ifac[i] = 1LL; ++i < N; )
		ifac[i] = inv(fac[i] = (fac[i-1]*i)%MOD);
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for( i = 0; i < n; scanf("%lld",&a[i++]) ) ;
		qsort(a,n,sizeof *a,cmp);
		printf("Case #%d: ",++cs);
		for ( ans = 0, i = m-1; i < n; i = j ) {
			for ( j = i; j < n && a[j] == a[i]; ++j ) ;
			ans += a[i]*C(j-1,m-1), ans %= MOD;
		}
		printf("%lld\n",ans);
	}
	return 0;
}

