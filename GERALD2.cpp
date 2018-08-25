/*
 * GERALD2: Black and White Tree
 * TOPIC: heavy-path decomposition, LCA queries, heaps, dynamic data structures
 * status: Accepted
 */
#include <bits/stdc++.h>
typedef int node_type;
typedef int size_type;
typedef long long value_type;
using namespace std;
enum { WHITE, BLACK };
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
#define N (1<<19)
#define MAXE (N<<1)
#define START (0)
#define MAXK (21)
#define oo (1LL<<62)
#define null (-1)
#define root (0)

int n,qr;
char color[N];

// a comparable (node,distance) object
struct state {
	node_type x;
	value_type w;
	state() {};
	state( node_type x, value_type w ) : x(x), w(w) {};
	// pick largest distance, and largest index in case of ties
	bool operator < ( const state &rhs ) const {
		if ( w==rhs.w ) 
			return x>rhs.x;
		return w>rhs.w;
	}
	bool operator > ( const state &rhs ) const {
		return rhs<*this;
	}
	bool operator == ( const state &rhs ) const {
		return !(*this<rhs || rhs<*this);
		//return w==rhs.w && x==rhs.x;
	}
};

const state neutral_element= state(null,-oo);

state operator + ( const value_type dw, const state &s ) {
	return state(s.x,s.w+dw);
}

class heap {
	set<state> s;
public:
	heap() { s.clear(); }
	void init() { s.clear(); }
	const state &top() const {
		if ( !s.size() ) 
			return neutral_element;
		return *(s.begin());
	}
	void push( const state &entry ) {
		if ( entry.w >= 0 )
			s.insert(entry);
	}
	void remove( const state &item ) {
		auto res= s.erase(item);
		//assert( res >= 1 );
	}
};

class chain {
	vector<node_type> c;
	mutable unordered_map<node_type,size_type> p;
public:
	chain() { c.clear(), p.clear(); };
	void append( const node_type x ) {
		assert( !p.count(x) );
		p[x]= c.size(), c.push_back(x);
	}
	size_type pos( const node_type x ) const {
		assert( p.count(x) );
		return p[x];
	}
	size_type len() const { return (size_type)(c.size()); }
	node_type access( const size_type i ) const { 
		assert( 0 <= i && i < c.size() );
		auto x= c[i]; 
		assert( pos(x) == i );
		return x;
	}
};

heap h[N];

template<typename T>
struct update_policy {
	void operator ()( T *arr, size_type v, size_type i, size_type j ) ;
};

template<typename T>
struct prefix_update: update_policy<T> {
	void operator ()( T *arr, size_type v, size_type i, size_type j ) {
		if ( i < j ) {
			auto k= (i+j)>>1;
			arr[v]= min(arr[L(v)],(k+1-i)+arr[R(v)]);
		}
	}
};

template<typename T>
struct suffix_update: update_policy<T> {
	void operator ()( T *arr, size_type v, size_type i, size_type j ) {
		if ( i < j ) {
			auto k= (i+j)>>1;
			arr[v]= min(arr[R(v)],(j-k)+arr[L(v)]);
		}
	}
};

template<class upolicy= update_policy<state>>
class range_tree {
private:
	bool is_prefix;
	upolicy updater;
	const chain *c;
	state *tr;
	size_type n;

	void _update( size_type v, size_type i, size_type j, 
		const size_type pos, const state val ) {
		if ( i > pos || pos > j )
			return ;
		if ( pos == i && j == pos ) {
			tr[v]= val;
			return ;
		}
		auto k= (i+j)>>1;
		_update(L(v),i,k,pos,val), _update(R(v),k+1,j,pos,val);
		updater(tr,v,i,j);
	}

	void build( size_type v, size_type i, size_type j ) {
		if ( i == j ) {
			assert( v < (n<<2) );
			tr[v]= h[c->access(i)].top();
			return ;
		}
		auto k= (i+j)>>1;
		build(L(v),i,k), build(R(v),k+1,j);
		updater(tr,v,i,j);
	}

	state _query( size_type v, size_type i, size_type j, 
			const size_type qi, const size_type qj ) const {
		if ( qi > qj || qi > j || qj < i )
			return neutral_element;

		if ( qi <= i && j <= qj ) {
			if ( (is_prefix && qi == i) ||\
				 (!is_prefix && qj == j) )
				return tr[v];
			return neutral_element;
		}

		auto k= (i+j)>>1;
		if ( qj <= k )
			return _query(L(v),i,k,qi,qj);
		if ( k+1 <= qi )
			return _query(R(v),k+1,j,qi,qj);
		assert( qi <= k && k+1 <= qj );
		if ( is_prefix ) 
			return min(_query(L(v),i,k,qi,qj),qi>=i?(k+1-qi)+_query(R(v),k+1,j,k+1,qj):neutral_element);
		else 
			return min(_query(R(v),k+1,j,qi,qj),qj<=j?(qj-k)+_query(L(v),i,k,qi,k):neutral_element);
	}

public:
	range_tree( const chain *c, bool flag ) : is_prefix(flag) {
		this->n= (this->c= c)->len(); 
		tr= new state[n<<2];
		build(1,0,n-1);
	};
	void update( const node_type x ) {
		//printf("Going to update chain at position %d (element is %d) with weight %lld\n",c->pos(x),x,h[x].top().w);
		_update(1,0,n-1,c->pos(x),h[x].top());
	}
	state query( const size_type qi, const size_type qj ) const {
		return _query(1,0,n-1,qi,qj);
	}
	~range_tree() { delete tr; }
};

class directed_tree {
private:
	size_type n,K,d[N];
	vector<node_type> child[N];
	node_type parent[N],anc[N][MAXK];
	void dfs( node_type x ) {
		size_type i,k; node_type y;
		for ( i= 0; i < (int)child[x].size(); ++i ) {
			for ( anc[y= child[x][i]][0]= x, k= 1; anc[y][k-1] != null; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
			d[y]= d[x]+1, dfs(y);
		}
	}
	node_type up( node_type x, size_type u ) const {
		for ( size_type k= 0; u; u>>= 1, ++k )
			if ( u&1 )
				x= anc[x][k];
		return x;
	}
	node_type lca( node_type x, node_type y ) const {
		if ( d[x] > d[y] )
			return lca(up(x,d[x]-d[y]),y);
		if ( d[x] < d[y] )
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
public:
	size_type distance( const node_type x, const node_type y ) const 
	{ return d[x]+d[y]-2*d[lca(x,y)]; }
	node_type level_ancestor( const node_type x, const size_type k ) const 
	{ return up(x,k); }
	directed_tree() {};
	void init( size_type n ) {
		this->n= n;
		for ( node_type x= 0; x < n; parent[x]= null, child[x++].clear() ) ;
		for ( K= 0; (1 << K) <= n; ++K ) ;
		for ( node_type x= root; x < n; ++x )
			for ( size_type k= 0; k < K; anc[x][k++]= null ) ;
	}
	void add_arc( node_type p, node_type x ) {
		child[parent[x]= p].push_back(x);
	}
	const vector<node_type> &get_adj( node_type x ) const {
		return child[x];
	}
	size_type get_size() const {
		return n;
	}
	void preprocess() { d[root]= 0, dfs(root); }
};

range_tree<prefix_update<state>> *pref[N];
range_tree<suffix_update<state>> *suff[N];

class hld {
private:
	vector<chain> chains;
	node_type best_son[N];
	size_type which_chain[N],
			  card[N];
	const directed_tree *T;
	size_type dfs( node_type x ) {
		assert( !card[x] );
		const vector<node_type> &adj= T->get_adj(x);
		best_son[x]= null, card[x]= 1;
		for ( auto y: adj ) {
			card[x]+= dfs(y);
			if ( best_son[x] == null || card[y] > card[best_son[x]] )
				best_son[x]= y;
		}
		return card[x];
	}
	void process( node_type x, node_type fr ) {
		if ( fr == null ) 
			chains.push_back(chain());
		chains[chains.size()-1].append(x), which_chain[x]= chains.size()-1;
		if ( best_son[x] != null )
			process(best_son[x],x);
		const vector<node_type> &adj= T->get_adj(x);
		for ( auto y: adj )
			if ( y != best_son[x] )
				process(y,null);
	}
public:
	hld() {};
	void init( const directed_tree *T ) {
		this->T= T;
		for ( node_type x= root; x < n; card[x++]= 0 ) ;
		chains.clear(), dfs(root); //we assume the directed tree is always rooted at "0"
		process(root,null);
		for ( int i= 0; i < (int)chains.size(); ++i ) {
			pref[i]= new range_tree<prefix_update<state>>(&chains[i],true);
			suff[i]= new range_tree<suffix_update<state>>(&chains[i],false);
		}
	}
	inline size_type which( const node_type x ) const 
	{ return which_chain[x]; }
	inline const chain &get_chain( const node_type x ) const 
	{ return chains[which(x)]; }

	node_type 
	jump( const node_type x ) const {
		node_type head= chains[which(x)].access(0);
		if ( head == root ) 
			return null;
		node_type pr= T->level_ancestor(head,1);
		return pr;
	}

};

class cgraph {
private:
	node_type to[MAXE];
	size_type last[N],_next[MAXE],E,n,p[N];
	bool seen[N];
	void dfs( node_type x ) {
		size_type i; node_type y;
		for ( seen[x]= true, i= last[x]; i >= 0; i= _next[i] )
			if ( !seen[y= to[i]] ) 
				p[y]= i, dfs(y);
	}
public:
	void init( size_type n ) {
		for ( node_type x= root; x < n; p[x]= last[x]= null, seen[x++]= false ) ;
		E= 0, this->n= n;
	}
	void add_arcs( node_type x, node_type y ) {
		size_type i= E++, j= E++;
		to[i]= y, _next[i]= last[x], last[x]= i;
		to[j]= x, _next[j]= last[y], last[y]= j;
	}
	cgraph() {}
	void preprocess( directed_tree &T ) {
		T.init(n), dfs(root);
		size_type k= 0;
		for ( node_type x= 0; x < n; ++x )
			if ( p[x] >= 0 )
				T.add_arc(to[p[x]^1],x), ++k;
		assert( p[root] == null ); assert( k == n-1 );
	}
};

cgraph G;
directed_tree T;
hld H;

node_type resolve_query( node_type el ) {
	node_type x= el,px= null;
	size_type i,pi= null;

	state candidate= state(el,0);

	for ( ;x != null; pi= i, px= x, x= H.jump(x) ) {
		i= H.which(x);
		if ( px != null ) {
			const chain &pch= H.get_chain(px);
			h[x].remove(1+pref[pi]->query(0,pch.len()-1));
			pref[i]->update(x), suff[i]->update(x);
		}
		const chain &ch= H.get_chain(x);
		candidate= min(candidate,T.distance(x,el)+pref[i]->query(ch.pos(x),ch.len()-1));
		candidate= min(candidate,T.distance(x,el)+suff[i]->query(0,ch.pos(x)));
		if ( px != null ) {
			const chain &pch= H.get_chain(px);
			h[x].push(1+pref[pi]->query(0,pch.len()-1));
			pref[i]->update(x), suff[i]->update(x);
		}
	}
	return candidate.x;
}

void propagate( node_type el ) {
	node_type x,nx;
	for ( x= el; (nx= H.jump(x)) != null; x= nx ) {
		size_type i= H.which(x);
		const chain &ch= H.get_chain(x);
		h[nx].remove(1+pref[i]->query(0,ch.len()-1));
		pref[i]->update(x), suff[i]->update(x);
		h[nx].push(1+pref[i]->query(0,ch.len()-1));
	}
	size_type i= H.which(x);
	pref[i]->update(x), suff[i]->update(x);
}

void remove( node_type el ) {
	assert( color[el] == WHITE );
	color[el]^= 1;
	h[el].remove(state(el,0));
	propagate(el);
}

void insert( node_type el ) {
	assert( color[el] == BLACK );
	color[el]^= 1;
	h[el].push(state(el,0));
	propagate(el);
}

int main() {
	int i,j,k,ts,pi;
	node_type el,x,y,nx,px,ans;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&qr); ) {
		for ( G.init(n), x= 1; x < n; ++x )
			scanf("%d",&y), G.add_arcs(--y,x);
		for ( x= 0; x < n; color[x]= BLACK, h[x++].init() ) ;
		G.preprocess(T), H.init(&T), T.preprocess();
		for ( el= 0; el < n; insert(el++) ) ;
		for (;qr-- && 1==scanf("%d",&el);) {
			if ( color[--el] == WHITE ) {
				// query
				// remove
				ans= resolve_query(el);
				remove(el);
			}
			else {
				// insert
				// query
				insert(el);
				ans= resolve_query(el);
			}
			printf("%d\n",ans+1);
		}
	}
	return 0;
}

