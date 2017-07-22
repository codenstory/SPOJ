/*
 * BRKSTRNG
 * TOPIC: Knuth optimisation, DP, Monge condition, quadrangle inequality
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 0x400
#define oo (1LL<<60)
typedef long long i64;

i64 n,z[M][M],pos[M];
int m,K[M][M];

int main() {
	int i,j,k,t;
	for ( ;2 == scanf("%lld %d",&n,&m); ) {
		for ( pos[0] = 0, pos[m+1] = n, i = 1; i <= m; ++i )
			scanf("%lld",&pos[i]);
		for ( i = 0; i <= m+1; ++i )
			for ( j = 0; j <= m+1; z[i][j++] = +oo ) ;
		for ( k = 1, i = 0; (j = i+k-1) <= m+1; z[i][j] = 0, K[i][j] = i, ++i ) ;
		for ( ++k, i = 0; (j=i+k-1) <= m+1; z[i][j] = 0, K[i][j] = j, ++i ) ;
		for (;++k <= m+2;)
			for ( i = 0; (j = i+k-1) <= m+1; ++i ) {
				for ( t = K[i][j-1]; t <= K[i+1][j]; ++t )
					if ( z[i][t] < +oo && z[t][j] < +oo && z[i][j] >= z[i][t]+z[t][j]+(pos[j]-pos[i]) )
						z[i][j] = z[i][t]+z[t][j]+(pos[j]-pos[i]), K[i][j] = t;
			}
		printf("%lld\n",z[0][m+1]);
	}
	return 0;
}

