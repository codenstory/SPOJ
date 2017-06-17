/*
 * ABA12C: Buying Apples
 * TOPIC: dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define MAXW (0x80)
#define oo (1LL<<43)
#define min(x,y) ((x)<(y)?(x):(y))
typedef long long i64;

int m,n,price[N];
i64 z[N][MAXW];

i64 calc_z( int i, int w ) {
	i64 zz;
	int t;
	if ( z[i][w] < +oo )
		return z[i][w];
	if ( w == 0 )
		return z[i][w] = 0;
	if ( i == 0 )
		return z[i][w] = +oo;
	assert( i >= 1 );
	assert( w >= 1 );
	if ( price[i] == -1 ) 
		return z[i][w] = calc_z(i-1,w);
	for ( t = 0; t*i <= w; ++t ) 
		if ( (zz = calc_z(i-1,w-i*t)) < +oo )
			z[i][w] = min(z[i][w],zz+price[i]*t);
	return z[i][w];
}

int main() {
	int i,j,k,ts;
	i64 res;
	for (scanf("%d",&ts);ts--&&2==scanf("%d %d",&n,&m);) {
		for ( i = 1; i <= m; scanf("%d",&price[i++]) ) ;
		for ( i = 0; i <= m; ++i )
			for ( k = 0; k <= m; z[i][k++] = +oo ) ; 
		if ( (res = calc_z(m,m)) < +oo )
			printf("%lld\n",res);
		else puts("-1");
	}
	return 0;
}

