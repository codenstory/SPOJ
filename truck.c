/*
 * C. Big Truck
 * TOPIC: Dijkstra with binary heap
 * status: Accepted
 * LINK: https://open.kattis.com/problems/bigtruck
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
#define N (0x100)
#define MAXE (1<<21)
#define NONE (-1)
#define oo (1<<30)
#pragma comment(linker, "/STACK:16777216")
enum { SRC, DST };
#ifndef max
int max( int x, int y ) { return x<y?y:x; }
#endif

int V,E,n,to[MAXE],next[MAXE],last[N],weight[MAXE],
	d[2][N],pos[N],heap[N<<1],cnt,
	deg[N],adj[N][N],value[N],z[N];

void add_arcs( int x, int y, int w ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, weight[i] = w;
	to[j] = x, next[j] = last[y], last[y] = j, weight[j] = w;
}

void push( int *d, int x ) {
	int i,j;
	if ( pos[x] == NONE )
		pos[heap[cnt] = x] = cnt, ++cnt;
	for ( j = pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

int pop( int *d ) {
	int i,j,x = *heap;
	if ( cnt += (pos[x]=NONE) ) pos[*heap=heap[cnt]]=0;
	for ( j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

int dijkstra( int *d, int src, int dst ) {
	int x,y,i;
	for ( x = 0; x < n; pos[x] = NONE, d[x++] = +oo ) ;
	for ( cnt = 0, d[src] = 0, push(d,src); cnt; )
		for ( i = last[x = pop(d)]; i != NONE; i = next[i] ) 
			if ( d[y = to[i]] > d[x] + weight[i] )
				d[y] = d[x] + weight[i], push(d,y);
	return d[dst];
}

int calc_z( int x ) {
	int i;
	if ( z[x] < +oo )
		return z[x];
	for ( z[x] = 0, i = 0; i < deg[x]; z[x]=max(value[x]+calc_z(adj[x][i++]),z[x]) ) ;
	return z[x];
}

int main() {
	int i,j,k,m,x,y,src,dest;
	for ( ;1 == scanf("%d",&n); ) {
		for ( src=0,dest=n-1, E = 0, i = 0; i < n; z[i] = +oo, deg[i] = 0, last[i++] = NONE ) ;
		for ( i = 0; i < n; scanf("%d",&value[i++]) ) ;
		for ( scanf("%d",&m); m-- && 3 == scanf("%d %d %d",&i,&j,&k); add_arcs(--i,--j,k) ) ;
		if ( (m = dijkstra(d[SRC],src,dest)) < +oo ) {
			k = dijkstra(d[DST],dest,src);
			assert( k == m );
			for ( i = 0; i < E; ++i ) 
				if ( d[SRC][x=to[i^1]] < +oo && d[DST][y=to[i]] < +oo && d[SRC][x]+weight[i]+d[DST][y] == m )
					adj[x][deg[x]++] = y;
			z[dest] = value[dest], printf("%d %d\n",m,calc_z(src));
		}
		else puts("impossible");
	}
	return 0;
}

