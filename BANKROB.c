/*
 * BANKROB
 * TOPIC: maxflow, mincut
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXV (0x800)
#define MAXE (1<<21)
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))

int V,E,m,n,to[MAXE],last[MAXV],next[MAXE],cap[MAXE],flow[MAXE],src,sink,
	visited[MAXV],yes,seen[MAXV],indeed,d[MAXV],*head,*tail,q[MAXV];

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0;
	flow[i] = flow[j] = 0;
}

int bfs() {
	int i,x,y;
	for ( head = tail = q, d[*tail++ = sink] = 0, visited[sink] = ++yes; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes )
				visited[*tail++ = y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	int i,y,dt;
	if ( x == sink ) 
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( visited[y = to[i]] == yes && d[y]+1 == d[x] )
			if ( cap[i] > flow[i] )
				if ( dt = dfs(y,min(cap[i]-flow[i],df)) ) {
					flow[i] += dt, flow[i^1] -= dt;
					return dt;
				}
	return 0;
}

int maxflow() {
	int T = 0,dt;
	for (;bfs(src,sink);)
		for (;++indeed && (dt = dfs(src,+oo)); T += dt );
	return T;
}

int main() {
	int i,j,k,ss,tt;
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		scanf("%d %d",&ss,&tt), --ss, --tt;
		for ( E = 0, V = n+n, i = 0; i < V; last[i++] = -1 ) ;
		for ( k = 0; k < m; ++k ) 
			scanf("%d %d",&i,&j), --i, --j, add_arcs(i+n,j,+oo), add_arcs(j+n,i,+oo);
		for ( i = 0; i < n; add_arcs(i,i+n,1), ++i ) ;
		src = ss+n, sink = tt, printf("%d\n",maxflow());
	}
	return 0;
}

