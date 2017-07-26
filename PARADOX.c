/*
 * PARADOX
 * TOPIC: dfs, scc, implication graph
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x100
#define MAXE (0x400)
#define enc(t,x) ((t)|((x)<<1))
enum { WHITE, GREY, BLACK };
enum { S, T };
enum { FALSE, TRUE };

int n,val[N],arg[N],st[N],*top,seen[N],yes,scc[N],mark,
	color[N],to[2][MAXE],next[2][MAXE],last[2][N],E[2];

void add_arcs( int x, int y ) {
	int i = E[S]++, j = E[T]++;
	to[S][i] = y, next[S][i] = last[S][x], last[S][x] = i;
	to[T][j] = x, next[T][j] = last[T][y], last[T][y] = j;
}

void dfs( const int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = last[t][x]; i >= 0; i = next[t][i] )
		if ( seen[y = to[t][i]] != yes )
			dfs(t,y);
	if ( t ) *++top = x;
	else scc[x] = mark;
}

int main() {
	int i,j,k,t;
	char tmp[0x10];
	for ( ;1 == scanf("%d",&n) && n; puts(!k?"PARADOX":"NOT PARADOX") ) {
		for ( i = 0; i < n; ++i ) {
			scanf("%d %s",&j,tmp), arg[i]=(j-1), val[i]=0[tmp]=='t';
			assert( 0 <= arg[i] && arg[i] < n );
		}
		for ( t = 0; t <= 1; ++t )
			for ( E[t] = 0, i = 0; i < n; last[t][enc(TRUE,i)] = last[t][enc(FALSE,i)] = -1, ++i ) ;
		for ( i = 0; i < n; j = arg[i], k = val[i]?TRUE:FALSE, add_arcs(enc(TRUE,i),enc(k,j)), add_arcs(enc(FALSE,i),enc(k^1,j)), ++i ) ;
		for ( ++yes, top = st, i = 0; i < n; ++i )
			for ( t = 0; t <= 1; ++t )
				if ( seen[k=enc(t,i)] != yes )
					dfs(T,k);
		for ( mark = -1, ++yes; top > st; )
			if ( seen[i = *top--] != yes )
				++mark, dfs(S,i);
		for ( k = 1, i = 0; i < n && k; ++i )
			if ( scc[enc(FALSE,i)] == scc[enc(TRUE,i)] && (k = 0) ) ;
	}
	return 0;
}

