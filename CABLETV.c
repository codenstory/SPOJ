/*
 * CABLETV
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x400
#define MAXV 0x80
#define MAXE (1<<21)
#define min(x,y) ((x)<(y)?(x):(y))
#define oo (1<<30)

int m,n,to[MAXE],next[MAXE],last[MAXV],q[MAXV],*head,*tail,visited[MAXV],yes,seen[MAXV],indeed,cap[MAXE],flow[MAXE],
	E,V,d[MAXV],g[N][N];

int bfs( int src, int sink ) {
	int x,y,i;
	for ( head=tail=q, d[*tail++ = sink] = 0, visited[sink] = ++yes; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes ) 
				visited[*tail++ = y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, const int sink, int df ) {
	int y,dt,i;
	if ( x == sink ) 
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i]>flow[i] && visited[y=to[i]]==yes && d[y]+1==d[x] )
			if ( dt=dfs(y,sink,min(df,cap[i]-flow[i])) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

int maxflow( int src, int sink ) {
	int T = 0,dt;
	for(;bfs(src,sink);)
		for (;++indeed&&(dt=dfs(src,sink,+oo));T+=dt) ;
	return T;
}

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

void init( int u, int v ) {
	int x,y,i,j,src,sink;
	for ( E = 0, V = n+n, src = u, sink = v+n, i = 0; i < V; last[i++] = -1 ) ;
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			if ( g[i][j] )
				add_arcs(i+n,j,+oo);
	for ( i = 0; i < n; ++i )
		if ( i == u || i == v )
			add_arcs(i,i+n,+oo);
		else add_arcs(i,i+n,1);
}

int main() {
	int i,j,k,ts,w;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); printf("%d\n",w) ) {
		for ( memset(g,0,sizeof g); m-- && 2 == scanf("%*[( \n]%d%*[, ]%d%*[) \n]",&i,&j); g[i][j] = g[j][i] = 1 ) ;
		for ( w = n, i = 0; i < n; ++i )
			for ( j = i+1; j < n; ++j ) {
				if ( g[i][j] ) continue ;
				init(i,j), k = maxflow(i+n,j), w = min(w,k);
			}
	}
	return 0;
}

