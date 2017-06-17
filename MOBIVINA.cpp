/*
 * MOBIVINA
 * TOPIC: network flow, similar to COCONUTS
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#define N 0x100
#define MAXV (N)
#define MAXE ((N+N+N*N)*2)
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))
using namespace std;

int adj[N][N],deg[N];
int n,visited[MAXV],yes,q[MAXV],*head,*tail,
	V,src,sink,isdig[256],cap[MAXV][MAXV],p[MAXV];

int augment() {
	int i,j,k,x,y,df = +oo;
	for ( head=tail=q, visited[*tail++=src]=++yes; head<tail && visited[sink] != yes; )
		for ( x=*head++, i = 0; visited[sink] != yes && i < deg[x]; ++i ) 
			if ( cap[x][y=adj[x][i]] > 0 && visited[y] != yes )
				visited[*tail++=y] = yes, p[y] = x;
	if ( visited[sink] != yes ) return 0;
	for ( x=p[sink], df=cap[x][sink]; x!=src; y = x, x = p[y],df=min(df,cap[x][y]) );
	assert(df > 0);
	for ( x=p[y=sink],cap[x][y]-=df,cap[y][x]+=df; x!=src; y=x,x=p[y],cap[x][y]-=df,cap[y][x]+=df );
	return df;
}

int maxflow() {
	int T = 0, dt;
	for(;(dt=augment());T+=dt) ;
	return T;
}

#define add_arcs( x, y, c ) (adj[x][deg[x]++]=y, cap[x][y] = c)

int main() {
	int i,j,k;
	scanf("%d",&n);
	V = n+2, src = V-2, sink = V-1;
	for ( i = 0; i < n; ++i )
		scanf("%d",&k), add_arcs(src,i,k);
	for ( i = 0; i < n; ++i )
		scanf("%d",&k), add_arcs(i,sink,k);
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			scanf("%d",&k), add_arcs(i,j,k);
	printf("%d\n",maxflow());
	return 0;
}

