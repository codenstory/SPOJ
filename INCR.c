/*
 * INCR
 * TOPIC: DP on permutations
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef long long i64;
#define MOD (1000000000LL)
#define N 42
#define oo (N-1)

int a[N][N][N][N][N];
char expanded[N][N][N][N][N];
/*
 * a[n][i][j][k][t] -- the number of permutations of {1..n}
 * such that the first index at which we have an increasing subsequence
 * of length 2 is i,
 * of length 3 is j,
 * of length 4 is k,
 * of length 5 is t.
 * The cells a[n][i][j][+oo][+oo] contain the number of permutations
 * that do not have increasing subsequences of length 4 and 5
 */

void add( int *dst, int w ) { (*dst) += w, (*dst) %= MOD; }

void forward_dp( int n, int i, int j, int k, int t ) {
	int x, weight = a[n][i][j][k][t];
	if ( i < +oo && j < +oo && k < +oo && t < +oo ) {
		assert( i < j && j < k && k < t );
		for ( x = t; x >= k+1; --x )
			add(&a[n+1][i][j][k][x],weight);
		for ( x = k; x >= j+1; --x )
			add(&a[n+1][i][j][x][t+1],weight);
		for ( x = j; x >= i+1; --x )
			add(&a[n+1][i][x][k+1][t+1],weight);
		for ( x = i; x >= 2; --x )
			add(&a[n+1][x][j+1][k+1][t+1],weight);
		add(&a[n+1][i+1][j+1][k+1][t+1],weight);
		return ;
	}
	if ( i < +oo && j < +oo && k < +oo ) {
		assert( i < j && j < k );
		for ( x = k+1; x <= n+1; ++x )
			add(&a[n+1][i][j][k][x],weight);
		for ( x = k; x >= j+1; --x )
			add(&a[n+1][i][j][x][+oo],weight);
		for ( x = j; x >= i+1; --x )
			add(&a[n+1][i][x][k+1][+oo],weight);
		for ( x = i; x >= 2; --x )
			add(&a[n+1][x][j+1][k+1][+oo],weight);
		add(&a[n+1][i+1][j+1][k+1][+oo],weight);
		return ;
	}
	if ( i < +oo && j < +oo ) {
		assert( i < j );
		for ( x = j+1; x <= n+1; ++x )
			add(&a[n+1][i][j][x][+oo],weight);
		for ( x = j; x >= i+1; --x )
			add(&a[n+1][i][x][+oo][+oo],weight);
		for ( x = i; x >= 2; --x )
			add(&a[n+1][x][j+1][+oo][+oo],weight);
		add(&a[n+1][i+1][j+1][+oo][+oo],weight);
		return ;
	}
	if ( i < +oo ) {
		for ( x = i+1; x <= n+1; ++x )
			add(&a[n+1][i][x][+oo][+oo],weight);
		for ( x = i; x >= 2; --x )
			add(&a[n+1][x][+oo][+oo][+oo],weight);
		add(&a[n+1][i+1][+oo][+oo][+oo],weight);
		return ;
	}
	for ( x = 2; x <= n+1; ++x )
		add(&a[n+1][x][+oo][+oo][+oo],weight);
	add(&a[n+1][+oo][+oo][+oo][+oo],weight);
}

void dp_precalc() {
	int i,j,k,t,n,
		ii,jj,kk,tt;

	a[1][+oo][+oo][+oo][+oo] = 1LL;

	for ( n = 1; n < N-1; ++n )
		for ( ii = 1; ii <= n+1; ++ii )
			for ( jj = ii; jj <= n+1; ++jj )
				for ( kk = jj; kk <= n+1; ++kk )
					for ( tt = kk; tt <= n+1; ++tt ) {
						i = (ii>n?+oo:ii);
						j = (jj>n?+oo:jj);
						k = (kk>n?+oo:kk);
						t = (tt>n?+oo:tt);
						if ( !a[n][i][j][k][t] || expanded[n][i][j][k][t] ) continue ;
						expanded[n][i][j][k][t] = 1, forward_dp(n,i,j,k,t);
					}
}

int main() {
	int i,j,k,t,ts,n,m;
	i64 ans;
	dp_precalc();
	for (scanf("%d",&ts);ts--&&2==scanf("%d %d",&n,&m);) {
		if ( n == 1 ) {
			puts(m==1?"1":"0");
			continue ;
		}
		assert( n >= 2 );
		if ( n < m ) {
			puts("0");
			continue ;
		}
		if ( m == 1 ) {
			puts("1");
			continue ;
		}
		assert( m >= 2 );
		if ( m == 2 ) {
			for ( ans = 0, i = 2; i <= n; ++i )
				ans += a[n][i][+oo][+oo][+oo], ans %= MOD;
		}
		else if ( m == 3 ) {
			for ( ans = 0, i = 2; i <= n; ++i )
				for ( j = i+1; j <= n; ++j )
					ans += a[n][i][j][+oo][+oo], ans %= MOD;
		}
		else if ( m == 4 ) {
			for ( ans = 0, i = 2; i <= n; ++i )
				for ( j = i+1; j <= n; ++j )
					for ( k = j+1; k <= n; ++k )
						ans += a[n][i][j][k][+oo], ans %= MOD;
		}
		else {
			assert( m == 5 );
			for ( ans = 0, i = 2; i <= n; ++i )
				for ( j = i+1; j <= n; ++j )
					for ( k = j+1; k <= n; ++k )
						for ( t = k+1; t <= n; ++t )
							ans += a[n][i][j][k][t], ans %= MOD;
		}
		printf("%lld\n",ans);
	}
	return 0;
}

