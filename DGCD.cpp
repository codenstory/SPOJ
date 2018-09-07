/*
 * status: Accepted
 */
#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#define MAXK (18)
#define N (1 << MAXK)
#define MAXE (N<<1)
using namespace std;
typedef int node_type;
typedef int size_type;
//typedef long long value_type;
typedef int value_type;
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
#define Q (N << 5)
#define root (0)
#define null (-1)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
typedef unsigned long long u64;

value_type global_h[Q],global_ans[Q],global_first[Q],global_last[Q],global_g[Q];

inline u64 enc( u64 x, u64 y, u64 z ) { return x|(y<<MAXK)|(z<<(MAXK+MAXK)); }

int getnum() {
	int n= 0,ch;
	static int dig[256]= {0};
	if ( !dig['0'] )
		for ( ch= '0'; ch <= '9'; dig[ch++]= 1 ) ;
	for ( ;(ch= getchar()) != EOF && !dig[ch]; ) ;
	for ( n= ch-'0'; (ch= getchar()) != EOF && dig[ch]; n= 10*n+ch-'0' ) ;
	return n;
}

//value_type GCD( value_type x, value_type y ) 
//{ return !y?x:GCD(y,x%y); }
//{ return __gcd(x,y); }
//{ return !y?labs(x):GCD(labs(y),labs(x)%labs(y)); }
//

value_type GCD( value_type x, value_type y ) {
	x= abs(x); y= abs(y);
	return __gcd(x,y);
}

size_type global_lookup[N],
		  llen[N],_left[N],chain_id;
node_type global_chain[N];

struct chain {
	size_type id;
	chain() {}
	inline node_type pos2node( const size_type pos ) const { 
		assert( 0 <= pos && pos < llen[id] );
		return global_chain[pos+(id?_left[id-1]:0)];
	}
	inline size_type node2pos( const node_type x )   const { 
		return global_lookup[x];
	}
	inline void append( node_type x ) {
		global_lookup[x]= llen[id]++, global_chain[_left[id]++]= x;
	}
	chain( size_type id ) {
		this->id= id, llen[id]= 0, _left[id]= id?_left[id-1]:0;
   	}
	inline size_type len() const { return llen[id]; }
};

class ctree {
	node_type *adj[N];
	value_type w[N];
	size_type n,K,d[N],tin[N],tout[N],tick,cur[N];
	node_type anc[N][MAXK];
	void dfs( node_type x ) {
		tin[x]= ++tick;
		for ( size_type k, i= 0; i < cur[x]; ++i ) {
			node_type y= adj[x][i];
			for ( anc[y][0]= x, k= 1; anc[y][k-1] != null; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
			d[y]= d[x]+1, dfs(y);
		}
		tout[x]= ++tick;
	}
	node_type up( node_type x, size_type u ) const {
		for ( size_type k= 0; u; u>>= 1, ++k )
			if ( u & 1 )
				x= anc[x][k];
		return x;
	}
public:
	inline void reserve( node_type x, size_type card ) {
		adj[x]= (node_type *)malloc(card*sizeof *adj[x]);
		assert( adj[x] );
	}
	node_type lca( node_type x, node_type y ) const {
		if ( tin[x] <= tin[y] && tout[y] <= tout[x] )
			return x;
		if ( tin[y] <= tin[x] && tout[x] <= tout[y] )
			return y;
		if ( d[x] > d[y] )
			return lca(up(x,d[x]-d[y]),y);
		if ( d[y] > d[x] )
			return lca(y,x);
		if ( x == y ) 
			return x;
		for ( size_type k= K-1; k; --k ) {
			assert( anc[x][k] == anc[y][k] );
			if ( anc[x][k-1] != anc[y][k-1] )
				x= anc[x][k-1], y= anc[y][k-1];
		}
		return anc[x][0];
	}
	inline size_type size() const { return n; }
	inline void assign_weight( node_type x, value_type wgt ) { w[x]= wgt; }
	inline value_type weight( const node_type x ) const { return w[x]; }
	node_type level_ancestor( const node_type x, const size_type k ) const { return up(x,k); }
	const pair<node_type *,size_type> children( const node_type x ) const {
		return {adj[x],cur[x]};
	}
	void init( const size_type n ) {
		this->n= n, tick= -1;
		for ( K= 0; (1<<K) <= n; ++K ) ;
		for ( node_type x= root; x < n; cur[x]= 0, adj[x++]= NULL )
			for ( auto k= 0; k < K; anc[x][k++]= null ) ;
	}
	void add_arc( const node_type px, const node_type x ) {
		adj[px][cur[px]++]= x;
	}
	void preprocess() { d[root]= 0, dfs(root); }
};

class range_tree {
	value_type *ans,*g,*first,*last,*h;
	size_type n;
	const chain *ch;
	const ctree *T;
#define push_up(v,i,j) {\
		assert( !h[v] );\
		if ( i < j ) {\
			first[v]= first[L(v)], last[v]= last[R(v)];\
			g[v]= GCD(first[R(v)]-last[L(v)],GCD(g[L(v)],g[R(v)]));\
		}\
		ans[v]= GCD(g[v],first[v]);\
	}
#define push_down(v,i,j) {\
		if ( h[v] ) {\
			if ( i < j ) \
				h[L(v)]+= h[v], h[R(v)]+= h[v];\
			first[v]+= h[v], last[v]+= h[v], h[v]= 0;\
			ans[v]= GCD(g[v],first[v]);\
		}\
	}
	void build( size_type v, size_type i, size_type j ) {
		h[v]= 0;
		assert( i <= j );
		if ( i == j ) {
			g[v]= 0LL, ans[v]= first[v]= last[v]= T->weight(ch->pos2node(i));
			return ;
		}
		auto k= (i+j)>>1;
		build(L(v),i,k), build(R(v),k+1,j);
	   	push_up(v,i,j);
	}
	value_type _query( size_type v,  
					   size_type  i, size_type  j, 
					   size_type qi, size_type qj ) {
		push_down(v,i,j);
		if ( qi > j || qj < i ) return 0LL;
		if ( qi <= i && j <= qj ) 
			return ans[v];
			//return GCD(first[v],g[v]);
		auto k= (i+j)>>1;
		auto res= GCD(_query(L(v),i,k,qi,qj),_query(R(v),k+1,j,qi,qj));
		push_up(v,i,j);
		return res;
	}

	void _update( size_type v, size_type i, size_type j,
				  size_type qi, size_type qj, value_type d ) {
		push_down(v,i,j);
		if ( qi > j || qj < i ) return ;
		if ( qi <= i && j <= qj ) {
			h[v]+= d;
			push_down(v,i,j);
			return ;
		}
		auto k= (i+j)>>1;
		_update(L(v),i,k,qi,qj,d), _update(R(v),k+1,j,qi,qj,d);
		push_up(v,i,j);
	}

public:
	range_tree( const chain *ch, const ctree *T ) : ch(ch), T(T) {

			/*
		ans= global_ans+(ch->id?4*_left[ch->id-1]:0);
		h= global_h+(ch->id?4*_left[ch->id-1]:0);
		first= global_first+(ch->id?4*_left[ch->id-1]:0);
		last= global_last+(ch->id?4*_left[ch->id-1]:0);
		g= global_g+(ch->id?4*_left[ch->id-1]:0);
		*/
		ans= (value_type*)malloc(6*ch->len()*sizeof *ans);
		h= (value_type *)malloc(6*ch->len()*sizeof *ans);
		first= (value_type *)malloc(6*ch->len()*sizeof *ans);
		last= (value_type *)malloc(6*ch->len()*sizeof *ans);
		g= (value_type *)malloc(6*ch->len()*sizeof *ans);

		build(1,0,ch->len()-1);
	}
	value_type query( size_type qi, size_type qj ) {
		return _query(1,0,ch->len()-1,qi,qj);
	}
	void update( size_type qi, size_type qj, value_type d ) {
		_update(1,0,ch->len()-1,qi,qj,d);
	}
};

class cgraph {
	node_type to[MAXE];	
	size_type last[N],next[MAXE],E,n,p[N],card[N];
	bool seen[N];
	void dfs( node_type x ) {
		assert( !seen[x] );
		seen[x]= true ;
		for ( size_type i= last[x]; i != null; i= next[i] )
			if ( !seen[to[i]] ) 
				p[to[i]]= i, dfs(to[i]);
	}
public:
	void add_edge( node_type x, node_type y ) {
		auto i= E++, j= E++;
		to[i]= y, next[i]= last[x], last[x]= i;
		to[j]= x, next[j]= last[y], last[y]= j;
	}
	void init( size_type n ) {
		this->n= n, E= 0;
		for ( node_type x= root; x < n; p[x]= null, seen[x]= false, last[x++]= null ) ;
	}
	void preprocess( ctree &t ) {
		dfs(root); t.init(n);
		assert( p[root] == null );
		for ( node_type x= root; x < n; card[x++]= 0 ) ;
		for ( node_type x= root+1; x < n; ++x ) 
			++card[to[p[x]^1]];
		for ( node_type x= root; x < n; ++x )
			t.reserve(x,card[x]);
		for ( node_type x= root+1; x < n; ++x ) {
			assert( p[x] != null );
			t.add_arc(to[p[x]^1],x);
		}
	}
	inline size_type size() const { return n; }
};

cgraph G;
ctree T;

class hpd {
	const ctree *t;
	chain *chains[N];
	size_type chlen;
	size_type which_chain[N],card[N];
	node_type best_son[N];
	range_tree *rt[N];

	size_type dfs( node_type x ) {
		assert( !card[x] );
		best_son[x]= null;
		const pair<node_type *,size_type> children= t->children(x);
		for ( size_type i= 0; i < children.second; ++i ) {
			node_type y= children.first[i];
			card[x]+= dfs(y);
			if ( best_son[x] == null || card[y] > card[best_son[x]] )
				best_son[x]= y;
		}
		return ++card[x];
	}

	void hld( node_type x, node_type from ) {
		if ( from == null ) 
			chains[chlen++]= new chain(++chain_id);
		which_chain[x]= chain_id, chains[chlen-1]->append(x);
		if ( best_son[x] != null )
			hld(best_son[x],x);
		const pair<node_type *,size_type> children= t->children(x);
		for ( size_type i= 0; i < children.second; ++i ) {
			node_type y= children.first[i];
			if ( y != best_son[x] )
				hld(y,null);
		}
	}

	inline const chain *get_chain( const node_type x ) const { return chains[which(x)]; }
	inline size_type which( node_type x ) const { return which_chain[x]; }

	void extract_chain( u64 *cpath, size_type &nn, node_type px, node_type x ) {
		size_type pid= which(px), id;
		for ( nn= 0; (id= which(x)) != pid; ) {
			const chain *ch= get_chain(x);
			cpath[nn++]= enc(id,0,ch->node2pos(x));
			x= t->level_ancestor(ch->pos2node(0),1);
		}
		assert( which(x) == pid );
		const chain *ch= get_chain(x);
		if ( ch->node2pos(px)+1 <= ch->node2pos(x) )
			cpath[nn++]= enc(pid,ch->node2pos(px)+1,ch->node2pos(x));
	}

	u64 x2z[N], y2z[N];

public:
	void init( const ctree *t ) {
		this->t= t, chlen= 0, chain_id= -1;
		for ( auto x= root; x < t->size(); card[x++]= 0 ) ;
	}
	void preprocess() { 
		dfs(root), hld(root,null);
		for ( auto i= 0; i < chlen; ++i ) 
			rt[i]= new range_tree(chains[i],t);
	}
	
	void update( node_type x, node_type y, value_type val ) {
		node_type z= t->lca(x,y);
		size_type x2zlen,y2zlen;
		extract_chain(x2z,x2zlen,z,x), extract_chain(y2z,y2zlen,z,y);
		for ( auto i= 0; i < x2zlen; ++i ) {
			auto id= x2z[i]&MASK(MAXK);
			auto qi= (x2z[i]>>MAXK)&MASK(MAXK), qj= x2z[i]>>(MAXK+MAXK);
			rt[id]->update(qi,qj,val);
		}
		for ( auto i= 0; i < y2zlen; ++i ) {
			auto id= y2z[i]&MASK(MAXK);
			auto qi= (y2z[i]>>MAXK)&MASK(MAXK), qj= y2z[i]>>(MAXK+MAXK);
			rt[id]->update(qi,qj,val);
		}
		auto id= which(z);
		auto qi= get_chain(z)->node2pos(z), qj= qi;
		rt[id]->update(qi,qj,val);
	}

	value_type query( node_type x, node_type y ) {
		node_type z= t->lca(x,y);
		size_type x2zlen,y2zlen;
		extract_chain(x2z,x2zlen,z,x), extract_chain(y2z,y2zlen,z,y);
		value_type ans= 0LL;
		for ( auto i= 0; i < x2zlen; ++i ) {
			auto id= x2z[i]&MASK(MAXK);
			auto qi= (x2z[i]>>MAXK)&MASK(MAXK), qj= x2z[i]>>(MAXK+MAXK);
			ans= GCD(ans,rt[id]->query(qi,qj));
		}
		for ( auto i= 0; i < y2zlen; ++i ) {
			auto id= y2z[i]&MASK(MAXK);
			auto qi= (y2z[i]>>MAXK)&MASK(MAXK), qj= y2z[i]>>(MAXK+MAXK);
			ans= GCD(ans,rt[id]->query(qi,qj));
		}
		auto id= which(z);
		auto qi= get_chain(z)->node2pos(z), qj= qi;
		ans= GCD(ans,rt[id]->query(qi,qj));
		return ans;
	}
	hpd() {};
};

hpd H;

int main() {
	node_type x,y;
	size_type i,j,k,n,qr;
	int ch;
	value_type wgt;
	char comm[0x10];
	for (;(n= getnum()) >= 0;) {
		for ( G.init(n), k= 0; k < n-1; x= getnum(), y= getnum(), G.add_edge(x,y), ++k ) ;
		G.preprocess(T), T.preprocess();
		for ( x= root; x < n; wgt= getnum(), T.assign_weight(x++,wgt) ) ;
		H.init(&T), H.preprocess();
		for ( qr= getnum(); qr--; ) {
			for ( ;(ch= getchar()) != 'F' && ch != 'C'; ) ;
			x= getnum(), y= getnum();
			switch ( ch ) {
				case 'F': //printf("%lld\n",H.query(x,y));
						  printf("%lld\n",H.query(x,y));
						  break ;
				case 'C': H.update(x,y,getnum());
						  break ;
				default:  assert( 0 );
			}
		}
		break ;
	}
	return 0;
}

