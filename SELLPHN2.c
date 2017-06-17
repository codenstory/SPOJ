/*
 * SELLPHN2: Mobile Company 2
 * TOPIC: maxflow 
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
#define MAXV (N+N)
#define MAXE (1<<21)
#define min(x,y) ((x)<(y)?(x):(y))
#define oo (1<<30)

int dig[256],L,m,n,F[N],S[N],
	to[MAXE],last[MAXE],cap[MAXE],flow[MAXE],next[MAXE],
	d[MAXV],seen[MAXV],visited[MAXV],yes,indeed,
	q[MAXV],*head,*tail,
	src,sink,V,E;

int getnum() {
	int n = 0,ch;
	for (;(ch = getchar()) != EOF && !('0'<=ch && ch<='9'); );
	for ( n = ch-'0'; (ch = getchar()) != EOF && '0' <= ch && ch <= '9'; n = 10*n+ch-'0' );
	return n;
}

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q,d[*tail++=sink] = 0, visited[sink]=++yes; head < tail;)
		for ( i = last[x=*head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes )
				visited[*tail++ = y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	int dt;
	int i,y;
	if ( x == sink )
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y=to[i]] == yes && d[y]+1 == d[x] )
			if ( dt = dfs(y,min(df,cap[i]-flow[i])) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

int maxflow() {
	int T = 0,dt;
	for (;bfs();)
		for ( ;++indeed && (dt = dfs(src,+oo)); T += dt );
	return T;
}

int main() {
	int i,j,k,ts,src2;
	for ( i = '0'; i <= '9'; dig[i++] = 1 ) ;
	for ( ts = getnum(); ts--; ) {
		L = getnum(), m = getnum(), n = getnum();
		for ( i = 0; i < m; F[i++] = getnum() ) ;
		for ( i = 0; i < n; S[i++] = getnum() ) ;
		for ( V = m+n+3, src = V-3, src2 = V-2, sink = V-1, E = 0, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				add_arcs(i,j+m,getnum());
		add_arcs(src,src2,L);
		for ( i = 0; i < m; ++i )
			add_arcs(src2,i,F[i]);
		for ( j = 0; j < n; ++j )
			add_arcs(j+m,sink,S[j]);
		printf("%d\n",maxflow());
	}
	return 0;
}

