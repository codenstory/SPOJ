/*
 * ADATRIP
 * TOPIC: optimized Dijkstra,Dial's Algorithm
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
#include <queue>
#define tol (1e-13)
#define N (5*(100000))
#define MAXE (1<<21)
#define W 10
#define oo 0xfffffffful
using namespace std;

int to[MAXE],next[MAXE],last[N],E,m,n,V,cur,seen[N],yes,MAXW;
unsigned int d[N];
char w[MAXE];
queue<int> q[N*W+1];

void add_arcs( int x, int y, int weight ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, w[i] = weight;
	to[j] = x, next[j] = last[y], last[y] = j, w[j] = weight;
	if ( MAXW < weight ) MAXW = weight;
}

void dijkstra( int src ) {
	int i,j,k,x,y;
	unsigned int weight;
	for ( seen[src]=++yes, q[cur=d[src]=0].push(src); cur <= MAXW*(n-1); ) {
		for ( ;cur <= MAXW*(n-1) && q[cur].empty(); ++cur ) ;
		if ( cur > MAXW*(n-1) ) break ;
		for ( x = q[cur].front(), q[cur].pop(), i = last[x]; i >= 0; i = next[i] )
			if ( seen[y = to[i]] != yes || d[y] > d[x]+w[i] ) 
				q[d[y] = d[x]+w[i]].push(y), seen[y] = yes;
	}
	for ( weight = 0, y = 0; y < n; ++y )
		if ( seen[y] == yes && d[y] > weight )
			weight = d[y];
	for ( k = 0, y = 0; y < n; ++y )
		if ( seen[y] == yes && d[y] == weight && ++k ) ;
	printf("%u %d\n",weight,k);
}

int main() {
	int i,j,k,qr;
	for ( ;3 == scanf("%d %d %d",&n,&m,&qr); ) {
		for ( MAXW = -1, cur = 0, E = 0, i = 0; i < n; last[i++] = -1 ) ;
		for (;m-- && 3 == scanf("%d %d %d",&i,&j,&k); add_arcs(i,j,k) ) ;
		for ( ;qr-- && 1 == scanf("%d",&i); dijkstra(i) ) ;
	}
	return 0;
}

