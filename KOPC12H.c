/*
 * KOPC12H
 * TOPIC: dp, combinatorics, decimal range decomposition
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#define N 0x10
#define S 0x80
typedef long long i64;

i64 c[N][N],
	e[S][N],o[S][N],
	w[N];

i64 f( int d0, int *d, int m, int alpha, int beta ) {
	int dig,t,i,j,k,s,q;
	i64 ans = 0;
	if ( m == 1 ) {
		for ( dig = d0; dig <= d[0]-1; ++dig )
			if ( (dig&1) && beta+dig < alpha )
				++ans;
			else if ( !(dig&1) && beta < alpha+dig )
				++ans;
		return ans;
	}
	assert( m >= 2 );
	for ( dig = d0; dig <= d[m-1]-1; ++dig )
		for ( t = 0; t <= m-1; ++t ) {
			if ( dig&1 ) {
				for ( s = 0; s < S; ++s )
					for ( q = 0; q < s-dig+alpha-beta; ++q )
						ans += c[m-1][t]*e[s][t]*o[q][m-1-t];
			}
			else {
				for ( s = 0; s < S; ++s )
					for ( q = 0; q < s+dig+alpha-beta; ++q )
						ans += c[m-1][t]*e[s][t]*o[q][m-1-t];
			}
		}
	assert( dig == d[m-1] );
	(dig&1)?(beta+=dig):(alpha+=dig);
	return ans+f(0,d,m-1,alpha,beta);
}

int is_good( int *d, int n ) {
	int even = 0, odd = 0, i;
	for ( i = 0; i < n; ++i )
		if ( d[i]&1 )
			odd += d[i];
		else even += d[i];
	return even>odd?1:0;
}

i64 query( int n ) {
	int d[N],i,j,k,m = 0;
	if ( n == 0 ) return 0;
	for ( ;n; d[m++]=(n%10), n/=10 );
	assert( m >= 1 );
	/*printf("f = %lld\n",f(1,d,m,0,0));*/
	return w[m-1]+f(1,d,m,0,0)+is_good(d,m);
}

i64 qr( int n ) {
	int d[N],i,j,k,m = 0;
	if ( n == 0 ) return 0;
	for ( ;n; d[m++] = (n%10), n /= 10 );
	assert( m >= 1 );
	return is_good(d,m);
}

int main() {
	int i,j,k,t,s,q,m,ts;
	i64 ans;
	for ( i = 0; i < N; c[i++][0] = 1 ) ;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			c[i][j] = c[i-1][j]+c[i-1][j-1];
	for ( e[0][0] = 1, s = 0; s < S; ++s )
		for ( k = 1; k < N; ++k )
			for ( t = 0; t <= s && t <= 8; t += 2 )
				e[s][k] += e[s-t][k-1];
	for ( o[0][0] = 1, s = 1; s < S; ++s )
		for ( k = 1; k < N; ++k )
			for ( t = 1; t <= s && t <= 9; t += 2 )
				o[s][k] += o[s-t][k-1];
	for ( m = 1; m < N; ++m )
		for ( t = 0; t <= m; ++t )
			for ( s = 0; s < S; ++s )
				for ( q = 0; q < s; ++q )
					w[m] += c[m][t]*e[s][t]*o[q][m-t];
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&i,&j); printf("%lld\n",query(j)-query(i-1)) ) ;
	return 0;
}

