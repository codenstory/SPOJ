/*
 * GOT
 * TOPIC: HLD + Wavelet Trees
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
#include <map>
#include <set>
#define SH (18)
#define MAXN (BIT(SH))
#define MAXE (MAXN<<1)
#define BIT(k) (1ULL<<(k))
#define is_ancestor(p,x) (in[p] <= in[x] && out[x] <= out[p])
using namespace std;

class WaveletTree {
private:
	map<int,int> *mp;
	WaveletTree *son[2];
	int l,r,mv,*C,n;
	int map_left( int i ) { return i-C[i]; }
	int map_right( int i ) { return C[i]-1; }
	bool is_leaf;
	int count( int i, int c ) {
		if ( i < 0 || !n ) return 0;
		if ( is_leaf ) {
			assert( c == mv );
			return min(n,i+1);
		}
		assert( i >= 0 );
		return c <= mv ? son[0]->count(map_left(i),c) : son[1]->count(map_right(i),c);
	}
public:
	WaveletTree( int *A, int l, int r, int n, map<int,int> *mp ) {
		assert( l <= r );
		this->mv = (((this->l=l)+(this->r=r))>>1), this->n = n;
		this->mp = mp;
		if ( this->is_leaf = (l==r) ) {
			son[0] = son[1] = NULL, C = NULL;
			return ;
		}
		int *x[2],cn[2] = {0},i,j,k;
		for ( i=0; i<n; (A[i++]<=mv)?++cn[0]:++cn[1] );
		for ( i=0; i<=1; x[i]=(int *)malloc(cn[i]*sizeof *x[i]), ++i ) ;
		for ( cn[0]=cn[1]=0, i=0; i<n; A[i]<=mv?(x[0][cn[0]++]=A[i]):(x[1][cn[1]++]=A[i]), ++i ) ;
		C = (int *)malloc(n*sizeof *C);
		for ( C[0]=(A[0]>mv?1:0), i=1; i<n; C[i]=C[i-1]+(A[i]>mv?1:0), ++i ) ;
		son[0] = new WaveletTree(x[0],l,mv,cn[0],mp), son[1] = new WaveletTree(x[1],mv+1,r,cn[1],mp);
		free(x[0]), free(x[1]);
	}
	bool query( int i, int j, int color ) {
		assert( i <= j );
		assert( mp != NULL );
		bool res = mp->find(color)==mp->end()?false:count(j,(*mp)[color])>count(i-1,(*mp)[color]);
		return res;
	}
};

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return -1;
	if ( ch=='-' ) {
		sign = -1, ch = getchar_unlocked();
	}
	assert( dig[ch] );
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && ch != '\0' && isdigit(ch) && dig[ch]; n=10*n+(ch-'0') ) ;
	return sign*n;
}

int to[MAXE],last[MAXN],_next[MAXE],E,V,n,m,K,
	seen[MAXN],yes,card[MAXN],parent[MAXN],anc[MAXN][SH+1],
	heavy_edge[MAXN],
	d[MAXN],
	L[MAXN],R[MAXN],cur_pos,list[MAXN],which_chain[MAXN],
	chain_id,position_in_chain[MAXN],
	color[MAXN],
	in[MAXN],out[MAXN],tick;

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	assert( i >= 0 && j >= 0 );
	to[i] = y, _next[i] = last[x], last[x] = i;
	assert( j >= 0 && j < MAXE );
	to[j] = x; _next[j] = last[y]; last[y] = j;
}

int dfs( int x ) {
	int i,k,y;
	assert( seen[x] != yes );
	assert( 0 <= x && x <= n-1 );
	for ( in[x] = ++tick, seen[x] = yes, card[x] = 1, heavy_edge[x] = -1, i = last[x]; i >= 0; i = _next[i] )
		if ( seen[y = to[i]] != yes ) {
			d[y] = d[x]+1, parent[y] = i;
			for ( anc[y][0] = x, k = 1; anc[y][k-1] != -1; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
			card[x] += dfs(y);
			if ( heavy_edge[x] == -1 || card[y] > card[to[heavy_edge[x]]] )
				heavy_edge[x] = i;
		}
	out[x] = tick;
	return card[x];
}

void hld( int x, int edge_id, bool is_heavy_edge ) {
	int i;
	assert( seen[x] != yes );
	seen[x] = yes;
	if ( edge_id != -1 )
		list[cur_pos++] = edge_id;
	if ( (i=heavy_edge[x])!=-1 ) {
		if ( !is_heavy_edge ) ++chain_id;
		if ( L[chain_id]<0 )
			L[chain_id]=cur_pos;
		if ( R[chain_id]<cur_pos )
			R[chain_id]=cur_pos;
		hld(to[i],i,true);
	}
	for ( i=last[x]; i>=0; i=_next[i] )
		if ( i!=heavy_edge[x] && seen[to[i]] != yes ) 
			hld(to[i],i,false);
}

WaveletTree *tr[MAXN];
int A[MAXN];

int up( int x, unsigned int d ) {
	for ( int k = 0; d; ++k, d >>= 1 )
		if ( d&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( x == y ) return x;
	for ( int k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

bool Q( int u, int v, int col ) {
	assert( is_ancestor(v,u) );
	int cu,cv = which_chain[v];
	while ( (cu = which_chain[u]) != cv ) {
		if ( cu <= chain_id ) {
			if ( tr[cu]->query(0,position_in_chain[u],col) )
				return true ;
			u = to[1^list[L[cu]]];
		}
		if ( color[u] == col )
			return true ;
		if ( u == v )
			return color[u] == col;
		assert( anc[u][0] != -1 );
		u = anc[u][0];
	}
	if ( cu <= chain_id ) {
		assert( position_in_chain[v] <= position_in_chain[u] );
		assert( which_chain[u] == which_chain[v] );
		bool ans = tr[cu]->query(position_in_chain[v],position_in_chain[u],col);
		return ans;
	}
	return color[u] == col;
}

bool query( int x, int y, int k ) {
	if ( x == y ) 
		return color[x] == k;
	int z = lca(x,y);
	return Q(x,z,k) || Q(y,z,k);
}

int main() {
	int i,j,k,l,t,qr,x,y;
	for ( ;(n = getnum()) != -1; ) {
		qr = getnum();
		for ( E = 0, tick=-1,cur_pos=0, chain_id=-1, i=0; i<n; which_chain[i]=-1, L[i]=-1, R[i]=-2, color[i]=getnum(), last[i++]=-1 ) ;
		assert( E >= 0 );
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( parent[i] = -1, k = 0; k < K; anc[i][k++] = -1 ) ;
		assert( E >= 0 );
		for ( k = 0; k < n-1; ++k ) {
			i = getnum(), j = getnum();
			add_arcs(i-1,j-1);
		}
		++yes, dfs(0);
		assert( card[0] == n );
		++yes, hld(0,-1,false);
		for ( i = 0; i <= chain_id; ++i ) {
			assert( L[i] <= R[i] );
			for ( t = 0, l = L[i]; l <= R[i]; ++l ) 
				y = to[list[l]], position_in_chain[y] = l-L[i], which_chain[y] = i, A[t++] = color[y];
			map<int,int> *mp = new map<int,int>();
			set<int> s;
			assert( t >= 1 );
			for ( s.clear(), l = 0; l < t; s.insert(A[l++]) ) ;
			mp->clear(), k = 0;
			for ( set<int> :: iterator it = s.begin(); it != s.end(); ++it )
				if ( mp->find(*it) == mp->end() )
					(*mp)[*it] = k++;
			assert( k >= 1 );
			for ( l = 0; l < t; A[l] = (*mp)[A[l]], ++l ) ;
			tr[i] = new WaveletTree(A,0,k-1,t,mp);
		}
		for ( k = chain_id, x = 0; x < n; ++x )
			if ( which_chain[x] < 0 )
				which_chain[x] = ++k;
		for (;qr--;) {
			i = getnum()-1, j = getnum()-1, k = getnum();
			puts(query(i,j,k)?"Find":"NotFind");
		}
		for ( i = 0; i <= chain_id; delete tr[i++] ) ;
		putchar('\n');
	}
	return 0;
}

