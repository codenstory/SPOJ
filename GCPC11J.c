/*
 * GCPC11J
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<18)
#define MAXE (N<<1)

int E,to[MAXE],last[N],next[MAXE],n,seen[N],yes,d[N],
	q[N],*head,*tail;

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

int bfs( int src, int *furthest ) {
	int x,y,i,D = -1;
	for ( seen[src] = ++yes, head = tail = q, d[*tail++ = src] = 0; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( seen[y = to[i]] != yes ) {
				d[y] = d[x]+1, seen[*tail++ = y] = yes;
				if ( d[y] > D ) D = d[*furthest = y];
			}
	return D;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( E = 0, i = 0; i < n; last[i++] = -1 ) ;
		for ( k = 0; k < n-1 && 2 == scanf("%d %d",&i,&j); add_arcs(i,j), ++k ) ;
		bfs(0,&i), k = bfs(i,&j);
		printf("%d\n",(k&1)?(k+1)/2:k/2);
	}
	return 0;
}

