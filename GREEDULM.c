/*
 * GREEDULM
 * TOPIC: DP, optimal binary search trees, Knuth optimisation
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#define oo (DBL_MAX-0x400)
#define N 0x200
#define tol 1e-9
typedef long long i64;

int n,K[N][N];
long double pp[N],pq[N],c[N][N];
i64 p[N],q[N],s,weight[N];

long double sum( long double *p, int i, int j ) {
	return p[j]-(i>=1?p[i-1]:0);
}

int main() {
	int i,j,k,t;
	for ( ;1 == scanf("%d",&n) && n; ) {
		for ( s = 0, i = 1; i <= n; ++i )
			scanf("%lld",&p[i]), s += p[i];
		for ( i = 0; i <= n; ++i )
			scanf("%lld",&q[i]), s += q[i];
		for ( i = 1; i <= n; ++i )
			pp[i] = pp[i-1]+(p[i]+0.00)/s;
		for ( i = 0; i <= n; ++i )
			pq[i] = (i>=1?pq[i-1]:0)+(q[i]+0.00)/s;
		for ( i = 0; i <= n; ++i )
			for ( j = i; j <= n; ++j )
				c[i][j] = +oo;
		for ( k = 1, i = 0; (j=i+k-1) <= n; c[i][j] = 0, K[i][j] = i, ++i ) ;
		for ( ;++k <= n+1; )
			for ( i = 0; (j = i+k-1) <= n; ++i ) 
				for ( t = K[i][j-1]; t <= K[i+1][j]; ++t )
				/*for ( t = i+1; t <= j; ++t )*/
					if ( c[i][t-1]+c[t][j]+sum(pp,i+1,j)+sum(pq,i,j) <= c[i][j] )
						c[i][j] = c[i][t-1]+c[t][j]+sum(pp,i+1,j)+sum(pq,i,j), K[i][j] = t;
		printf("%.Lf\n",c[0][n]*s+tol);
	}
	return 0;
}

