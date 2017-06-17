/*
 * ASSIGN4: Another Assignment Problem
 * TOPIC: mincost maxflow
 * status: TLE
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x100
#define MAXV (0x400)
#define MAXE (1<<21)
#define oo (1LL<<60)
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
#define min(x,y) ((x)<(y)?(x):(y))
typedef long long i64;

i64 d[MAXV],pi[MAXV],C[N][N],a[N],b[N],flow[MAXE];
int E,V,src,sink,last[MAXV],next[MAXE],to[MAXE],cost[MAXE],cap[MAXE],m,n,
	heap[MAXV<<1],pos[MAXV],cnt,p[MAXV],
	inq[MAXV],indeed;

void add_arcs( int x, int y, int c, int w ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, cost[i] =  w, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, cost[j] = -w, flow[j] = 0;
}

void push( int x ) {
	int i,j;
	if ( inq[x] != indeed )
		pos[heap[cnt] = x] = cnt, ++cnt, inq[x] = indeed;
	for ( j = pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cnt += (pos[x]=-1) ) 
		pos[*heap=heap[cnt]] = 0;
	for ( inq[x] = 0, j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

int dijkstra() {
	int x,y,i;
	for ( x = 0; x < V; d[x++] = +oo ) ;
	for ( d[src] = 0, ++indeed, cnt = 0, push(src); cnt; )
		for ( i = last[x = pop()]; i >= 0; i = next[i] )
			if ( cap[i] > flow[i] && d[y=to[i]] > d[x]+pi[x]-pi[y=to[i]]+cost[i] ) 
				d[y] = d[x]+pi[x]-pi[y]+cost[p[y] = i], push(y);
	for ( x = 0; x < V; ++x )
		if ( d[x] < +oo ) 
			pi[x] += d[x];
	return d[sink] < +oo;
}

void maxflow() {
	int i,j,x,y;
	i64 df;
	for ( x = 0; x < V; pi[x++] = 0 ) ;
	for ( ;dijkstra(); ) {
		for ( i=p[sink],x=to[i^1],df=cap[i]-flow[i]; x != src; i=p[x],x=to[i^1],df=min(df,cap[i]-flow[i]) ) ;
		assert( df > 0 );
		for ( i=p[sink],flow[i]+=df,flow[i^1]-=df,x=to[i^1]; x != src; i=p[x],x=to[i^1],flow[i]+=df,flow[i^1]-=df ) ;
	}
}

i64 mincost_maxflow() {
	i64 T = 0,i;
	for ( maxflow(), i = 0; i < E; ++i )
		if ( flow[i] > 0 )
			T += flow[i]*cost[i];
	return T;
}

int dig[256];
int getnum() {
	int n = 0,ch;
	for ( ;(ch = getchar_unlocked()) != EOF && !dig[ch]; );
	for ( n=ch-'0'; (ch = getchar_unlocked()) != EOF && dig[ch]; n = 10*n+ch-'0' );
	return n;
}

int main() {
	int i,j,k,ts;
	for ( i = '0'; i <= '9'; dig[i++] = 1 ) ;
	for ( ts = getnum(); ts--; ) {
		m = getnum(), n = getnum();
		for ( i = 0; i < m; ++i )
			a[i] = getnum();
		for ( j = 0; j < n; ++j )
			b[j] = getnum();
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				C[i][j] = getnum();
		for ( V = m+n+2, E = 0, i = 0; i < V; last[i++] = -1 ) ;
		for ( src = V-2, sink = V-1, i = 0; i < m; ++i )
			add_arcs(src,i,a[i],0);
		for ( j = 0; j < n; ++j )
			add_arcs(j+m,sink,b[j],0);
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				add_arcs(i,j+m,a[i],C[i][j]);
		printf("%lld\n",mincost_maxflow());
	}
	return 0;
}

