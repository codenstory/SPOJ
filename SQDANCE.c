/*
 * SQDANCE
 * TOPIC: DSU, union-find, easy
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)

int n,m,rank[N],parent[N];

void link( int x, int y ) {
	if ( rank[x] > rank[y] )
		parent[y] = x;
	else {
		parent[x] = y;
		if ( rank[x] == rank[y] ) ++rank[y];
	}
}

int find( int x ) {
	return parent[x] = (parent[x] == x)?x:find(parent[x]);
}

void merge( int x, int y ) {
	link(find(x),find(y));
}

int main() {
	int i,j,k,ts,ans,x,y;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( x = 0; x < n; rank[parent[x] = x] = 0, ++x ) ;
		for ( ans = 0; m-- && 2 == scanf("%d %d",&x,&y); ) {
			if ( find(--x) == find(--y) ) {
				++ans;
				continue ;
			}
			merge(x,y);
		}
		printf("%d\n",ans);
	}
	return 0;
}

