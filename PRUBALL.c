/*
 * PRUBALL
 * TOPIC: dp, egg-drop problem
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 0x40
#define N 0x400
#define oo (1<<30)

int min( int x, int y ) { return x<y?x:y; }
int max( int x, int y ) { return x<y?y:x; }

int m,n,z[M][N];

int calc_z( int i, int j ) {
	int t,w;
	if ( z[i][j] < +oo )
		return z[i][j];
	if ( j == 0 )
		return z[i][j] = 0;
	assert( j >= 1 );
	if ( i == 0 )
		return z[i][j] = +oo;
	if ( i == 1 ) 
		return z[i][j] = j;
	assert( i >= 2 );
	for ( t = 1; t <= j; ++t ) {
		w = max(calc_z(i-1,t-1),calc_z(i,j-t));
		if ( w < +oo )
			z[i][j] = min(z[i][j],1+w);
	}
	return z[i][j];
}

int main() {
	int i,j,k,ts,cs;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&cs,&m,&n);
		for ( i = 0; i <= m; ++i )
			for ( j = 0; j <= n; ++j )
				z[i][j] = +oo;
		printf("%d %d\n",cs,calc_z(m,n));
	}
	return 0;
}

