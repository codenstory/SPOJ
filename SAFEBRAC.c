/*
 * SAFEBRAC
 * TOPIC: dp, matrix power, repeated squaring
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
#define MOD 1000000007LL
#define A 11
typedef long long i64;

int n;
i64 z[N],ax[A][A],t[A][A],x[A][A],
	g[A][A] = {
			   0,0,0,0,0,0,0,1,0,0,0,
			   0,0,1,0,1,0,0,0,0,0,0,
			   0,1,0,1,0,1,0,0,0,0,0,
			   0,0,1,0,0,0,1,0,0,0,0,
			   0,1,0,0,0,1,0,1,0,0,0,
			   0,0,1,0,1,0,1,0,1,0,0,
			   0,0,0,1,0,1,0,0,0,1,0,
			   1,0,0,0,1,0,0,0,1,0,0,
			   0,0,0,0,0,1,0,1,0,1,0,
			   0,0,0,0,0,0,1,0,1,0,0,
			   1,1,1,1,1,1,1,1,1,1,0
			 };

int main() {
	int i,j,k,ts;
	i64 ans;
	for ( i = 0; i < A; ++i )
		for ( j = 0; j < A; ++j )
			ax[i][j] = (i==j?1:0);
	/*
	for ( z[0] = 1, n = 1; n < N; ++n ) {
		for ( i = 0; i < A; ++i )
			for ( j = 0; j < A; ++j )
				for ( t[i][j] = 0, k = 0; k < A; ++k )
					t[i][j] += ax[i][k]*g[k][j], t[i][j] %= MOD;
		for ( i = 0; i < A; ++i )
			for ( j = 0; j < A; ax[i][j] = t[i][j], ++j ) ;
		for ( z[n] = 0, i = 0; i < A-1; z[n] += ax[A-1][i++], z[n] %= MOD ) ;
	}*/
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < A; ++i )
			for ( j = 0; j < A; ax[i][j]=(i==j?1:0),x[i][j]=g[i][j], ++j ) ;
		for ( ;n; n >>= 1 ) {
			if ( n&1 ) {
				for ( i = 0; i < A; ++i )
					for ( j = 0; j < A; ++j )
						for ( t[i][j] = 0, k = 0; k < A; ++k )
							t[i][j] += ax[i][k]*x[k][j], t[i][j] %= MOD;
				for ( i = 0; i < A; ++i )
					for ( j = 0; j < A; ++j )
						ax[i][j] = t[i][j];
			}
			for ( i = 0; i < A; ++i )
				for ( j = 0; j < A; ++j )
					for ( t[i][j] = 0, k = 0; k < A; ++k )
						t[i][j] += x[i][k]*x[k][j], t[i][j] %= MOD;
			for ( i = 0; i < A; ++i )
				for ( j = 0; j < A; ++j )
					x[i][j] = t[i][j];
		}
		for ( ans = 0, i = 0; i < A-1; ++i )
			ans += ax[A-1][i], ans %= MOD;
		printf("%lld\n",ans);
	}
	return 0;
}

