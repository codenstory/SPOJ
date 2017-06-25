/*
 * ITRIX12E
 * TOPIC: matrix exponentiation, graphs, matrix power, modular inverse, Gauss elimination
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 9
#define MOD (1000000000LL+7LL)
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
typedef long long i64;

int is_prime( int n ) {
	int k;
	if ( n == 1 ) return 0;
	for ( k = 2; k*k <= n; ++k )
		if ( 0 == (n%k) )
			return 0;
	return 1;
}

i64 g[N][N],n,t[N][N],a[N][N],b[N][N],ax[N][N],x[N][N];

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) { !y?(*a=1,*b=0,*d=x):(ext_euclid(y,x%y,b,a,d),*b-=(*a)*(x/y)); }
i64 inv( i64 x ) {
	i64 a,b,d;
	for ( ext_euclid(x,MOD,&a,&b,&d); a < 0; a += MOD ) ;
	assert( d == 1 );
	return a%MOD;
}

int get_pivot( int k, int *r, int *c ) {
	int i,j;
	for ( j = k, i = k; i < N; ++i )
		if ( a[i][j] ) {
			*r = i, *c = j;
			return 1;
		}
	return 0;
}

int main() {
	int i,j,k,ts,row,col;
	i64 cf,ans;
	for ( i = 0; i < N; ++i )
		for ( j = 0; j < N; ++j )
			if ( is_prime(i+j+2) )
				g[i][j] = 1;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%lld",&n); ) {
		for ( ans = 0, i = 1; i <= N; ++i )
			if ( is_prime(i) ) ++ans;
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				a[i][j]=(x[i][j]=g[i][j])-(i==j?1:0), ax[i][j]=b[i][j]=(i==j?1:0);
		for ( k = 0; k < N; ++k ) {
			assert( get_pivot(k,&row,&col) ) ;
			assert( a[row][col] );
			for ( j = 0; j < N; ++j )
				xchg(a[k][j],a[row][j]),xchg(b[k][j],b[row][j]);
			assert( a[k][k] );
			for ( cf = inv(a[k][k]), j = 0; j < N; ++j )
				a[k][j] *= cf, a[k][j] %= MOD, b[k][j] *= cf, b[k][j] %= MOD;
			assert( a[k][k] == 1 );
			for ( i = k+1; i < N; ++i ) {
				for ( cf = a[i][k], j = 0; j < N; ++j ) {
					for ( a[i][j] -= (cf*a[k][j])%MOD; a[i][j] < 0; a[i][j] += MOD ) ;
					for ( b[i][j] -= (cf*b[k][j])%MOD; b[i][j] < 0; b[i][j] += MOD ) ;
				}
				assert( a[i][k] == 0 ) ;
			}
		}
		for ( k = N-1; k >= 1; --k ) 
			for ( i = k-1; i >= 0; --i ) {
				if ( a[i][k] == 0 ) continue ;
				for ( cf = a[i][k], j = 0; j < N; ++j ) {
					for ( a[i][j] -= (cf*a[k][j])%MOD; a[i][j] < 0; a[i][j] += MOD );
					for ( b[i][j] -= (cf*b[k][j])%MOD; b[i][j] < 0; b[i][j] += MOD );
				}
				assert( a[i][k] == 0 );
			}

		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				if ( i == j ) 
					assert( a[i][j] == 1 );
				else assert( a[i][j] == 0 );

		for ( ; n; n >>= 1 ) {
			if ( n&1 ) {
				for ( i = 0; i < N; ++i )
					for ( j = 0; j < N; ++j )
						for ( t[i][j] = 0, k = 0; k < N; ++k )
							t[i][j] += (ax[i][k]*x[k][j])%MOD, t[i][j] %= MOD;
				for ( i = 0; i < N; ++i )
					for ( j = 0; j < N; ++j )
						ax[i][j] = t[i][j];
			}
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N; ++j )
					for ( t[i][j] = 0, k = 0; k < N; ++k )
						t[i][j] += (x[i][k]*x[k][j])%MOD, t[i][j] %= MOD;
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N; ++j )
					x[i][j] = t[i][j];
		}
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				for ( ax[i][j] -= g[i][j]; ax[i][j] < 0; ax[i][j] += MOD );
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				for ( t[i][j] = 0, k = 0; k < N; ++k )
					t[i][j] += (ax[i][k]*b[k][j])%MOD, t[i][j] %= MOD;
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				ans += t[i][j], ans %= MOD;
		printf("%lld\n",ans);
	}
	return 0;
}

