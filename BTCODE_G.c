/*
 * BTCODE_G: Coloring Trees
 * TOPIC: heavy-light decomposition, segment tree, lca
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<MAXK)
#define MAXE (N<<1)
#define MAXK (19)
#define is_parent(x,p) (in[p] <= in[x] && out[x] <= out[p])

int n,to[MAXE],last[N],next[MAXE],E,parent[N],seen[N],yes,
	tick,out[N],in[N],special_child[N],d[N],card[N],
	anc[N][MAXK],K,
	head[N],len,e[N],C,which_chain[N],pos[N];
unsigned int tr[N<<1];

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

int up( int x, unsigned int u ) {
	int k;
	for ( k = 0; k < K && u; u >>= 1, ++k )
		if ( u&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	assert( d[x] == d[y] );
	if ( x == y ) 
		return x;
	for ( k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

void dfs( int x ) {
	int i,k,y;
	assert( seen[x] != yes );
	for ( in[x] = ++tick, special_child[x] = -1, card[x] = 1, seen[x] = yes, i = last[x]; i >= 0; i = next[i] ) 
		if ( seen[y = to[i]] != yes ) {
			for ( parent[y]=i, d[y]=d[x]+1, anc[y][0] = x, k = 1; anc[y][k-1] != -1; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
			dfs(y), card[x] += card[y];
			if ( special_child[x] == -1 || card[y] > card[to[special_child[x]]] )
				special_child[x] = i;
		}
	out[x] = ++tick;
}

void hld( int x, int idx ) {
	int i,y;
	if ( head[C] == -1 )
		head[C] = x;
	e[pos[x] = len++] = idx, which_chain[x] = C;
	if ( special_child[x] != -1 )
		hld(to[special_child[x]],special_child[x]);
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( i != special_child[x] && parent[y = to[i]] == i ) 
			++C, hld(y,i);
}

unsigned int build_tree( int v, int i, int j ) {
	int k = (i+j)/2;
	if ( i == j )
		return tr[v] = 1UL;
	return tr[v] = build_tree(2*v,i,k)|build_tree(2*v+1,k+1,j);
}

unsigned int
update( int v, int i, int j, int qpos, int val ) {
	int k = (i+j)/2;
	if ( qpos > j || qpos < i ) return tr[v];
	if ( i == j ) {
		assert( qpos == i );
		return tr[v] = (1UL<<val);
	}
	return tr[v] = (update(2*v,i,k,qpos,val)|update(2*v+1,k+1,j,qpos,val));
}

unsigned int 
query_tree( int v, int i, int j, int qi, int qj ) {
	int k = (i+j)/2;
	if ( qi > j || qj < i )
		return 0;
	if ( qi <= i && j <= qj )
		return tr[v];
	return query_tree(2*v,i,k,qi,qj)|query_tree(2*v+1,k+1,j,qi,qj);
}

unsigned int 
query_up( int u, int v ) {
	int uchain, vchain = which_chain[v];
	unsigned int ans = 0;
	assert( is_parent(u,v) );
	for ( ;; ) {
		if ( (uchain = which_chain[u]) == vchain ) {
			ans |= query_tree(1,0,len-1,pos[v],pos[u]);
			break ;
		}
		ans |= query_tree(1,0,len-1,pos[head[uchain]],pos[u]);
		u = anc[head[uchain]][0];
	}
	return ans;
}

unsigned int query( int x, int y ) {
	int z = lca(x,y);
	return query_up(x,z)|query_up(y,z);
}

int dig[256];

int num() {
	int ch,n = 0;
	for ( ;(ch = getchar_unlocked()) != EOF && !dig[ch]; ) ;
	if ( ch == EOF )
		return -1;
	for ( n = ch-'0'; (ch = getchar_unlocked()) != EOF && dig[ch]; n = 10*n+ch-'0' ) ;
	return n;
}

int main() {
	int i,j,k,qr,comm;
	unsigned int u;
	for ( i = '0'; i <= '9'; dig[i++] = 1 ) ;
	for ( ;(n = num()) != -1; ) {
		for ( len = tick = 0, C = -1, E = 0, i = 0; i < n; pos[i] = head[i] = -1, last[i++] = -1 ) ;
		for ( k = 0; k < n-1; ++k, add_arcs(num(),num()) ) ;
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( k = 0; k < K; anc[i][k++] = -1 ) ;
		++yes, d[0] = 0, parent[0] = -1, dfs(0);
		++C, hld(0,-1), memset(tr,0,sizeof tr);
		build_tree(1,0,len-1);
		for ( qr = num(); qr--; ) {
			comm = num(), i = num(), j = num();
			if ( comm == 1 ) {
				assert( pos[i] >= 0 ) ;
				update(1,0,len-1,pos[i],j);
			}
			else {
				u = query(i,j);
				if ( u == 1 ) {
					puts("NO");
					continue ;
				}
				puts((u&(u-1))?"NO":"YES");
			}
		}
	}
	return 0;
}

