/*
 * OPTM
 * TOPIC: mincut, maxflow
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
#define tol (1e-13)
#define MAXL (31)
#define MAXN (512)
#define MAXE (1<<21)
#define BIT(k) (1LL<<(k))
#define TST(u,k) (((u)>>(k))&1)
#define oo (1LL<<60)
typedef long long i64;
using namespace std;

bool marked[MAXN];
int n,m,K,xx[3000],yy[3000],len,E,V,src,sink,
	to[MAXE],next[MAXE],last[MAXN],present[MAXN],absent[MAXN],
	seen[MAXN],yes,visited[MAXN],indeed,d[MAXN],q[MAXN],*head,*tail;
i64 cap[MAXE],flow[MAXE],mark[MAXN];

class Graph {
	private:
		int T;
		void add_arcs( int x, int y, i64 c ) {
			int i = E++, j = E++;
			to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
			to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
		}
		i64 dfs( int x, i64 df ) {
			int i,y;
			i64 dt;
			if ( x == sink )
				return df;
			if ( visited[x] == indeed ) return 0;
			for ( visited[x] = indeed, i = last[x]; i >= 0; i = next[i] ) 
				if ( flow[i] < cap[i] && seen[y=to[i]] == yes && d[y]+1 == d[x] )
					if ( dt = dfs(y,min(df,cap[i]-flow[i])) ) {
						flow[i] += dt, flow[i^1] -= dt;
						return dt;
					}
			return 0;
		}
		bool bfs() {
			int x,y,i;
			for ( head=tail=q, d[*tail++=sink]=0,seen[sink]=++yes; head < tail; )
				for ( i = last[x=*head++]; i >= 0; i = next[i] )
					if ( flow[i^1] < cap[i^1] && seen[y=to[i]] != yes )
						seen[y] = yes, d[*tail++=y]=d[x]+1;
			return seen[src] == yes;
		}
		void maxflow() { for (;bfs();) for (;++indeed&&dfs(src,+oo);); }
	public:
		Graph( int T ) {
			int i,j,k,l;
			for ( this->T=T,E=0,V=2+n,src=V-2,sink=V-1, i = 0; i < V; last[i++] = -1 ) ;
			memset(present,0,sizeof present), memset(absent,0,sizeof absent);
			for ( l = 0; l < len; ++l ) {
				i = xx[l], j = yy[l];
				if ( marked[i] && marked[j] ) continue ;
				if ( !marked[i]  && !marked[j] ) {
					add_arcs(i,j,1), add_arcs(j,i,1);
					continue ;
				}
				if ( marked[i] ) swap(i,j);
				assert( !marked[i] && marked[j] ) ;
				if ( TST(mark[j],T) )
					++present[i];
				else ++absent[i];
			}
			for ( i = 0; i < n; ++i )
				if ( !marked[i] ) 
					add_arcs(src,i,present[i]), add_arcs(i,sink,absent[i]);
		}
		void assign_marks() {
			int x,y,i;
			for ( maxflow(), head=tail=q, seen[*tail++=src] = ++yes; head < tail; )
				for ( i=last[x=*head++]; i >= 0; i = next[i] )
					if ( flow[i] < cap[i] && seen[y=to[i]] != yes )
						seen[*tail++=y] = yes;
			for ( i = 0; i < E; ++i ) 
				if ( seen[x=to[i^1]] == yes && seen[y=to[i]] != yes ) 
					if ( 0 <= x && x < n && y == sink && !marked[x] ) mark[x] |= BIT(T);
		}
};

int main() {
	int i,j,k,l,ts;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( len = m, l = 0; l < len && 2 == scanf("%d %d",&i,&j); xx[l]=--i, yy[l]=--j, ++l ) ;
		memset(mark,0,sizeof mark), memset(marked,0,sizeof marked);
		for ( scanf("%d",&K), l = 0; l < K; scanf("%d %d",&i,&k), marked[--i]=true, mark[i]=k, ++l ) ;
		for ( i = 0; i < MAXL; (new Graph(i++))->assign_marks() ) ;
		for ( i = 0; i < n; printf("%lld\n",mark[i++]) ) ;
	}
	return 0;
}

