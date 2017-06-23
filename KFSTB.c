/*
 * KFSTB
 * TOPIC: topological sort, DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<14)
enum { D, T };
#define MAXV (N)
#define MAXE (2*N)
#define MAXN (N)
#define MOD 1000000007LL
typedef long long i64;

int m,n,E[2],to[2][MAXE],next[2][MAXE],last[2][MAXV],src,sink,
	st[MAXN],*top,seen[MAXN],yes,pos[MAXN],v[MAXN];
i64 z[N];

void add_arcs( int x, int y ) {
	int i = E[D]++, j = E[T]++;
	to[D][i] = y, next[D][i] = last[D][x], last[D][x] = i;
	to[T][j] = x, next[T][j] = last[T][y], last[T][y] = j;
}

void dfs( int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = last[t][x]; i >= 0; i = next[t][i] ) 
		if ( seen[y = to[t][i]] != yes )
			dfs(t,y);
	*++top = x;
}

int main() {
	int i,j,k,ts,t,x,y;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d %d",&n,&m,&src,&sink), --src, --sink;
		for ( t = D; t <= T; ++t )
			for ( E[t] = 0, i = 0; i < n; last[t][i++] = -1 ) ;
		for ( ;m-- && 2 == scanf("%d %d",&i,&j); --i, --j, add_arcs(i,j) ) ;
		for ( ++yes, top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(D,i);
		for ( m = 0; top > st; pos[i=*top--]=m, v[m++]=i ) ;
		memset(z,0,sizeof z);
		for ( i = pos[src], z[src] = 1; ++i < n; ) {
			for ( x = v[i], j = last[T][x]; j >= 0; j = next[T][j] ) {
				y = to[T][j];
				assert( pos[y] < i );
				z[x] += z[y], z[x] %= MOD;
			}
		}
		printf("%lld\n",z[sink]);
	}
	return 0;
}

