#include <bits/stdc++.h>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
enum { S, T };
#include <vector>
#include <map>
#include <set>
#define N 0x400
#define MAXV (2048)
#define oo (1LL<<60)
#define MAXE (1<<21)
typedef long long i64;
using namespace std;

int m,n,B,scan[N],pack[N],visited[MAXV],yes,d[MAXV],seen[MAXV],indeed,
	src,sink,E,to[MAXE],last[MAXE],_next[MAXE],V,
	q[MAXV],*head,*tail;
i64 cap[MAXE],flow[MAXE],mx[N];

void add_arcs( int x, int y, i64 c ) {
	int i= E++, j= E++;
	to[i]= y, _next[i]= last[x], last[x]= i, cap[i]= c, flow[i]= 0;
	to[j]= x, _next[j]= last[y], last[y]= j, cap[j]= 0, flow[j]= 0;
}

void construct( i64 limit ) {
	int i,j;
	for ( E= 0, i= 0; i <= m+n+2; last[i++]= -1 ) ;
	for ( i= 0; i < m; ++i )
		for ( j= 0; j < n; ++j ) {
			i64 k= min((limit-pack[j])/scan[j],mx[j]);
			if ( k > 0 )
				add_arcs(i,j+m,1);
		}
	for ( src= m+n, sink= m+n+1, i= 0; i < m; ++i )
		add_arcs(src,i,B);
	for ( j= 0; j < n; ++j )
		add_arcs(j+m,sink,B);
}

i64 dfs( int x, i64 df ) {
	i64 dt;
	if ( x == sink )
		return df;
	if ( seen[x] == indeed )
		return 0;
	seen[x]= indeed;
	if ( !df ) return 0;
	assert( df > 0 );
	for ( int y, i= last[x]; i != -1; i= _next[i] )
		if ( visited[y= to[i]] == yes && d[y]+1 == d[x] && cap[i] > flow[i] ) {
			if ( dt= dfs(y,min(df,cap[i]-flow[i])) ) {
				flow[i]+= dt, flow[i^1]-= dt;
				return dt;
			}
		}
	return 0;
}


int bfs() {
	int x,y,i,j,k;

	for ( head=tail=q, d[sink]= 0, visited[*tail++= sink]= ++yes; head < tail; )
		for ( x= *head++, i= last[x]; i != -1; i= _next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y=to[i]] != yes ) 
				d[y]= d[x]+1, *tail++= y, visited[y]= yes;
	return visited[src] == yes;
}

int mate[2][N];
int alt_path( int x ) {
	if ( seen[x] == yes )
		return 0;
	int i,y;
	for ( i = last[x]; i != -1; i = _next[i] )
		if ( mate[T][y= to[i]] == -1 || alt_path(mate[T][y]) ) {
			mate[S][x]= y;
			mate[T][y]= x;
			return 1;
		}
	return 1;
}


i64 maxflow() {
	i64 T=0,dt;
	for ( ;bfs(); )
		for ( ;++indeed && (dt= dfs(src,+oo)); T+= dt ) ;
	return T;
}

i64 capacity[N];

int main() {
	int i,j,k,cs= 0,ts;
	i64 good,bad,mid;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&m,&B,&n);
		for ( i= 0; i < n; ++i )
			scanf("%lld %d %d",mx+i,scan+i,pack+i);
		for ( good= +oo, bad= 0; bad+1 != good; ) {
			i64 sum= 0;
			mid= (good+bad)/2;
			for ( j= 0; j < n; ++j )
				if ( (capacity[j]= min((mid-pack[j])/scan[j],mx[j])) < 0 )
					capacity[j]= 0;
			sort(capacity,capacity+n);
			for ( k= 0, i= n-1; i >= 0 && k < m; sum+= capacity[i--], ++k ) ;
			if ( sum >= B ) good= mid;
			else bad= mid;
		}
		printf("Case #%d: %lld\n",++cs,good);
	}
	return 0;
}

