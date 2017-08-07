/*
 * AKBAR
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define MAXE (N<<1)
#define NONE (-1)
typedef long long i64;
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)

int n,m,E,to[MAXE],last[N],next[MAXE],protected_by[N],hh,tt,dist,yes,
	pos[N],strength[N],ok;
i64 q[N],*head,*tail;

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

void bfs( int src, int limit ) {
	int x,y,i,k;
	i64 u,v;
	if ( protected_by[src] != NONE ) {
		ok = 0;
		return ;
	}
	for ( head=tail=q, protected_by[src] = src, *tail++ = src|(((i64)limit)<<21), hh = 0, tt = 1, dist = 0; head < tail; ) {
		u = *head++;
		if ( hh++ == tt )
			hh = head-q, tt = tail-q, ++dist;
		if ( dist == limit ) continue ;
		x = (u&MASK(21)), k = (u>>21);
		if ( !k ) continue ;
		for ( i = last[x]; i >= 0; i = next[i] ) {
			if ( protected_by[y = to[i]] == NONE )
				protected_by[y] = src, *tail++ = (y|((i64)(k-1))<<21);
			else if ( protected_by[y] != src ) {
				ok = 0;
				return ;
			}
		}
	}
}

int main() {
	int i,j,k,ts,r;
	for ( scanf("%d",&ts); ts-- && 3 == scanf("%d %d %d",&n,&r,&m); ) {
		for ( E = 0, i = 0; i < n; protected_by[i] = NONE, last[i++] = -1 ) ;
		for ( ;r-- && 2 == scanf("%d %d",&i,&j); add_arcs(--i,--j) ) ;
		for ( ok = 1, i = 0; i < m; ++i ) {
			scanf("%d %d",&pos[i],&strength[i]);
			if ( !ok ) continue ;
			bfs(pos[i]-1,strength[i]);
		}
		if ( !ok ) {
			puts("No");
			continue ;
		}
		for ( i = 0; i < n && ok; ++i )
			if ( protected_by[i] == NONE )
				ok = 0;
		puts(ok?"Yes":"No");
	}
	return 0;
}

