/*
 * NETADMIN
 * TOPIC: maxflow
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
#define MAXV (0x400)
#define MAXE (1<<21)
#define min(x,y) ((x)<(y)?(x):(y))
#define oo (1<<30)

int n,m,h[N],len,src = 0,sink,V,E,
	last[MAXV],next[MAXE],to[MAXE],visited[MAXV],yes,seen[MAXV],indeed,q[MAXV],*head,*tail,d[MAXV],
	cap[MAXE],flow[MAXE];

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q,d[*tail++=sink]=0,visited[sink]=++yes; head<tail;)
		for ( i = last[x=*head++]; i >= 0; i = next[i] )
			if ( visited[y=to[i]] != yes && cap[i^1] > flow[i^1] )
				visited[*tail++=y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	int i,y,dt;
	if ( x == sink )
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y=to[i]] == yes )
			if ( d[y]+1 == d[x] && (dt = dfs(y,min(cap[i]-flow[i],df))) ) {
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
	int i,j,k,ts,total;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&n,&m,&k);
		for ( src = 0, len = 0; len < k; scanf("%d",&h[len++]), --h[len-1] );
		for ( E = 0, V = n+1, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for (;m-- && 2 == scanf("%d %d",&i,&j);) {
			--i, --j;
			if ( !(0 <= i && i < n && 0 <= j && j < n) )
				continue ;
			add_arcs(i,j,1), add_arcs(j,i,1);
		}
		if ( len == 0 ) {
			puts("0");
			continue ;
		}
		for ( i = 0; i < len; add_arcs(h[i++],sink,1) ) ;
		if ( (total = maxflow()) == len ) {
			printf("%d\n",1);
			continue ;
		}
		for ( k = 2; k <= len+4; ++k ) {
			for ( i = 0; i < E; ++i )
				if ( cap[i] > 0 && to[i] != sink )
					++cap[i];
			if ( (total += maxflow()) >= len ) {
				printf("%d\n",k);
				break ;
			}
		}
	}
	return 0;
}

