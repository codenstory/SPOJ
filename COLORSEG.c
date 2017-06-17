/*
 * COLORSEG: Coloring Segments
 * TOPIC: dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD 1000003LL
#define N 0x40
#define M 0x40
#define oo (1LL<<60)
typedef long long i64;

int gcd( int x, int y ) { return !y?x:gcd(y,x%y); }

int n,m,coprime[M+M][M+M];
i64 z[M][N][M][M];

i64 calc_z( const int m, int i, int j, int k ) {
	int t;
	if ( z[m][i][j][k] < +oo )
		return z[m][i][j][k];
	assert( j >= 1 && k >= 1 );
	assert( j <= m && k <= m );
	if ( i == 2 ) 
		return z[m][i][j][k] = 1LL;
	if ( i < 2 )
		return z[m][i][j][k] = 0LL;
	assert( i >= 3 );
	z[m][i][j][k] = 0LL;
	for ( t = 1; t <= m; ++t )
		if ( coprime[t+j][j+k] )
			z[m][i][j][k] += calc_z(m,i-1,t,j), z[m][i][j][k] %= MOD;
	return z[m][i][j][k];
}

int main() {
	int i,j,k,t,ts,cs = 0;
	i64 ans;
	for ( m = 0; m < M; ++m )
		for ( n = 0; n < N; ++n )
			for ( j = 0; j <= m; ++j )
				for ( k = 0; k <= m; ++k )
					z[m][n][j][k] = +oo;
	for ( k = 2; k < M+M; ++k )
		for ( t = 2; t < M+M; ++t )
			coprime[k][t] = gcd(k,t)==1;
	for ( scanf("%d",&ts); ts-- && 2==scanf("%d %d",&n,&m); ) {
		printf("Case %d: ",++cs);
		if ( n == 2 ) {
			printf("%d\n",m*m);
			continue ;
		}
		if ( n == 1 ) {
			printf("%d\n",m);
			continue ;
		}
		assert( n >= 3 );
		for ( ans = 0, j = 1; j <= m; ++j )
			for ( k = 1; k <= m; ++k )
				ans += calc_z(m,n,j,k), ans %= MOD;
		printf("%lld\n",ans);
	}
	return 0;
}

