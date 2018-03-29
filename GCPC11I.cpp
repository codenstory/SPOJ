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
	active[MAXE],yes;
i64 flow[MAXE],cap[MAXE],sz[N],d[N],p[N],
	xx[N],yy[N],rr[N],w[MAXE],sum[N],old_cap[MAXE];

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

void add_arcs( int x, int y, int c, i64 a, i64 b ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
	old_cap[i] = c, old_cap[j] = 0;
	w[i] = a, w[j] = -a;
}

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[heap[cn] = x] = cn, ++cn;
	for ( j = pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cn += (pos[x] = -1) )
		pos[*heap = heap[cn]] = 0;
	for ( j = 0; (i=j,j<<=1,++j) < cn; bubble ) {
		if ( j < cn-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

bool dijkstra() {
	int x,y,i,j,k;
	for ( cn = 0, x = 0; x < V; d[x] = +oo, pos[x++] = -1 ) ;
	for ( d[src] = 0, push(src); cn; )
		for ( i = last[x=pop()]; i >= 0; i = next[i] )
			if ( cap[i] > flow[i] && d[y=to[i]] > d[x]+p[x]-p[y=to[i]]+w[i] )
				d[y] = d[x]+p[x]-p[y]+w[parent[y] = i], push(y);
	for ( x = 0; x < V; ++x )
		if ( d[x] < +oo )
			p[x] += d[x];
	return d[sink] < +oo;
}

i64 mincost_maxflow() {
	int x,i;
	i64 T = 0, dt;
	for ( x = 0; x < V; p[x++] = 0 ) ;
	for ( ;dijkstra(); ) {
		for ( i=parent[sink], dt=cap[i]-flow[i]; (x=to[i^1]) != src; i=parent[x],dt=min(dt,cap[i]-flow[i]) ) ;
		for ( i=parent[sink], flow[i]+=dt, flow[i^1]-=dt; (x=to[i^1]) != src; i=parent[x], flow[i]+=dt, flow[i^1]-=dt ) ;
	}
	for ( x = 0; x < m; ++x )
		T += flow[id[x]];
	return T;
}

int get_winner() {
	int best = -1;
	for ( int x = 0; x < m; ++x ) {
		sum[x] = sz[x]+flow[id[x]];
		if ( best == -1 || sum[x] > sum[best] )
			best = x;
	}
	return best;
}

int main() {
	int i,j,k,ts;
	bool ok;
	i64 low, high, mid, FLOW, fl;
	for ( ts = getnum(); ts--; ) {
		m = getnum(), n = getnum();
		for ( i = 0; i < m; ++i ) 
			xx[i] = getnum(), yy[i] = getnum(), sz[i] = getnum(), rr[i] = getnum(), rr[i] *= rr[i];
		for ( i = 0; i < n; ++i ) {
			xx[m+i] = getnum(), yy[m+i] = getnum(), sz[m+i] = getnum();
		}
		for ( E = 0, V = m+n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( S(xx[i]-xx[m+j])+S(yy[i]-yy[m+j]) <= rr[i] )
					add_arcs(i,m+j,sz[m+j],(i==WILLY?0:1),0);
		for ( i = 0; i < m; ++i )
			add_arcs(src,i,+oo,0,0), id[i] = E-2;
		for ( j = 0; j < n; ++j )
			add_arcs(m+j,sink,sz[m+j],0,0);
		FLOW = mincost_maxflow();
		for ( ok = false, low = -1, high = 10000; low+1 < high && !ok; ) {
			mid = (low+high)>>1;
			for ( i = 0; i < E; ++i ) {
				if ( 0 <= to[i] && to[i] <= m+n-1 && 0 <= to[i^1] && to[i^1] <= m+n-1 )
					cap[i] = min(old_cap[i],mid);
				if ( mid <= high )
					flow[i] = 0;
			}
			if ( (fl = mincost_maxflow()) >= FLOW ) {
				high = mid;
				if ( get_winner() == WILLY ) ok = true ;
			}
			else low = mid;
		}
		if ( ok ) {
			puts("Suiting Success");
			continue ;
		}
		else {
			puts("Lonesome Willy");
			continue ;
		}
		for ( i = 0; i < E; ++i )  {
			if ( 0 <= to[i] && to[i] <= m+n-1 && 0 <= to[i^1] && to[i^1] <= m+n-1 )
				cap[i] = min(old_cap[i],high);
			flow[i] = 0;
		}
		assert( FLOW == mincost_maxflow() );
		puts(get_winner()==WILLY?"Suiting Success":"Lonesome Willy");
	}
	return 0;
}

