/*
 * GCJ1C09C
 * TOPIC: DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<14)
#define M (0x80)
typedef long long i64;
#define oo (1LL<<60)

int n,m,pos[M];
i64 z[M][M];

i64 min( i64 x, i64 y ) { return x > y ? y:x; }

i64 calc_z( int i, int j ) {
	int t;
	if ( z[i][j] < +oo )
		return z[i][j];
	if ( i > j ) return z[i][j] = 0;
	if ( i == j ) 
		return z[i][j] = (pos[i+1]-1)-(pos[i-1]+1);
	for ( t = i; t <= j; ++t )
		z[i][j] = min(z[i][j],pos[j+1]-pos[i-1]-2+calc_z(i,t-1)+calc_z(t+1,j));
	return z[i][j];
}

int main() {
	int i,j,k,ts,cs = 0;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( i = 1; i <= m; scanf("%d",&pos[i++]) ) ;
		pos[0] = 0, pos[m+1] = n+1;
		for ( i = 0; i <= m+1; ++i )
			for ( j = 0; j <= m+1; ++j )
				z[i][j] = +oo;
		printf("Case #%d: %lld\n",++cs,calc_z(1,m));
	}
	return 0;
}

