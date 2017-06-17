/*
 * Greedy Island
 * GREED
 * status: Accepted
 * TOPIC:mincost maxflow, weighted matching
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXV (1<<21)
#define MAXE (1<<21)
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))

int n,last[MAXV],next[MAXE],to[MAXE],cap[MAXE],cost[MAXE],src,sink,V,E,flow[MAXE],m,
	cards[MAXV],heap[MAXV<<1],pos[MAXV],cnt,d[MAXV],pi[MAXV],p[MAXV],cc[MAXV];

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[heap[cnt] = x] = cnt, ++cnt;
	for ( j = pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cnt += (pos[x]=-1) )
		pos[*heap = heap[cnt]] = 0;
	for ( j=0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

void add_arcs( int x, int y, int c, int w ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, cost[i] =  w;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, cost[j] = -w;
	flow[i] = flow[j] = 0;
}

int dijkstra() {
	int x,y,i;
	for ( cnt = 0, x = 0; x < V; pos[x] = -1, d[x++] = +oo );
	for ( d[src] = 0, push(src); cnt; )
		for ( i = last[x = pop()]; i >= 0; i = next[i] )
			if ( cap[i] > flow[i] && d[y=to[i]] > d[x]+pi[x]-pi[y=to[i]]+cost[i]  )
				d[y] = d[x]+pi[x]-pi[y]+cost[i], push(y), p[y] = i;
	for ( x = 0; x < V; ++x )
		if ( d[x] < +oo )
			pi[x] += d[x];
	return d[sink] < +oo;
}

int mincost_maxflow() {
	int x,i,w = 0,dt,T = 0;
	for ( x = 0; x < V; pi[x++] = 0 ) ;
	for (;dijkstra(); T += dt ) {
		for ( dt=+oo, i=p[sink], x=to[i^1], dt=min(dt,cap[i]-flow[i]); x!=src; i=p[x],x=to[i^1],dt=min(dt,cap[i]-flow[i]) ); 
		for ( i=p[sink],x=to[i^1], flow[i]+=dt,flow[i^1]-=dt; x != src; i=p[x],x=to[i^1],flow[i]+=dt,flow[i^1]-=dt );
	}
	assert( T == n );
	for ( i = 0; i < E; ++i )
		if ( flow[i] > 0 )
			w += cost[i]*flow[i];
	return w;
}

int main() {
	int i,j,k,t,ts;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d",&n), memset(cc,0,sizeof cc), i = 0; i < n; scanf("%d",&cards[i++]), ++cc[--cards[i-1]] );
		for ( E = 0, V = n+n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( scanf("%d",&m), k = 0; k < m; ++k ) {
			scanf("%d %d",&i,&j), --i, --j;
			if ( i == j ) continue ;
			add_arcs(i,j+n,+oo,1), add_arcs(j,i+n,+oo,1);
		}
		for ( i = 0; i < n; ++i )
			add_arcs(i+n,i,+oo,0), add_arcs(i,i+n,1,0);
		for ( i = 0; i < n; ++i )
			add_arcs(src,i,cc[i],0), add_arcs(i+n,sink,1,0);
		printf("%d\n",mincost_maxflow());
	}
	return 0;
}

