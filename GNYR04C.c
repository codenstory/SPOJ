/*
 * GNYR04C
 * TOPIC: DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 11
#define MAXM 2001
#define oo (0xffffffffffffffffull)
#define BIT(k) (1ULL<<(k))
typedef unsigned long long u64;

int n,m,seen_z[MAXN][MAXM],seen_w[MAXN][MAXM];
u64 z[MAXN][MAXM],w[MAXN][MAXM];

u64 calc_z(int,int);

u64 f( u64 x, u64 y ) {
	assert( y < +oo && x < oo-y );
	return x+y;
}

u64 calc_w( int i, int k ) {
	if ( seen_w[i][k] )
		return w[i][k];
	seen_w[i][k] = 1;
	if ( i == 0 )
		return 0;
	if ( k < BIT(i-1) )
		return 0;
	return w[i][k] = f(calc_w(i,k-1),calc_z(i,k));
}

u64 calc_z( int i, int k ) {
	int t;
	if ( seen_z[i][k] )
		return z[i][k];
	seen_z[i][k] = 1;
	if ( i == 0 ) 
		return 0;
	if ( i == 1 )
		return z[i][k] = k>=1?1:0;
	if ( k == BIT(i-1) ) 
		return z[i][k] = 1ULL;
	if ( k < BIT(i-1) )
		return z[i][k] = 0ULL;
	return z[i][k] = calc_w(i-1,k>>1);
}

int main() {
	int i,j,k,ts,cs = 0;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		printf("Data set %d: %d %d %llu\n",++cs,n,m,calc_w(n,m));
	}
	return 0;
}

