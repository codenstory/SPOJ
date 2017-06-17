/*
 * XOINC: A Coin Game
 * TOPIC: dp O(n^3) ----> O(n^2)
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x800
#define oo (1LL<<60)
enum { S, T };
typedef long long i64;

int n;
i64 c[N],z[N][2][N],pref[N];

int main() {
	int i,j,k,t,l;
	i64 curmin;
	for ( ;1 == scanf("%d",&n); printf("%lld\n",z[n][S][1]) ) {
		for ( i = n; i >= 1; scanf("%lld",&c[i--]) ) ;
		for ( i = 1; i <= n; ++i )
			pref[i] = pref[i-1]+c[i];
		for ( k = 0; k <= n; ++k )
			for ( t = S; t <= T; ++t )
				for ( l = 0; l <= n; ++l )
					z[k][t][l] = -oo;
		for ( l = 1; l <= n; ++l )
			for ( t = S; t <= T; z[0][t++][l] = 0 ) ;
		for ( k = 1; k <= n; ++k )
			for ( t = S; t <= T; ++t ) {
				for ( l = 1, curmin = +oo, i = 1; i <= 2*l && i <= k; ++i )
					if ( z[k-i][t^1][i] < curmin )
						curmin = z[k-i][t^1][i];
				assert( -oo < curmin && curmin < +oo );
				for ( z[k][t][l] = pref[k]-curmin; (++l) <= n; z[k][t][l] = pref[k]-curmin ) {
					if ( k-(2*l-1) >= 0 && curmin > z[k-(2*l-1)][t^1][2*l-1] )
						curmin = z[k-(2*l-1)][t^1][2*l-1];
					if ( k-2*l >= 0 && curmin > z[k-2*l][t^1][2*l] )
						curmin = z[k-2*l][t^1][2*l];
					assert( -oo < curmin && curmin < +oo );
				}
			}
	}
	return 0;
}

