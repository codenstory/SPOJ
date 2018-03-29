/*
 * GASWARS
 * TOPIC: maxflow, binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define MAXE (1<<17)
#define oo (1LL<<30)
typedef long long i64;
#define xchg(x,y) (((x)==(y)) || ((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))

i64 min( i64 x, i64 y ) { return x < y ? x:y; }

int n,heap[N],pos[N],cnt,seen[N],yes,parent[N],visited[N],aha,q[N],*head,*tail,V,
	to[MAXE],next[MAXE],last[N],E,active[MAXE],indeed;
i64 flow[MAXE],cap[MAXE],cost[MAXE],p[N],d[N];

/*
void push( int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[heap[cnt] = x] = cnt, ++cnt;
	for ( j = pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cnt += (pos[x] = -1) )
		pos[*heap = heap[cnt]] = 0;
	for ( j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

int dijkstra( int src, int dst ) {
	int i,j,k,x,y;
	for ( x = 0; x < n; pos[x++] = -1 ) ;
	for ( d[src] = 0, seen[src] = ++yes, cnt = 0, push(src); cnt; )
		for ( i = last[x = pop()]; i >= 0; i = next[i] )
			if ( active[i] == indeed ) 
				if ( cap[i] > flow[i] && (seen[y=to[i]] != yes || d[y=to[i]] > d[x]+p[x]-p[y=to[i]]+cost[i]) )
					d[y] = d[x]+p[x]-p[y]+cost[i], seen[y] = yes, parent[y] = i, push(y);
	for ( x = 0; x < n; ++x )
		if ( seen[x] == yes )
			p[x] += d[x];
	return seen[dst] == yes;
}
*/

i64 dfs( int sink, int x, i64 df ) {
	int y,i;
	i64 dt;
	if ( x == sink )
		return df;
	if ( seen[x] == yes ) return 0;
	for ( seen[x] = yes, i = last[x]; i >= 0; i = next[i] )
		if ( active[i] == indeed )
			if ( cap[i] > flow[i] && visited[y=to[i]] == aha && d[y]+1 == d[x] ) 
				if ( dt = dfs(sink,y,min(df,cap[i]-flow[i])) ) {
					flow[i] += dt, flow[i^1] -= dt;
					return dt;
				}
	return 0;
}

int bfs( int src, int sink ) {
	int x,y,i,j,k;
	for ( head = tail = q, visited[*tail++ = sink] = ++aha, d[sink] = 0; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( active[i^1] == indeed && active[i] == indeed && cap[i^1] > flow[i^1] )
				if ( visited[y = to[i]] != aha || d[y] > d[x]+1 )
					visited[y] = aha, d[y] = d[x]+1, *tail++ = y;
	return visited[src] == aha;
}

void add_arcs( int x, int y, int capacity ) {
	int i = E++, j = E++;
	assert( y >= 0 );
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = capacity, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

i64 maxflow( int src, int sink ) {
	int x,i;
	i64 T=0,df;
	/*
	for ( x = 0; x < n; p[x++] = 0 ) ;
	for ( ;dijkstra(src,sink); T += df ) {
		for ( i=parent[sink],df=cap[i]-flow[i],x=to[i^1]; x != src; i = parent[x],df = min(df,cap[i]-flow[i]), x = to[i^1] ) ;
		for ( i=parent[sink],flow[i]+=df,flow[i^1]-=df,x=to[i^1]; x != src; i=parent[x],flow[i]+=df,flow[i^1]-=df,x=to[i^1] ) ;
	}
	return T;
	*/
	for ( ;bfs(src,sink); )
		for ( ;++yes && (df = dfs(sink,src,+oo)); T += df ) ;
	return T;
}

#define inside(x) (0<=(x) && (x)<n)

int main() {
	int i,j,k,kk,l,ts,g,m,t,src,sink,supersource,supersink;
	i64 low,high,mid;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d %d %d",&n,&m,&k,&l,&g);
		for ( E = 0, V = n+4, src = V-2, sink = V-1, supersource = V-4, supersink = V-3, i = 0; i < V; last[i++] = -1 ) ;
		for ( kk = 0; kk < m; ++kk ) {
			scanf("%d %d %d",&i,&j,&t);
			add_arcs(i-1,j-1,t), add_arcs(j-1,i-1,t);
		}
		for ( i = 0; i < k; ++i ) {
			scanf("%d",&j), --j;
			add_arcs(src,j,+oo);
		}
		for ( i = 0; i < l; ++i ) {
			scanf("%d",&j), --j;
			add_arcs(j,sink,+oo);
		}
		add_arcs(supersource,src,g), add_arcs(sink,supersink,g);
		for ( low = 0, high = +oo; low+1 < high; ) {
			mid = (low+high)>>1;
			for ( ++indeed, i = 0; i < E; i += 2 ) {
				if ( cap[i] <= mid || !(inside(to[i]) && inside(to[i^1])) ) 
					active[i] = active[i^1] = indeed;
				flow[i] = flow[i^1] = 0;
			}
			if ( maxflow(supersource,supersink) < g ) low = mid;
			else high = mid;
		}
		if ( high < +oo ) {
			printf("%lld\n",100*high);
		}
		else puts("-1");
	}
	return 0;
}

