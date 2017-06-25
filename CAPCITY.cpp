/*
 * CAPCITY
 * TOPIC: scc, strongly-connected components, sinks and sources
 * status: Accepted
 */
#include <algorithm>
#include <vector>
#include <set>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#define N (1<<17)
#define MAXE (1<<20)
using namespace std;
enum { D, T };

int m,n,E[2],to[2][MAXE],next[2][MAXE],last[2][N],
	scc[N],mark,st[N],*top,yes,seen[N],outdeg[N];
vector<int> component[N];
set<int> adj[N],radj[N];

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
	if ( t ) *++top = x;
	else
		component[scc[x]=mark].push_back(x);
}

void floodfill( int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = last[D][x]; i >= 0; i = next[D][i] )  {
		if ( seen[y = to[D][i]] != yes ) 
			floodfill(y);
		if ( scc[x] != scc[y] )
			adj[scc[x]].insert(scc[y]), ++outdeg[scc[x]], radj[scc[y]].insert(scc[x]);
	}
}

void topological_sort( int x ) {
	int y;
	assert( seen[x] != yes );
	for ( set<int> :: iterator it = adj[x].begin(); it != adj[x].end(); ++it )
		if ( seen[y = *it] != yes )
			topological_sort(y);
	*++top = x;
}

int get_sink() {
	int k = 0, x = -1, i;
	for ( i = 0; i <= mark; ++i )
		if ( !outdeg[i] ) {
			if ( ++k >= 2 ) 
				return -1;
			x = i;
		}
	return x;
}

queue<int> q;
bool is_connected( int src ) {
	int x,y;
	for ( q.push(src), seen[src] = ++yes; !q.empty(); q.pop() )
		for ( set<int> :: iterator it = radj[x = q.front()].begin(); it != radj[x].end(); ++it )
			if ( seen[y = *it] != yes )
				q.push(y), seen[y] = yes;
	for ( x = 0; x <= mark; ++x )
		if ( seen[x] != yes )
			return false ;
	return true ;
}

int main() {
	int i,j,k,t,sink;
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		for ( t = D; t <= T; ++t )
			for ( E[t] = 0, i = 0; i < n; last[t][i++] = -1 ) ;
		for ( ;m-- && 2 == scanf("%d %d",&i,&j); add_arcs(--i,--j) ) ;
		for ( top = st, ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( mark = -1, ++yes; top > st; )
			if ( seen[i = *top--] != yes )
				component[++mark].clear(),radj[mark].clear(),adj[mark].clear(),outdeg[mark]=0,dfs(D,i);
		for ( ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				floodfill(i);
		if ( (sink = get_sink()) == -1 || !is_connected(sink) ) {
			puts("0");
			continue ;
		}
		set<int> s;
		for ( i = 0; i < (int)component[sink].size(); s.insert(component[sink][i++]) ) ;
		printf("%d\n",s.size()), k = 0;
		for ( set<int> :: iterator it = s.begin(); it != s.end(); ++it ) {
			if ( ++k > 1 ) putchar(' ');
			printf("%d",*it+1);
		}
		putchar('\n');
	}
	return 0;
}

