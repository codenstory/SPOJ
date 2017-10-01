/*
 * TFRIENDS
 * TOPIC: SCC
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
enum { D, T };

int n,st[N],*top,adj[2][N][N],deg[2][N],scc[N],mark,
	seen[N],yes;
char g[N][N];

void dfs( const int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = 0; i < deg[t][x]; ++i )
		if ( seen[y = adj[t][x][i]] != yes )
			dfs(t,y);
	if ( t ) *++top = x;
	else scc[x] = mark;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%s",g[i++]) ) ;
		memset(deg,0,sizeof deg);
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == 'Y' )
					adj[D][i][deg[D][i]++] = j, adj[T][j][deg[T][j]++] = i;
		for ( ++yes, mark = -1, top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes ) 
				dfs(T,i);
		for ( ++yes; top > st; )
			if ( seen[i = *top--] != yes )
				++mark, dfs(D,i);
		printf("%d\n",mark+1);
	}
	return 0;
}

