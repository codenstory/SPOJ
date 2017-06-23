/*
 * BRPAR: Brackets Parade
 * TOPIC: bracket sequence, combinatorics
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000000LL+7LL)
#define N 0x800
#define M (0x80)
typedef long long i64;

i64 f[N],invf[N],r[N],c[N][N];
int m,n,d[M];

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) {
	!y?(*a=1,*b=1,*d=x):(ext_euclid(y,x%y,b,a,d),(*b)-=(*a)*(x/y));
}

i64 inv( i64 x, i64 P ) {
	i64 a,b,d;
	for ( ext_euclid(x,P,&a,&b,&d); a < 0; a += P );
	return a%P;
}

int main() {
	int i,j,k,ts;
	i64 ans;
	for ( f[0] = invf[0] = 1LL, i = 1; i < N; ++i )
		f[i] = (f[i-1]*i)%MOD, invf[i] = ((r[i]=inv(i,MOD))*invf[i-1])%MOD;
	for ( i = 0; i < N; ++i )
		c[i][0] = 1LL;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			c[i][j] = (c[i-1][j-1]+c[i-1][j])%MOD;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&m); ) {
		for ( n = 0, i = 0; i < m; ++i )
			scanf("%d",&d[i]), n += d[i];
		for ( ans = 1LL, i = 0; i < m; ++i )
			ans *= invf[d[i]], ans %= MOD;
		ans *= f[n], ans %= MOD;
		ans *= r[n+1], ans %= MOD;
		printf("%lld\n",(ans*c[2*n][n])%MOD);
	}
	return 0;
}

