/*
 * EC_P
 * TOPIC: bridges in a graph
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x400)
#define SH 10
#define MAXE (1<<19)
#define MASK(k) ((1ULL<<(k))-1ULL)
typedef long long i64;
enum { WHITE, GREY, BLACK };

int min( int x, int y ) { return x<y?x:y; }
int max( int x, int y ) { return x<y?y:x; }

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( *x == *y )
		return 0;
	return *x<*y?-1:1;
}

int E,m,n,to[MAXE],next[MAXE],last[N],d[N],h[N],p[N],color[N];
i64 *ptr,e[MAXE];

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

void dfs( int x, int depth ) {
	int i,y;
	assert( color[x] == WHITE ) ;
	for ( color[x]=GREY, h[x]=d[x]=depth, i=last[x]; i>=0; i=next[i] ) 
		if ( color[y=to[i]]==WHITE ) 
			p[y]=i, dfs(y,depth+1), h[x]=min(h[x],h[y]);
		else if ( color[y]==GREY && (p[x]==-1||to[p[x]^1]!=y) )
			h[x]=min(h[x],d[y]);
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( p[y = to[i]] == i ) 
			if ( h[y] > d[x] )
				*ptr++ = (max(x+1,y+1)|(min(x+1,y+1)<<SH));
	color[x] = BLACK;
}

int main() {
	int i,j,k,cs = 0,ts;
	i64 u;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( E = 0, i = 0; i < n; color[i] = WHITE, last[i++] = -1 ) ;
		for ( ;m-- && 2==scanf("%d %d",&i,&j); add_arcs(--i,--j) ) ;
		printf("Caso #%d\n",++cs), ptr=e, p[0]=-1, dfs(0,0);
		if ( ptr == e ) {
			puts("Sin bloqueos");
			continue ;
		}
		printf("%d\n",ptr-e), qsort(e,ptr-e,sizeof *e,cmp);
		for ( u = -1, i = 0; i < ptr-e; u = e[i++] ) {
			printf("%lld %lld\n",e[i]>>SH,e[i]&MASK(SH));
			assert( e[i] != u ) ;
		}
	}
	return 0;
}

