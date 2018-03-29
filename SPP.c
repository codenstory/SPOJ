/*
 * SPP
 * TOPIC: matrix expo
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x20)
typedef long long i64;

int m;
i64 a[N],b[N],c[N],alpha,beta,modulo,
	A[N][N],sum;

i64 g( i64 x, i64 y ) {
	i64 z;
	if ( x+y >= modulo )
		return (x+y)%modulo;
	if ( x+y < 0 ) {
		for ( z = x+y; z < 0; z += modulo ) ;
		return z%modulo;
	}
	return (x+y);
}

i64 f( i64 n ) {
	i64 ax[N][N],tmp[N][N],x[N][N],i,j,k,ans = 0,vec[N];

	if ( n == 0 ) return 0;
	assert( n > 0 );
	if ( n <= m ) {
		for ( i = 1; i <= n; ++i )
			ans = g(ans,a[i]);
		return ans;
	}
	assert( n > m );

	for ( j = m, i = 1; i <= m+1; ++i ) 
		vec[i] = (i==1?sum:a[j--]);

	for ( i = 1; i <= m+1; ++i )
		for ( j = 1; j <= m+1; ++j ) 
			ax[i][j] = (i==j?1:0), x[i][j] = A[i][j];

	for ( n -= m; n; n >>= 1 ) {
		if ( n & 1 ) {
			for ( i = 1; i <= m+1; ++i )
				for ( j = 1; j <= m+1; ++j )
					for ( tmp[i][j] = 0, k = 1; k <= m+1; ++k )
						tmp[i][j] = g(tmp[i][j],(ax[i][k]*x[k][j])%modulo);
			for ( i = 1; i <= m+1; ++i )
				for ( j = 1; j <= m+1; ++j )
					ax[i][j] = tmp[i][j];
		}
		for ( i = 1; i <= m+1; ++i )
			for ( j = 1; j <= m+1; ++j )
				for ( tmp[i][j] = 0, k = 1; k <= m+1; ++k )
					tmp[i][j] = g(tmp[i][j],(x[i][k]*x[k][j])%modulo);
		for ( i = 1; i <= m+1; ++i )
			for ( j = 1; j <= m+1; ++j )
				x[i][j] = tmp[i][j];
	}

	for ( j = 1; j <= m+1; ++j )
		ans = g(ans,(ax[1][j]*vec[j])%modulo);
	return ans;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&m);
		assert( m+1 < N );
		for ( sum = 0, i = 1; i <= m; ++i )
			scanf("%lld",&b[i]), a[i] = b[i], sum += a[i];
		for ( i = 1; i <= m; ++i )
			scanf("%lld",&c[i]);
		scanf("%lld %lld %lld",&alpha,&beta,&modulo);
		memset(A,0,sizeof A);
		for ( i=1, j = 1; j <= m+1; ++j )
			A[i][j] = (j==1?1:c[j-1]);
		for ( i=2, j = 1; j <= m+1; ++j )
			A[i][j] = (j==0?0:c[j-1]);
		for ( ;++i <= m+1; )
			for ( j = 1; j <= m+1; ++j )
				A[i][j] = (j==i-1?1:0);
		sum %= modulo, printf("%lld\n",(f(beta)-f(alpha-1)+modulo)%modulo);
	}
	return 0;
}

