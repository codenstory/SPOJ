/*
 * SUBMERGE
 * TOPIC: articulation points -- correct implementation
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<14)
#define MAXE (1<<20)
enum { WHITE, GREY, BLACK };

int min( int x, int y ) { return x<y?x:y; }

int to[MAXE],last[N],next[MAXE],d[N],h[N],is_arti[N],root=0,
	color[N],E,p[N],in[N],out[N],tick,ans,n,m;

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

void dfs( int x, int depth ) {
	int i,y,k=0;
	assert( color[x] == WHITE ) ;
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( color[to[i]] == WHITE && ++k ) ;
	for ( is_arti[x] = 0, in[x]=++tick, h[x]=d[x]=depth, color[x]=GREY, i=last[x]; i>=0; i=next[i] ) {
		switch ( color[y = to[i]] ) {
			case WHITE: p[y] = i, dfs(y,depth+1);
						h[x] = min(h[x],h[y]); 
						if ( x == root && k > 1 && h[y] > d[x] || x != root && h[y] >= d[x] )
							is_arti[x] = 1;
						break ;
			case GREY:  if ( p[x] == -1 || to[p[x]^1] != y ) 
							h[x] = min(h[x],d[y]);
						break ;
			default:    break ;
		}
	}
	color[x] = BLACK, out[x] = ++tick;
}

int main() {
	int i,j,k;
	for ( ;2 == scanf("%d %d",&n,&m) && (n||m); printf("%d\n",ans) ) {
		for ( E = 0, i = 0; i < n; color[i] = WHITE, last[i++] = -1 ) ;
		for ( ;m-- && 2 == scanf("%d %d",&i,&j); add_arcs(--i,--j) ) ;
		for ( tick = p[root] = -1, dfs(root,0), ans = 0, i = 0; i < n; ans += is_arti[i++] ) ;
	}
	return 0;
}

