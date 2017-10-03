/*
 * NTICKETS
 * TOPIC: heavy-light decomposition
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH 18
#define MAXN (1<<SH)
#define N (MAXN)
#define MAXE (MAXN<<1)
#define oo (1L<<30)
#define is_ancestor(p,x) (in[p] <= in[x] && out[x] <= out[p])
int max( int x, int y ) { return x<y?y:x; }

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

int K,E,V,n,to[MAXE],next[MAXE],last[MAXN],
	card[N],heavy_edge[N],seen[N],yes,parent[N],depth[N],
	chain_id,position_in_the_chain[N],which_chain[N],
	list[N],cur_pos,
	L[N],R[N],
	anc[N][SH],in[N],out[N],tick;
int weight[MAXE],**tr;

void add_arcs( int x, int y, int w ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, weight[i] = w;
	to[j] = x, next[j] = last[y], last[y] = j, weight[j] = w;
}

int dfs( int x ) {
	int i,k,y;	
	if ( seen[x] == yes ) return card[x];
	for ( in[x] = ++tick, seen[x] = yes, card[x] = 1, heavy_edge[x] = -1, i = last[x]; i >= 0; i = next[i] )
		if ( seen[y=to[i]] != yes ) {
			for ( anc[y][0]=x,parent[y]=i, k=1; anc[y][k-1]>=0; anc[y][k]=anc[anc[y][k-1]][k-1], ++k ) ;
		   	depth[y] = depth[x]+1, card[x] += dfs(y);
			if ( heavy_edge[x] == -1 || card[y] > card[to[heavy_edge[x]]] )
				heavy_edge[x] = i;
		}
	out[x] = tick;
	return card[x];
}

int up( int x, unsigned int d ) {
	int k = 0;
	for ( ;d; d >>= 1, ++k )
		if ( d&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( depth[x] > depth[y] )
		return lca(up(x,depth[x]-depth[y]),y);
	if ( depth[y] > depth[x] )
		return lca(y,x);
	assert( depth[x] == depth[y] );
	if ( x == y )
		return x;
	for ( k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

void hld( int x, int edge_id, int is_heavy_edge ) {
	int i,y;
	assert( seen[x] != yes );
	seen[x] = yes;
	if ( edge_id != -1 ) 
		list[cur_pos++] = edge_id;
	if ( (i=heavy_edge[x]) != -1 ) {
		if ( !is_heavy_edge ) ++chain_id;
		if ( L[chain_id] < 0 )
			L[chain_id] = cur_pos;
		if ( R[chain_id] < cur_pos )
			R[chain_id] = cur_pos;
		hld(to[i],i,1);
	}
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( i != heavy_edge[x] && seen[y = to[i]] != yes )
			hld(y,i,0);
}

int build( int *tr, int v, int i, int j, int left, int right ) {
	int k = ((i+j)>>1), mid = ((left+right)>>1);
	assert( i <= j );
	if ( i == j ) {
		assert( left == right );
		return tr[v] = weight[list[left]];
	}
	return tr[v] = max(build(tr,v<<1,i,k,left,mid),build(tr,1|(v<<1),k+1,j,mid+1,right));
}

int qry( int *tr, int v, int i, int j, int qi, int qj ) {
	int k = ((i+j)>>1);
	if ( qi > j || qj < i ) return -oo;
	if ( qi <= i && j <= qj )
		return tr[v];
	return max(qry(tr,v<<1,i,k,qi,qj),qry(tr,1|(v<<1),k+1,j,qi,qj));
}

int tree_query( int u, int v ) {
	int ans = -oo;
	int cu, cv = which_chain[v];
	assert( is_ancestor(v,u) );

	while ( (cu=which_chain[u]) != cv ) {
		if ( cu <= chain_id ) {
			ans = max(ans,qry(tr[cu],1,0,R[cu]-L[cu],0,position_in_the_chain[u]));
			u = to[1^list[L[cu]]];
		}
		if ( u == v ) return ans;
		ans = max(ans,weight[parent[u]]);
		u = anc[u][0];
	}
	if ( cu > chain_id )
		return ans;
	return max(ans,qry(tr[cu],1,0,R[cu]-L[cu],position_in_the_chain[v]+1,position_in_the_chain[u]));
}

int query( const int x, const int y ) {
	int z;
	if ( x == y ) return 0;
	z = lca(x,y);
	return max(tree_query(x,z),tree_query(y,z));
}

int main() {
	int i,j,k,l,left,right,x,y,qr;
	for (;(n=getnum());) {
		for ( E = 0, i = 0; i < n; L[i] = -1, R[i] = -2, which_chain[i] = -1, last[i++] = -1 ) ;
		for ( K = 0; (1<<K) < n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( depth[i] = 0, k = 0; k < K; anc[i][k++] = -1 ) ;
		for ( l = 0; l < n-1; ++l ) {
			i = getnum()-1, j = getnum()-1, k = getnum();
			add_arcs(i,j,k);
		}
		++yes, tick=-1, parent[0]=-1, dfs(0);
		assert( card[0] == n );
		++yes, cur_pos = 0, chain_id = -1, hld(0,-1,0);
		assert( cur_pos == n-1 );

		tr = (int **)malloc((chain_id+1)*sizeof *tr);
		for ( ++yes, i = 0; i <= chain_id; ++i ) {
			for ( l = L[i]; l <= R[i]; ++l ) {
				x = to[list[l]^1], y = to[list[l]];
				assert( seen[x] != yes );
				seen[x] = yes;
				assert( heavy_edge[x] == list[l] );
				which_chain[y] = i;
				position_in_the_chain[y] = l-L[i];
			}
			assert( R[i] >= L[i] );
			tr[i] = (int *)malloc(((R[i]-L[i]+1)<<2)*sizeof *tr[i]);
			build(tr[i],1,0,R[i]-L[i],L[i],R[i]);
		}
		for ( k = chain_id, x = 0; x < n; ++x )
			if ( which_chain[x] < 0 )
				which_chain[x] = ++k;
		for ( qr = getnum(); qr--; ) {
			x = getnum()-1, y = getnum()-1;
			printf("%d\n",query(x,y));
		}
		for ( i = 0; i < chain_id; free(tr[i++]) ) ;
		free(tr);
	}
	return 0;
}

