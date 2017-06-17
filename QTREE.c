/*
 * QTREE: Query on a Tree
 * TOPIC: heavy-light decomposition, lca, trees
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define oo (1<<30)
#define MAXK (15)
#define MAXV (1<<MAXK)
#define MAXE (MAXV)
#define ROOT (0)
#define TROOT (1)
#define DBG 1
#define is_parent(z,x) (in[z] <= in[x] && out[x] <= out[z])

int max( int x, int y ) { return x<y?y:x; }

int n,to[MAXE],next[MAXE],last[MAXV],E,V,cost[MAXE],parent[MAXV],idx[MAXV],
	seen[MAXV],yes,card[MAXV],skip[MAXV],special_child[MAXV],
	C,head[MAXV],pos[MAXV],which_chain[MAXV],chain_size[MAXV],d[MAXV],
	anc[MAXV][MAXK],K,base_pos[MAXV],
	in[MAXV],out[MAXV],tick,
	tr[MAXV<<2],arr[MAXV],len,
	pos_in_arr[MAXE];

void add_arcs( int x, int y, int k ) {
	int i = E++, j = E++;
	to[i]=y, next[i]=last[x], last[x]=i, cost[i]=k;
	to[j]=x, next[j]=last[y], last[y]=j, cost[j]=k;
}

void dfs( int x ) {
	int i,k,y;
	assert( seen[x] != yes ) ;
	for ( in[x] = ++tick, seen[x] = yes, card[x] = 1, special_child[x] = -1, i = last[x]; i >= 0; i = next[i] ) 
		if ( seen[y=to[i]] != yes ) {
			parent[y] = i, d[y] = d[x]+1;
			for ( anc[y][0] = x, k = 1; anc[y][k-1] != -1; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
		   	dfs(y), card[x] += card[y];
			if ( special_child[x] == -1 || card[y] > card[to[special_child[x]]] )
				special_child[x] = i;
		}
	out[x] = ++tick;
}

void hld( int x ) {
	int i,y;
	if ( head[C] == -1 ) head[C] = x;
	pos[x] = chain_size[which_chain[x]=C]++;
	if ( special_child[x] != -1 ) 
		hld(y = to[special_child[x]]);
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( parent[y=to[i]]==i && special_child[x]!=i )
			head[++C]=-1, chain_size[C]=0, hld(y);
}

int query_tree( int v, int i, int j, int qi, int qj ) {
	int k = (i+j)/2;
	if ( qi > j || qj < i ) return -oo;
	if ( qi <= i && j <= qj )
		return tr[v];
	return max(query_tree(2*v,i,k,qi,qj),query_tree(2*v+1,k+1,j,qi,qj));
}

int build_tree( int v, int i, int j ) {
	int k = (i+j)/2;
	if ( i == j ) 
		return tr[v] = cost[arr[i]];
	return tr[v] = max(build_tree(2*v,i,k),build_tree(2*v+1,k+1,j));
}

int up( int x, unsigned int d ) {
	int k;
	for ( k = 0; k < K; ++k )
		if ( (d>>k)&1 )
			x = anc[x][k];
	assert( x != -1 );
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	if ( x == y ) return x;
	for ( k = K-1; k; --k ) {
		/*assert( x != y );*/
		assert( anc[x][k] == anc[y][k] ) ;
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
#if DBG
	assert( anc[x][0] == anc[y][0] );
#endif
	assert( x != y );
	assert( anc[x][0] == anc[y][0] );
	return anc[x][0];
}

int query_up( int u, int v ) {
	int uchain, vchain = which_chain[v], ans = -oo;
#if DBG
	assert( is_parent(v,u) );
#endif
	for ( ;; ) {
		if ( (uchain = which_chain[u]) == vchain ) {
			ans = max(ans,query_tree(TROOT,0,len-1,base_pos[v],base_pos[u]-1));
			break ;
		}
		ans = max(ans,query_tree(TROOT,0,len-1,base_pos[head[uchain]],base_pos[u]-1));
		u = head[uchain];
#if DBG
		assert ( parent[u] != -1 ) ;
#endif
		ans = max(ans,cost[parent[u]]);
		u = anc[u][0];
	}
	return ans;
}

int query( int x, int y ) {
	int z = lca(x,y);
	assert( z != -1 );
	assert( is_parent(z,x) );
	assert( is_parent(z,y) );
	return max(query_up(x,z),query_up(y,z));
}

int update( int v, int i, int j, int loc, int new_value ) {
	int k = (i+j)/2;
	if ( i > loc || j < loc ) return tr[v];
	if ( i == j ) 
		return tr[v] = cost[arr[loc]] = cost[arr[loc]^1] = new_value;
	return tr[v] = max(update(2*v,i,k,loc,new_value),update(2*v+1,k+1,j,loc,new_value));
}

int main() {
	int i,j,k,l,t,ts,x,y;
	char comm[0x20];
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( E = 0, i = 0; i < n; parent[i] = -1, last[i++] = -1 ) ;
		for ( l = 0; l < n-1 && 3 == scanf("%d %d %d",&i,&j,&k); --i, --j, idx[l++] = E, add_arcs(i,j,k) ) ;
		for ( i = 0; i < E; pos_in_arr[i++] = -1 ) ;
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < K; anc[i][j++] = -1 ) ;
		++yes, tick = 0, d[ROOT] = 0, dfs(ROOT);
		head[C=0] = -1,chain_size[C]=0, hld(ROOT);
		for ( cost[E] = +oo, len = 0, k = 0; k <= C; arr[base_pos[x]=len++] = E, ++k ) 
			for ( i=special_child[x=head[k]], l=1; l<chain_size[k]; ++l )
				base_pos[x]=len, pos_in_arr[arr[len]=i]=len, ++len, x=to[i], i=special_child[x];
		build_tree(TROOT,0,len-1);
		for ( ;1==scanf("%s",comm) && 0[comm]!='D' && 2==scanf("%d %d",&i,&j); ) 
			switch ( 0[comm] ) {
				case 'Q': printf("%d\n",query(--i,--j)); break ;
				default:  i = idx[i-1];
						  if ( pos_in_arr[i] != -1 )
							 update(TROOT,0,len-1,pos_in_arr[i],j);
						  else if ( pos_in_arr[i^1] != -1 )
							 update(TROOT,0,len-1,pos_in_arr[i^1],j);
						  else cost[i] = cost[i^1] = j;
						  break ;
			}
	}
	return 0;
}

