/*
 * GCPC11I
 * TOPIC: mincost maxflow, reusing the flow, binary search
 * status: TLE
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
typedef long long i64;
using namespace std;
#define N (0x400)
#define MAXE (1<<21)
#define WILLY (0)
#define S(x) ((x)*(x))
#define oo (1LL<<30)
#define xchg(x,y) (((x)==(y)) || ((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]), xchg(heap[i],heap[j]))

int m,n,V,E,to[MAXE],next[MAXE],last[N],src,sink,heap[N<<1],pos[N],cn,parent[N],id[N],
	yes,q[N],*head,*tail,visited[N],indeed,
	seen[N];
i64 flow[MAXE],cap[MAXE],sz[N],d[N],p[N],
	xx[N],yy[N],rr[N],w[MAXE],sum[N],old_cap[MAXE];
bool active[N];

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
	old_cap[i] = c, old_cap[j] = 0;
}

i64 dfs( int x, i64 df ) {
	i64 dt,i,y;
	if ( x == sink ) 
		return df;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x] = yes, i = last[x]; i >= 0; i = next[i] )
		if ( visited[y = to[i]] == indeed && d[y]+1 == d[x] && cap[i] > flow[i] ) 
			if ( dt = dfs(y,min(cap[i]-flow[i],df)) ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

bool bfs() {
	int x,y,i,j,k;
	for ( head = tail = q, d[*tail++ = sink] = 0, visited[sink] = ++indeed; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && (visited[y=to[i]] != indeed || d[y]>d[x]+1) )
				d[y] = d[x]+1, visited[*tail++ = y] = indeed;
	return visited[src] == indeed;
}

i64 maxflow() {
	i64 T = 0, dt;
	for ( ;bfs(); )
		for ( ;++yes && (dt = dfs(src,+oo)); T += dt ) ;
	return T;
}

int main() {
	int i,j,k,ts;
	bool ok;
	i64 low, high, mid, FLOW, fl, prev, W;
	for ( ts = getnum(); ts--; ) {
		m = getnum(), n = getnum();
		assert( m+n+2 < N );
		for ( i = 0; i < m; ++i ) 
			xx[i] = getnum(), yy[i] = getnum(), sz[i] = getnum(), rr[i] = getnum(), rr[i] *= rr[i];
		for ( i = 0; i < n; ++i ) 
			xx[m+i] = getnum(), yy[m+i] = getnum(), sz[m+i] = getnum();
		for ( E = 0, V = m+n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( S(xx[i]-xx[m+j])+S(yy[i]-yy[m+j]) <= rr[i] )
					add_arcs(i,m+j,sz[m+j]);
		for ( i = 0; i < m; ++i )
			add_arcs(src,i,+oo), id[i] = E-2;
		for ( j = 0; j < n; ++j )
			add_arcs(m+j,sink,sz[m+j]);

		FLOW = maxflow();

		/* reset the whole thing */
		for ( E = 0, V = m+n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( j = 0; j < n; ++j ) active[j] = true ;
		for ( W = sz[WILLY], j = 0; j < n; ++j )
			if ( S(xx[WILLY]-xx[m+j])+S(yy[WILLY]-yy[m+j]) <= rr[WILLY] )
				W += sz[m+j], active[j] = false ;
		for ( ok = true, i = 1; i < m; ++i )
			if ( sz[i] > W ) {
				ok = false ;
				break ;
			}
		if ( !ok ) {
			puts("Lonesome Willy");
			continue ;
		}
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				//if ( active[j] )
					if ( S(xx[i]-xx[m+j])+S(yy[i]-yy[m+j]) <= rr[i] )
						add_arcs(i,m+j,sz[m+j]);
		for ( i = 0; i < m; ++i )
			add_arcs(src,i,W-sz[i]), id[i] = E-2;
		for ( j = 0; j < n; ++j )
			add_arcs(m+j,sink,sz[m+j]);
		puts(FLOW == maxflow() ? "Suiting Success":"Lonesome Willy");
	}
	return 0;
}

