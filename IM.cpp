/*
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#define MAXV (1<<15)
#define MAXE (1<<21)
using namespace std;

int last[MAXV], next[MAXE], flow[MAXE], cap[MAXE], V, E, d[MAXV], visited[MAXV], yes;



void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0;
	flow[i] = flow[j] = 0;
}

int bellman_ford() {
	int i,j,k,x,y;
}

int main() {
	int i,j,k,cs;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( k = 0; k < m; ++k ) {
			scanf("%d %d",&i,&j), --i, --j;
			add_arcs(i+n,j,1,ZERO), add_arcs(j+n,i,1,ZERO);
		}
		for ( i = 0; i < n; ++i )
			add_arcs(i,i+n,1,ZERO);
		add_arcs(0+n,2,1,ZERO), add_arcs(2+n,0,1,ZERO);
	}
	return 0;
}

