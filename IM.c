/*
 * IM
 * TOPIC: maxflow, vertex capacities
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXV (1<<16)
#define MAXE (1<<21)
#define min(x,y) ((x)<(y)?(x):(y))
#define oo (1<<30)

int last[MAXV],next[MAXE],to[MAXE],flow[MAXE],cap[MAXE],V,E,d[MAXV],visited[MAXV],yes,
	src,sink,q[MAXV],*tail,*head,seen[MAXV],indeed;

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0;
	flow[i] = flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q, visited[*tail++=sink]=++yes, d[sink]=0; head<tail; )
		for ( i=last[x=*head++]; i>=0; i=next[i] )
			if ( cap[i^1]>flow[i^1] && visited[y=to[i]]!=yes )
				d[*tail++=y]=d[x]+1, visited[y]=yes;
	return visited[src]==yes;
}

int dfs( int x, int df ) {
	int dt,i,y;
	if ( x == sink ) return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y = to[i]] == yes && d[y]+1 == d[x] )
			if ( (dt = dfs(y,min(df,cap[i]-flow[i]))) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

int maxflow() {
	int T = 0, dt;
	for (;bfs();)
		for (;++indeed&&(dt=dfs(src,+oo));T+=dt);
	return T;
}

int main() {
	int i,j,k,ts,m,n;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d %d",&n,&m), src = n+n, sink = src+1, V = sink+1, E = 0, i = 0; i < V; last[i++] = -1 ) ;
		for ( k = 0; k < m; ++k ) {
			scanf("%d %d",&i,&j);
			if ( !(1 <= i && i <= n && 1 <= j && j <= n) ) continue ;
		   	--i, --j, add_arcs(i+n,j,1), add_arcs(j+n,i,1);
		}
		for ( i = 0; i < n; add_arcs(i,i+n,1), ++i ) ;
		add_arcs(src,0,1), add_arcs(src,2,1), add_arcs(1+n,sink,2), add_arcs(1,1+n,1);
		if ( n == 2 ) {
			puts("NO");
			continue ; 
		}
		puts(maxflow()==2?"YES":"NO");
	}
	return 0;
}

