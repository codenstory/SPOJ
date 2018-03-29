/*
 * TETRAHRD
 * TOPIC: matrix expo
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000007LL)
typedef long long i64;
#define N 5

i64 A[N][N];

i64 f( i64 x, i64 y ) {
	i64 z = (x+y);
	if ( z >= MOD )
		return z % MOD;
	if ( z < 0 ) {
		for ( ;z < 0; z += MOD );
		return z;
	}
	return z;
}

i64 g( i64 x, i64 y ) {
	long long a = x, b = y, z = (a*b);
	if ( z >= MOD )
		return z%MOD;
	return z;
}

i64 getelem( int n ) {
	i64 ax[N][N],x[N][N],tmp[N][N],i,j,k,ans = 0,
		vec[] = {0,1,0,0,0};
	if ( n <= 0 ) return 0;
	if ( n == 1 || n == 2 ) return 0;
	if ( n == 3 ) return 1;
	assert( n >= 4 );
	for ( i = 0; i < N; ++i )
		for ( j = 0; j < N; ++j )
			ax[i][j] = (i==j?1:0), x[i][j] = A[i][j];
	for ( n -= 2; n; n >>= 1 ) {
		if ( n & 1 ) {
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N; ++j ) 
					for ( tmp[i][j] = 0, k = 0; k < N; ++k )
						tmp[i][j] = f(tmp[i][j],g(ax[i][k],x[k][j]));
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N; ++j )
					ax[i][j] = tmp[i][j];
		}
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j ) 
				for ( tmp[i][j] = 0, k = 0; k < N; ++k )
					tmp[i][j] = f(tmp[i][j],g(x[i][k],x[k][j]));
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				x[i][j] = tmp[i][j];
	}
	for ( ans = 0, j = 0; j < N; ++j )
		ans = f(ans,g(ax[0][j],vec[j]));
	return ans;
}

int main() {
	int i,j,k,ts,m,n;
	i64 ans;
	for ( j = 0; j < N; ++j )
		if ( j < 2 ) A[0][j] = 1;
		else A[0][j] = 0;
	for ( j = 0; j < N; ++j )
		if ( !j ) A[1][j] = 0;
		else A[1][j] = 1;
	for ( i = 2; i < N; ++i )
		for ( j = 0; j < N; ++j )
			A[i][j] = (i==j+1?1:0);
	/*
	for ( i = 0; i < N; ++i ) {
		for ( j = 0; j < N; ++j )
			printf("%lld ",A[i][j]);
		puts("");
	}*/
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&m,&n);
		ans = getelem(n)-getelem(m-1);
		if ( ans < 0 ) ans += MOD;
		printf("%lld\n",ans%MOD);
	}
	return 0;
}

