/*
 * DCEPC807
 * TOPIC: DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)
#define L(k) ((k)&((~(k))+1ULL))
#define oo (1<<30)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
enum { ALICE, BOB };

char which[N];
int z[2][N];

int calc_z( int t, unsigned int x ) {
	unsigned int u,i,j,k;
	if ( !x || !(x&(x-1)) ) 
		return z[t][x] = t^1;
	if ( z[t][x] < +oo )
		return z[t][x];
	for ( u = x; u; u &= ~L(u) ) 
		if ( calc_z(t^1,x&~L(u)) == t )
			return z[t][x] = t;
	for ( u = x; u && (i = which[L(u)]) >= 0; ) {
		for ( j = i; (1<<j) <= x && ((u>>j)&1); ++j ) ;
		for ( k = i; k+1 < j; ++k ) 
			if ( calc_z(t^1,x&~(3ULL<<k)) == t )
				return z[t][x] = t;
		u &= ~(MASK(j-i)<<i);
	}
	return z[t][x] = (t^1);
}

int main() {
	int i,j,k,ts,t,n;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( t = ALICE; t <= BOB; ++t )
		for ( i = 0; i < N; z[t][i++] = +oo ) ;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); puts(calc_z(ALICE,n)==ALICE?"Alice":"Bob") ) ;
	return 0;
}

