/*
 * LASTSHOT
 * TOPIC: DFS, scc
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
#define N (1<<16)
#define oo (1<<30)
enum { D, T };
using namespace std;

int n,m,scc[N],mark,st[N],*top,seen[N],yes,size[N],z[N],mcount[N],
	*head,*tail,q[N];
vector<int> adj[2][N];
set<int> lst[N];

void dfs( const int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = 0; i < (int)adj[t][x].size(); ++i )
		if ( seen[y = adj[t][x][i]] != yes )
			dfs(t,y);
	if ( t ) *++top = x;
	else ++size[scc[x] = mark];
}

void construct( int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = 0; i < (int)adj[D][x].size(); ++i ) {
		if ( seen[y = adj[D][x][i]] != yes )
			construct(y);
		if ( scc[x] != scc[y] )
			lst[scc[x]].insert(scc[y]);
	}
}

void floodfill( int x ) {
	if ( seen[x] == yes ) return ;
	seen[x] = yes;
	/*
	for ( set<int> :: iterator it = lst[x].begin(); it != lst[x].end(); floodfill(*it++) ) ;
	*/
	for ( int y, i = 0; i < (int)adj[D][x].size(); ++i )
		floodfill(adj[D][x][i]);
}

int main() {
	int i,j,k,t;
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		for ( i = 0; i < n; ++i )
			for ( lst[i].clear(), t = D; t <= T; adj[t++][i].clear() ) ;
		for ( k = 0; k < m; ++k ) {
			scanf("%d %d",&i,&j);
		    if ( i != j )
			  --i, --j, adj[D][i].push_back(j), adj[T][j].push_back(i);
		}
		for ( ++yes, top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( mark = -1, ++yes; top > st; )
			if ( seen[i = *top--] != yes )
				size[++mark] = 0, z[mark] = +oo, dfs(D,i);
		for ( ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				construct(i);
		for ( i = 0; i <= mark; mcount[i] = lst[i].size(), ++i ) ;
		for ( k = 0, i = 0; i < n; ++i ) {
			++yes, floodfill(i);
			for ( t = 0, j = 0; j < n; ++j )
				if ( seen[j] == yes )
					//t += size[j];
					++t;
			k = max(k,t);
		}
		printf("%d\n",k);
		break ;
	}
	return 0;
}

