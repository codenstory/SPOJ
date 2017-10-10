/*
 * TOUR
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
#include <set>
#define N 0x400
using namespace std;
enum { D, T };

vector<int> adj[2][N];
set<int> lst[N];
int n,seen[N],yes,*top,st[N],scc[N],mark,size[N];

void dfs( const int t, int x ) {
	assert( seen[x] != yes );
	seen[x] = yes;
	for ( int y, i = 0; i < (int)adj[t][x].size(); ++i )
		if ( seen[y = adj[t][x][i]] != yes )
			dfs(t,y);
	if ( t ) *++top = x;
	else ++size[scc[x] = mark];
}

void construct( int x ) {
	assert( seen[x] != yes );
	seen[x] = yes;
	for ( int y, i = 0; i < (int)adj[D][x].size(); ++i ) {
		if ( seen[y = adj[D][x][i]] != yes ) 
			construct(y);
		if ( scc[y] != scc[x] )
			lst[scc[x]].insert(scc[y]);
	}
}

int main() {
	int t,i,j,k,ts,ans;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( t = D; t <= T; ++t )
			for ( i = 0; i < n; lst[i].clear(), adj[t][i++].clear() ) ;
		for ( i = 0; i < n; ++i ) 
			for ( scanf("%d",&k); k-- && 1 == scanf("%d",&j); --j, adj[D][i].push_back(j), adj[T][j].push_back(i) ) ;
		for ( top = st, ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( mark = -1, ++yes; top > st; )
			if ( seen[i = *top--] != yes )
				++mark, size[mark] = 0, dfs(D,i);
		for ( ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				construct(i);
		for ( ans = 0, i = 0; i <= mark; ++i )
			if ( lst[i].size() == 0 ) {
				if ( ans == 0 )
					ans = size[i];
				else ans = -1;
			}
		printf("%d\n",ans==-1?0:ans);
	}
	return 0;
}

