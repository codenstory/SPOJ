/*
 * RELINETS: Reliable Nets
 * TOPIC: dsu, brute-force
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x10
#define M 21
#define L(k) ((k)&((~(k))+1UL))
#define oo (1<<30)

int n,m,u[M],v[M],c[M],cost[1<<M],rank[N],p[N],bts[1<<21],covers[1<<21],
	which[1<<21],best;

void link( int x, int y ) {
	if ( rank[x] > rank[y] )
		p[y] = x;
	else {
		p[x] = y;
		if ( rank[x] == rank[y] )
			++rank[y];
	}
}

int find( int x ) {
	return p[x]=(x==p[x]?x:find(p[x]));
}

void merge( int x, int y ) {
	link(find(x),find(y));
}

int main() {
	int i,j,k,l,t,cs = 0,ts,ok;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( k = 0; k < (1<<21); bts[k] = bts[k>>1]+(k&1), ++k ) ;
	while ( 2 == scanf("%d %d",&n,&m) && (n||m) ) {
		for ( k = 0; k < m; ++k )
			scanf("%d %d %d",u+k,v+k,c+k),--u[k],--v[k];
		for ( cost[0]=covers[0]=0, k = 1; k < (1<<m); ++k ) {
			cost[k] = cost[k&~L(k)]+c[i=which[L(k)]];
			covers[k] = covers[k&~L(k)]|(1<<u[i])|(1<<v[i]);
		}
		for ( best = +oo, k = 0; k < (1<<m); ++k ) {
			if ( best <= cost[k] || bts[k] <= n-1 || covers[k] != (1<<n)-1 ) 
				continue ;
			for ( ok = 1, t = k; t && ok; ok = ok&&!(l&(l-1)), t &= ~L(t) ) {
				for ( i = 0; i < n; rank[p[i] = i] = 0, ++i ) ;
				for ( l = k&~L(t); l; l &= ~L(l) ) {
					i = which[L(l)];
					if ( find(u[i]) != find(v[i]) )
						merge(u[i],v[i]);
				}
				for ( l = 0, i = 0; !(l&(l-1)) && i < n; l |= (1<<find(i++)) ) ;
			}
			if ( ok ) best = cost[k];
		}
		if ( best < +oo )
			printf("The minimal cost for test case %d is %d.\n",++cs,best);
		else printf("There is no reliable net possible for test case %d.\n",++cs);
	}
	return 0;
}

