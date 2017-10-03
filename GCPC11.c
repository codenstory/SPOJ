/*
 * GCPC11C
 * TOPIC: topological sort
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<15)
#define MAXE (1<<18)
enum { D, T };

int m,n,to[2][MAXE],last[2][N],next[2][MAXE],E[2],
	st[N],*top,seen[N],yes,scc[N],mark,
	xx[MAXE/2],yy[MAXE/2],cnt[N],
	q[N],*head,*tail,*ptr,order[N];

void add_arcs( int x, int y ) {
	int i = E[D]++, j = E[T]++;
	to[D][i] = y, next[D][i] = last[D][x], last[D][x] = i;
	to[T][j] = x, next[T][j] = last[T][y], last[T][y] = j;
}

void dfs( int x, int t ) {
	int i,y;
	for ( seen[x] = yes, i = last[t][x]; i >= 0; i = next[t][i] )
		if ( seen[y = to[t][i]] != yes )
			dfs(y,t);
	if ( t ) *++top = x;
	else scc[x] = mark;
}

int main() {
	int i,j,k,l,ts,x,y;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m) && ++yes; ) {
		for ( E[D] = E[T] = 0, i = 0; i < n; cnt[i] = 0, last[D][i] = last[T][i] = -1, ++i ) ;
		for ( l = 0; l < m; ++l ) {
			scanf("%d %d",&i,&j), --i, --j;
			add_arcs(i,j);
			xx[l] = i, yy[l] = j, ++cnt[j];
		}
		for ( top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(i,T);
		for ( ++yes, mark = -1; top > st; --top )
			if ( seen[i = *top] != yes )
				++mark, dfs(i,D);
		for ( i = 0; i < m && (scc[xx[i]] != scc[yy[i]]); ++i ) ;
		if ( i < m ) {
			puts("recheck hints");
			continue ;
		}
		for ( ptr = order, ++yes, head = tail = q, top = st, i = 0; i < n; ++i )
			if ( !cnt[i] ) *++top = i;
		assert( top > st );
		for (;top-st==1;)
			for ( i=last[D][*ptr++ =x=*top--]; top-st <= 1 && i >= 0; i = next[D][i] )
				if ( !--cnt[y=to[D][i]] )
					*++top = y;
		if ( ptr-order != n ) {
			puts("missing hints");
			continue ;
		}
		for ( i = 0; i < ptr-order-1; printf("%d ",order[i++]+1) ) ;
		printf("%d\n",order[i]+1);
	}
	return 0;
}

