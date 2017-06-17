/*
 * SHOP2
 * TOPIC: mincost maxflow
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N (2048*2)
#define oo (1LL<<60)
#define MAXV (N+N)
#define MAXE (1<<21)
#define xchg(x,y) (((x)==(y)) || ((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
typedef long long i64;
using namespace std;

int n,m,price[N],cash_value[N],dummy,
	V,E,p[MAXV],src,sink,last[MAXV],
	to[MAXE],next[MAXE],visited[MAXV],yes,seen[MAXV],indeed,q[MAXV],*head,*tail;
i64 flow[MAXE],d[MAXV],cap[MAXE];
vector<int> lst[N];

void add_arcs( int x, int y, i64 c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q,d[*tail++=sink] = 0, visited[sink]=++yes; head < tail;)
		for ( i = last[x=*head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes )
				visited[*tail++ = y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

i64 dfs( int x, i64 df ) {
	i64 dt;
	int i,y;
	if ( x == sink )
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y=to[i]] == yes && d[y]+1 == d[x] )
			if ( dt = dfs(y,min(df,cap[i]-flow[i])) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

i64 maxflow() {
	i64 T = 0,dt;
	for (;bfs();)
		for ( ;++indeed && (dt = dfs(src,+oo)); T += dt );
	return T;
}

int main() {
	int i,j,k,ts;
	i64 Total;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( Total = 0, i = 0; i < n; scanf("%d",&price[i++]), Total += price[i-1] ) ;
		for ( j = 0; j < m; scanf("%d",&cash_value[j++]) ) ;
		for ( j = 0; j < m; ++j )
			for ( scanf("%d",&k), lst[j].clear(); k-- && 1 == scanf("%d",&i); lst[j].push_back(--i) ) ;
		for ( V = n+m+2, src = V-2, sink = V-1, E = 0, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < n; add_arcs(src,i,price[i]), ++i ) ;
		for ( j = 0; j < m; add_arcs(j+n,sink,cash_value[j]), ++j ) ;
		for ( j = 0; j < m; ++j )
			for ( k = 0; k < (int)lst[j].size(); ++k )
				i = lst[j][k], add_arcs(i,j+n,price[i]);
		printf("%lld\n",Total-maxflow());
	}
	return 0;
}

