/*
 * BOTTOM: The Bottom of a Graph
 * TOPIC: scc
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#define N 5008
enum { S, T };
using namespace std;

int n,m,seen[N],yes,st[N],*top,mark,scc[N],
	*head,*tail,q[N];
vector<int> adj[2][N],lst[N];
set<int> cc[N];

void dfs( const int t, int x ) {
	int i,y;
	if ( seen[x] == yes ) return ;
	for ( seen[x] = yes, i = 0; i < (int)adj[t][x].size(); ++i )
		if ( seen[y = adj[t][x][i]] != yes )
			dfs(t,y);
	if ( t == T ) *++top = x;
	else cc[scc[x] = mark].insert(x);
}

int main() {
	int i,j,k,t,x,y,z;
	for ( ;1 == scanf("%d",&n) && n; ) {
		for ( t = S; t <= T; ++t )
			for ( i = 0; i < n; adj[t][i++].clear() ) ;
		for ( scanf("%d",&m); m-- && 2 == scanf("%d %d",&i,&j);\
		--i, --j, adj[S][i].push_back(j), adj[T][j].push_back(i) ) ;
		for ( ++yes, top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( ++yes, mark = -1; top > st; )
			if ( seen[i = *top--] != yes )
				lst[++mark].clear(), cc[mark].clear(), dfs(S,i);
		for ( ++yes, z = 0; z < n; ++z )
			if ( seen[z] != yes ) 
				for ( head=tail=q, seen[*tail++=z]=yes; head<tail; )
					for ( x = *head++, i = 0; i < (int)adj[S][x].size(); ++i ) {
						if ( seen[y = adj[S][x][i]] != yes )
							seen[*tail++ = y] = yes;
						if ( scc[y] != scc[x] )
							lst[scc[x]].push_back(scc[y]);
				}
		set<int> s;
		for ( s.clear(), i = 0; i <= mark; ++i )
			if ( !lst[i].size() )
				for ( set<int>::iterator it = cc[i].begin(); it != cc[i].end(); s.insert(*it++) ) ;
		if ( !s.size() ) {
			puts("");
			continue ;
		}
		k = 0;
		for ( set<int>::iterator it = s.begin(); it != s.end(); printf("%d",1+(*it++)) )
			if ( ++k > 1 ) putchar(' ');
		putchar('\n');
	}
	return 0;
}

