/*
 * INGRED
 * TOPIC: DP, bitmasks
 * status: Accepted
 */
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x80)
#define M 8
#define oo (MASK(32))
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k) & (~(k)+1ULL))
using namespace std;

int n,m,deg[N],adj[N][N*N],id[N],sx,sy,
	x[N*N],y[N*N],roads,s[M],which[1<<21];
unsigned int d[N][N],w[N*N],z[1<<M][N][N];

int main() {
	int i,j,k,t,l,ni,nj;
	unsigned int u,v,ans,dw;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( ;2 == scanf("%d %d",&n,&roads); ) {
		for ( i = 0; i < n; deg[i++] = 0 ) ;
		for ( i = 0; i < n; ++i )
			for ( j = i+1; j < n; ++j )
				d[j][i] = d[i][j] = +oo;
		for ( i = 0; i < roads && 3 == scanf("%d %d %u",x+i,y+i,w+i); d[x[i]][y[i]] = min(d[x[i]][y[i]],w[i]), d[y[i]][x[i]] = min(d[y[i]][x[i]],w[i]), ++i ) ;
		for ( k = 0; k < n; ++k )
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					if ( d[i][k] < +oo && d[k][j] < +oo && d[i][j] > d[i][k]+d[k][j] )
						d[i][j] = d[i][k]+d[k][j];
		for ( i = 0; i < roads; ++i )
			adj[x[i]][deg[x[i]]++] = i, adj[y[i]][deg[y[i]]++] = i;
		scanf("%d",&m), memset(id,-1,sizeof id);
		for ( i = 0; i < m; scanf("%d",&k), s[i++] = k ) ;
		sort(s,s+m);
		for ( i = 0; i < m; ++i ) id[s[i]] = i;
		scanf("%d %d",&sx,&sy);
		if ( sx > sy ) swap(sx,sy);
		memset(z,0xff,sizeof z), u = 0;
		if ( id[sx] >= 0 ) u |= BIT(id[sx]);
		if ( id[sy] >= 0 ) u |= BIT(id[sy]);
		for ( z[u][sx][sy] = 0, u = 0; u < (1<<m); ++u )
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					if ( z[u][i][j] < +oo ) {
						for ( v = ((~u)&MASK(m)); v && (k = which[L(v)]) >= 0; v &= ~L(v) ) {
							if ( d[i][s[k]] < +oo ) {
								ni = s[k], nj = j, dw = d[i][s[k]];
								if ( ni > nj ) swap(ni,nj);
								if ( z[u|BIT(k)][ni][nj] > z[u][i][j]+dw )
									z[u|BIT(k)][ni][nj] = z[u][i][j]+dw;
							}
							if ( d[j][s[k]] < +oo ) {
								ni = i, nj = s[k], dw = d[j][s[k]];
								if ( ni > nj ) swap(ni,nj);
								if ( z[u|BIT(k)][ni][nj] > z[u][i][j]+dw )
									z[u|BIT(k)][ni][nj] = z[u][i][j]+dw;
							}
						}
					}
		for ( ans = +oo, u = MASK(m), i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( z[u][i][j] < ans )
					ans = z[u][i][j];
		printf("%u\n",ans);
	}
	return 0;
}

