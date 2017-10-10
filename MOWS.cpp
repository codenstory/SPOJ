/*
 * MOWS
 * TOPIC: scc
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N (1<<21)
using namespace std;
enum { D, T };

vector<int> adj[2][N];
int m,n,seen[N],yes,st[N],*top,scc[N],card[N],mark,p,V,weight[N];

void dfs( int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = 0; i < (int)adj[t][x].size(); ++i )
		if ( seen[y = adj[t][x][i]] != yes )
			dfs(t,y);
	if ( t ) *++top = x;
	else ++card[scc[x] = mark];
}

int main() {
	int i,j,k,t,ts,best;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&n,&p,&m);
		for ( V = n+p, i = 0; i < V; ++i )
			for ( t = D; t <= T; adj[t++][i].clear() ) ;
		for ( ;m-- && 2 == scanf("%d %d",&i,&j); ) {
			if ( i >= 1001 )
				i = n+(i-1001);
			else if ( i <= n )
				--i;
			if ( j >= 1001 )
				j = n+(j-1001);
			else if ( j <= n )
				--j;
			adj[D][i].push_back(j), adj[T][j].push_back(i);
		}
		for ( ++yes, top = st, i = 0; i <V; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( ++yes, mark = -1; top > st; )
			if ( seen[i = *top--] != yes )
				card[++mark] = 0, weight[mark] = 0, dfs(D,i);
		for ( i = n; i < V; ++weight[scc[i++]] ) ;
		for ( best = -1, i = 0; i < n; ++i )
			if ( weight[scc[i]] > best )
				best = weight[scc[k=i]];
		printf("%d %d\n",k+1,best);
	}
	return 0;
}

