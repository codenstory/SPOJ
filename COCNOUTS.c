/*
 * COCNUTS
 * TOPIC: mincut, maxflow-mincut, neat
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
#define MAXV (1<<21)
#define MAXE (1<<21)
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))

int m,n,carry[N],src,sink,q[MAXV],*head,*tail,visited[MAXV],yes,seen[MAXV],indeed,g[N][N],
	last[MAXV],next[MAXE],to[MAXE],cap[MAXE],flow[MAXE],E,V,d[MAXV];

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q, visited[*tail++=sink] = ++yes, d[sink] = 0; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes )
				d[*tail++ = y] = d[x]+1, visited[y] = yes;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	int i,y,dt;
	if ( x == sink ) 
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y = to[i]] == yes )
			if ( d[y]+1 == d[x] && (dt = dfs(y,min(df,cap[i]-flow[i]))) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

int maxflow() {
	int T = 0,dt;
	for (;bfs();)
		for (;++indeed && (dt = dfs(src,+oo)); T += dt ) ;
	return T;
}

int main() {
	int i,j,k;
	for ( ;2 == scanf("%d %d",&n,&m) && (n||m); ) {
		for ( i = 0; i < n; scanf("%d",&carry[i++]) ) ; 
		for ( memset(g,0,sizeof g); m-- && 2==scanf("%d %d",&i,&j); --i, --j, g[i][j] = g[j][i] = 1 ) ;
		for ( E = 0, V = n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < n; ++i )
			if ( carry[i] ) 
				add_arcs(i,sink,1);
			else add_arcs(src,i,1);
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] )
					add_arcs(i,j,1);
		printf("%d\n",maxflow());
	}
	return 0;
}

