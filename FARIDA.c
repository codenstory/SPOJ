/*
 * TOPIC: trivial dp, good for interviews
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef long long i64;
#define N (10100)
#define oo (1LL<<60)
enum { NOTAKE, TAKE };

int n;
i64 c[N],z[N][2];

int main() {
	int i,j,k,ts,cs = 0,t;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n);
		for ( i = 1; i <= n; ++i )
			scanf("%lld",&c[i]);
		for ( i = 0; i <= n; ++i )
			for ( t = 0; t <= 1; ++t )
				z[i][t] = -oo;
		for ( z[0][NOTAKE] = 0, i = 1; i <= n; ++i ) {
			if ( z[i-1][NOTAKE] > -oo && z[i][TAKE] < z[i-1][NOTAKE]+c[i] )
				z[i][TAKE] = z[i-1][NOTAKE]+c[i];
			if ( z[i-1][TAKE] > -oo && z[i][NOTAKE] < z[i-1][TAKE] )
				z[i][NOTAKE] = z[i-1][TAKE];
			if ( z[i-1][NOTAKE] > -oo && z[i][NOTAKE] < z[i-1][NOTAKE] )
				z[i][NOTAKE] = z[i-1][NOTAKE];
		}
		printf("Case %d: %lld\n",++cs,z[n][TAKE]>z[n][NOTAKE]?z[n][TAKE]:z[n][NOTAKE]);
	}
	return 0;
}

