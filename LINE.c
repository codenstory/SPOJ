/*
 * LINE
 * TOPIC: DP
 * status: Accepted
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x10)
#define L(k) ((k)&((~(k))+1ULL))
#define oo (1<<30)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)

double S( double x ) { return x*x; }
char which[1<<21];
double x[2][N],y[2][N],z[1<<N][N][2],dist[N][N][2][2],len[N];
int n;

int main() {
	int i,j,k,cs = 0,t;
	unsigned int u,v,w;
	double ans;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( ;1 == scanf("%d",&n) && n; ) {
		for ( i = 0; i < n; ++i ) {
			scanf("%lf %lf %lf %lf",&x[0][i],&y[0][i],&x[1][i],&y[1][i]);
			len[i] = sqrt(S(x[0][i]-x[1][i])+S(y[0][i]-y[1][i]));
		}
		for ( u = 0; u < (1<<n); ++u )
			for ( i = 0; i < n; ++i )
				for ( t = 0; t <= 1; z[u][i][t++] = +oo ) ;
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j ) 
				if ( i != j )
				for ( t = 0; t <= 1; ++t )
					for ( k = 0; k <= 1; ++k ) {
						dist[i][j][t][k] = sqrt(S(x[t][i]-x[k][j])+S(y[t][i]-y[k][j]));
					}
		for ( i = 0; i < n; ++i )
			z[1<<i][i][0] = z[1<<i][i][1] = len[i];
		for ( u = 0; u < (1<<n); ++u )
			for ( v = u; v && (i=which[L(v)]) >= 0; v &= ~L(v) )
				for ( t = 0; t <= 1; ++t )
					if ( z[u][i][t] < +oo ) 
						for ( w = ((~u)&MASK(n)); w && (j=which[L(w)]) >= 0; w &= ~L(w) )
							for ( k = 0; k <= 1; ++k )
								if ( z[u|BIT(j)][j][k^1] > z[u][i][t]+dist[i][j][t][k]+len[j] )
									 z[u|BIT(j)][j][k^1] = z[u][i][t]+dist[i][j][t][k]+len[j];
		for ( ans = +oo, u = MASK(n), i = 0; i < n; ++i )
			for ( t = 0; t <= 1; ++t )
				if ( z[u][i][t] < ans )
					ans = z[u][i][t];
		printf("Case %d: %.5lf\n",++cs,ans);
	}
	return 0;
}

