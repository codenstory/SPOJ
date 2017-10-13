/*
 * RPLA
 * TOPIC: topological sort, DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<10)
enum { D, T };

int n,adj[2][N][N],deg[2][N],g[2][N][N],yes,cnt[N],*head,*tail,q[N],rank[N],
	e[N];

int cmp( const void *a, const void *b ) {
	int *x = (int *)a,
		*y = (int *)b;
	if ( *x == *y ) return 0;
	return *x<*y?-1:1;
}

int max( int x, int y ) { return x < y ? y:x; }

int main() {
	int i,j,k,ts,cs = 0,t,x,y,m;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( t = D; t <= T; ++t )
			for ( i = 0; i < n; deg[t][i++] = 0 ) ;
		for ( ++yes; m-- && 2 == scanf("%d %d",&j,&i); ) {
			if ( g[D][i][j] == yes )
				continue ;
			g[D][i][j] = g[T][j][i] = yes;
			adj[D][i][deg[D][i]++] = j;
			adj[T][j][deg[T][j]++] = i;
		}
		for ( t = D; t <= T; ++t )
			for ( i = 0; i < n; ++i )
				qsort(adj[t][i],deg[t][i],sizeof *adj[t][i],cmp);
		for ( i = 0; i < n; rank[i] = -1, cnt[i] = deg[T][i], ++i ) ;
		for ( head = tail = q, i = 0; i < n; ++i )
			if ( !cnt[i] ) rank[*tail++ = i] = 1;
		for ( ;head < tail; ) {
			x = *head++;
			for ( i = 0; i < deg[D][x]; ++i ) {
				y = adj[D][x][i];
				rank[y] = max(rank[y],rank[x]+1);
				if ( !--cnt[y] )
					*tail++ = y;
			}
		}
		for ( i = 0; i < n; ++i )
			e[i] = (i|(rank[i]<<10));
		qsort(e,n,sizeof *e,cmp);
		for ( printf("Scenario #%d:\n",++cs), i = 0; i < n; ++i )
			printf("%d %d\n",(e[i]>>10),e[i]&1023);
	}
	return 0;
}

