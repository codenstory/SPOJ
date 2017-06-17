/*
 * EN (entrapment)
 * TOPIC: mincut, bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXV (1<<17)
#define MAXE (1<<21)
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))

int n,m,E,V,last[MAXV],next[MAXE],to[MAXE],cap[MAXE],flow[MAXE],p[MAXV],visited[MAXV],yes,
	src,sink,q[MAXV],*head,*tail,d[MAXV];

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int i,x,y;
	for ( head=tail=q, d[src]=0, visited[*tail++ = src] = ++yes; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i] > flow[i] && visited[y = to[i]] != yes ) 
				visited[*tail++ = y] = yes, p[y] = i, d[y]=d[x]+1;
	return visited[sink] == yes;
}

void maxflow() {
	int x,i,res;
	assert( bfs() );
	for ( i=p[sink],x=to[i^1],++flow[i],--flow[i^1]; x!=src; i=p[x],x=to[i^1],++flow[i],--flow[i^1] );
	assert( 0 == bfs() ) ;
	for ( x = -2, res = +oo, i = 1; i < n; ++i ) 
		if ( visited[i] == yes && visited[i+n] != yes && res > d[i] ) 
			res = d[x = i];
	printf("%d\n",x<0?n:x+1);
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d %d",&n,&m), E = 0, V = n+n, src = 0, sink = V-1, i = src; i <= sink; last[i++] = -1 ) ;
		for ( ;m-- && 2 == scanf("%d %d",&i,&j); --i, --j, add_arcs(i+n,j,+oo) ) ;
		for ( i = 0; i < n; add_arcs(i,i+n,1), ++i ) ;
		add_arcs(0,0+n,1), maxflow();
	}
	return 0;
}

