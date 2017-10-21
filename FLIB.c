/*
 * FLIB
 * TOPIC: matrix expo
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000007LL)
#define LIMIT (1LL<<45)
typedef long long i64;

i64 A[3][3] = {1,3,2,0,5,3,0,3,2},
	ax[3][3],x[3][3],n,tmp[3][3];

i64 f( i64 x, i64 y ) {
	i64 z;
	if ( (z = x+y) >= MOD ) {
		return z%MOD;
	}
	if ( z < 0 ) {
		for ( ;z < 0; z += MOD ) ;
		return z % MOD;
	}
	return z;
	if ( x+y >= LIMIT )
		return (x+y)%MOD;
	if ( x+y <= -LIMIT ) {
		for ( z = x+y; z < 0; z += LIMIT ) ;
		return z%MOD;
	}
	return x+y;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%lld",&n); ) {
		for ( i = 0; i < 3; ++i )
			for ( j = 0; j < 3; ++j ) {
				ax[i][j] = (i==j?1:0);
				x[i][j] = A[i][j];
			}
		if ( n == 0 ) {
			puts("0");
			continue ;
		}
		for ( --n; n; n >>= 1 ) {
			if ( n & 1 ) {
				for ( i = 0; i < 3; ++i )
					for ( j = 0; j < 3; ++j )
						for ( tmp[i][j] = 0, k = 0; k < 3; ++k )
							tmp[i][j] = f(tmp[i][j],ax[i][k]*x[k][j]);
				for ( i = 0; i < 3; ++i )
					for ( j = 0; j < 3; ++j )
						ax[i][j] = tmp[i][j];
			}
			for ( i = 0; i < 3; ++i )
				for ( j = 0; j < 3; ++j )
					for ( tmp[i][j] = 0, k = 0; k < 3; ++k )
						tmp[i][j] = f(tmp[i][j],x[i][k]*x[k][j]);
			for ( i = 0; i < 3; ++i )
				for ( j = 0; j < 3; ++j )
					x[i][j] = tmp[i][j];
		}
		printf("%lld\n",(f(2*ax[0][0]+3*ax[0][1],2*ax[0][2]))%MOD);
	}
	return 0;
}

