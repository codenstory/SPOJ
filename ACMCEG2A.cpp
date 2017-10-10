/*
 * ACMCEG2A
 * TOPIC: DP, combinatorics, lexicographic order
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N 20
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
using namespace std;
typedef long long i64;

i64 n,m,fac[N],z[1<<N],F[1<<N],original;
int d[N],c[N],which[1<<21];
char _count[1<<N][10],bts[1<<N];

void recurse( unsigned int available, int i, i64 m ) {
	int k,ok = 0,used[10] = {0};
	i64 w = 0;
	if ( i == n ) {
		assert( !available );
		return ;
	}
	assert( i < n );
	assert( m >= w );
	for ( unsigned int u = available; u && (k = which[L(u)]) >= 0; u &= ~L(u) ) {
		if ( used[d[k]] ) continue ;
		used[d[k]] = 1;
		if ( z[available&~L(u)]+w > m ) { ok = 1; break ; }
		assert( m >= w );
		assert( m >= w+z[available&~L(u)] );
		w += z[available&~L(u)];
	}
	assert( ok );
	c[i] = d[k], recurse(available&~(1ULL<<k),i+1,m-w);
}

i64 lex_rank( int *d, int *summary, int len ) {
	i64 w = 0;
	int used[10] = {0};
	if ( len == 1 )
		return 0;
	assert( len >= 2 );
	for ( int i = 1; i < len; ++i )
		if ( d[i] < d[0] && !used[d[i]] ) {
			--summary[d[i]], used[d[i]] = 1;
			i64 dw = fac[len-1];
			for ( int j = 1; j <= 9; ++j )
				dw /= fac[summary[j]];
			w += dw;
			++summary[d[i]];
		}
	--summary[d[0]];
	return w+lex_rank(d+1,summary,len-1);
}

int main() {
	int i,j,k,ts,o[N],summary[10];
	for ( fac[0] = 1LL, i = 1; i < N; fac[i] = fac[i-1]*i, ++i ) ;
	for ( i = 0; i < (1<<N); bts[i] = bts[i>>1]+(i&1), ++i ) ;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%lld %lld",&n,&m); ) {
		k = 0, original = n;
		memset(summary,0,sizeof summary);
		do { o[k] = d[k] = (n%10), ++k; } while ( n /= 10 ) ;
		for ( i = 0, j = k-1; i < j; ++i, --j )
			swap(o[i],o[j]);
		sort(d,d+k), n = k, z[0] = 1;
		for ( unsigned int v,u = 1; u < (1<<n); ++u ) {
			for ( i = which[L(u)], j = 1; j <= 9; _count[u][j] = _count[u&~L(u)][j], ++j ) ;
			++_count[u][d[i]], z[u] = fac[bts[u]];
			for ( j = 1; j <= 9; ++j ) {
				assert( (z[u] % fac[_count[u][j]]) == 0 ) ;
				z[u] /= fac[_count[u][j]];
			}
		}
		for ( i = 0; i < n; ++summary[o[i++]] ) ;
		i64 e = lex_rank(o,summary,(int)n);
		assert( e+m <= z[MASK(n)] );
		recurse(MASK(n),0,m+e);
		for ( i = 0; i < n; printf("%d",c[i++]) ) ;
		putchar('\n');
	}
	return 0;
}

