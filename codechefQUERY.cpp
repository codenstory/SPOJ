/*
 * QUERY: Observing the Tree
 * TOPIC: persistent range trees, persistent arrays, rollbacks, heavy-path decomposition
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
#include <set>
#include <map>
#include <unordered_map>
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
#define tol (1e-13)
using namespace std;
typedef unsigned long long u64;
typedef long long value_type;
#define N (1<<17)
#define MAXK (20)
#define MAXE (N<<1)
typedef int node_type;
typedef int size_type;
#define null (-1)
#define root (0)
#define MAXQ (MAXE)
typedef long long value_type;

const value_type neutral_element= 0LL;
size_type tot,go_back;
bool is_being_changed;

class ctree {
	vector<node_type> child[N];
	node_type anc[N][MAXK];
	size_type n,K,d[N],tick,tin[N],tout[N];
	void dfs( const node_type x ) {
		size_type k;
		tin[x]= ++tick;
		for ( node_type y: child[x] ) {
			for ( anc[y][0]= x, k= 1; anc[y][k-1]!=null; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
			d[y]= d[x]+1, dfs(y);
		}
		tout[x]= ++tick;
	}
	node_type up( node_type x, size_type u ) const {
		for ( size_type k= 0; u; u>>= 1, ++k )
			if ( u&1 )
				x= anc[x][k];
		return x;
	}
public:
	bool is_ancestor( const node_type px, const node_type x ) const {
		return tin[px] <= tin[x] && tout[x] <= tout[px];
	}
	node_type lca( node_type x, node_type y ) const {
		if ( is_ancestor(x,y) ) return x;
		if ( is_ancestor(y,x) ) return y;
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
	node_type level_ancestor( const node_type x, const size_type k ) const { return up(x,k); }
	const vector<node_type> &children( const node_type x ) const { return child[x]; }
	void init( const size_type n ) {
		this->n= n, tick= -1;
		for ( node_type x= root; x < n; child[x++].clear() ) ;
		for ( d[root]= 0, K= 0; (1<<K) <= n; ++K ) ;
		for ( node_type x= root; x < n; ++x )
			for ( size_type k= 0; k < K; anc[x][k++]= null ) ;
	}
	ctree() {};
	void add_arc( const node_type px, const node_type x ) { child[px].push_back(x); }
	size_type size() const { return n; }
	void preprocess() { dfs(root); }
};

class cgraph {
	node_type to[MAXE];
	size_type last[N],next[MAXE],E,p[N],n;
	bool seen[N];
	void dfs( node_type x ) {
		seen[x]= true ;
		for ( size_type i= last[x]; i != null; i= next[i] )
			if ( !seen[to[i]] )
				p[to[i]]= i, dfs(to[i]);
	}
public:
	void add_arcs( const node_type x, const node_type y ) {
		auto i= E++, j= E++;
		to[i]= y, next[i]= last[x], last[x]= i;
		to[j]= x, next[j]= last[y], last[y]= j;
	}
	cgraph() {};
	void init( size_type n ) {
		this->n= n, E= 0;
		for ( node_type x= root; x < n; seen[x]= false, p[x]= null, last[x++]= null ) ;
	}
	void preprocess( ctree &t ) { 
		dfs(root);
		assert( p[root] == null );
		for ( node_type x= root+1; x < n; ++x ) {
			assert( p[x] != null );
			t.add_arc(to[p[x]^1],x);
		}
	}
};

class chain {
	mutable unordered_map<node_type,size_type> p;
	vector<node_type> c;
	size_type id;
public:
	node_type get( const size_type position ) const {
		assert( 0 <= position && position < c.size() );
		return c[position];
	}
	size_type pos( const node_type x ) const {
		assert( p.count(x) );
		return p[x];
	}
	size_type len() const { return c.size(); }
	void init() { c.clear(), p.clear(); }
	chain( size_type id ) : id(id) { init(); }
	void append( const node_type x ) { p[x]= c.size(), c.push_back(x); }
	size_type get_id() const { return id; }
};

inline value_type F( value_type A, value_type B, value_type n ) {
	return A*n + B*n*(n-1)/2;
}

class range_tree {
	size_type change_operation_id;
	struct cell {
		size_type i,j;
		value_type s,a,b;
		cell *son[2];
		cell() { i= 0, j= null, s= a= b= 0, son[0]= son[1]= NULL; };
		cell( const size_type i, const size_type j ) : i(i), j(j) { s= a= b= 0, son[0]= son[1]= NULL; }
		cell( const cell *other ) {
			this->i= other->i, this->j= other->j;
			this->a= other->a, this->b= other->b, this->s= other->s;
			this->son[0]= other->son[0], this->son[1]= other->son[1];
		}
	};
	cell *rt;
	cell *build( size_type i, size_type j ) {
		assert( i <= j );
		cell *x= new cell(i,j);
		if ( i == j ) 
			return x;
		auto k= (i+j)>>1;
		x->son[0]= build(i,k), x->son[1]= build(k+1,j);
		return x;
	}

	inline value_type f( const cell *x, const value_type A, const value_type B ) const {
		value_type n= x->j-x->i+1;
		return F(A,B,n);
	}
	inline value_type g( const cell *x ) const { return x->s+f(x,x->a,x->b); }

	value_type _query( const cell *x, const size_type qi, const size_type qj, 
					   value_type A, value_type B ) const {

		if ( qi > x->j || qj < x->i )
			return neutral_element;

		if ( qi <= x->i && x->j <= qj ) 
			return x->s+f(x,x->a+A+B*(x->i-qi),x->b+B);

		if ( x->i <= qi && qj <= x->j ) {
			A+= x->a+(qi-x->i)*x->b, B+= x->b;
			return _query(x->son[0],qi,qj,A,B)+_query(x->son[1],qi,qj,A,B);
		}

		if ( qi <= x->i && x->i <= qj ) {
			assert( qj < x->j );
			return _query(x,x->i,qj,A+B*(x->i-qi),B);
		}

		if ( x->i <= qi && qi <= x->j ) {
			assert( x->j < qj );
			return _query(x,qi,x->j,A,B);
		}
		
		assert( 0 );
	}

	void _update( cell **z, cell *x, const size_type qi, const size_type qj, 
					                 value_type a, value_type b ) {
		if ( !(qi > x->j || x->i > qj) ) {
			*z= new cell(x);
			if ( qi <= x->i && x->j <= qj ) {
				(*z)->a+= a+(x->i-qi)*b, (*z)->b+= b;
				return ;
			}
			if ( x->i <= qi && qj <= x->j ) {
				_update(&((*z)->son[0]),x->son[0],qi,qj,a,b);
				_update(&((*z)->son[1]),x->son[1],qi,qj,a,b);
				(*z)->s= g((*z)->son[0])+g((*z)->son[1]);
				return ;
			}
			if ( qi <= x->i && x->i <= qj ) {
				assert( qj < x->j );
				_update(z,x,x->i,qj,a+(x->i-qi)*b,b);
				return ;
			}
			if ( x->i <= qi && qi <= x->j ) {
				assert( x->j < qj );
				_update(z,x,qi,x->j,a,b);
				return ;
			}
		}
	}

	void clear( cell *x ) {
		if ( !x ) return ;
		clear(x->son[0]), clear(x->son[1]);
		delete x;
	}



public:
	range_tree( const size_type n, const size_type cid ) : change_operation_id(cid) {
		rt= (n!=null?build(0,n-1):NULL);
	}
	size_type get_time() const { return change_operation_id; }
	value_type query( const size_type qi, const size_type qj ) const {
		return _query(rt,qi,qj,0LL,0LL);
	}
	~range_tree() {clear(rt);}

	void update( range_tree &new_tree, const size_type qi, const size_type qj,
									   const value_type a, const value_type b ) {
		_update(&(new_tree.rt),rt,qi,qj,a,b);
	}
};

size_type timestamp;
ctree T;

class persistent_array {
	size_type n;
	struct cell {
		cell *son[2];
		value_type val;
		size_type i,j;
		cell( const cell *other ) {
			this->i= other->i, this->j= other->j, this->val= other->val;
			this->son[0]= other->son[0], this->son[1]= other->son[1];
		}
		cell( const size_type i, const size_type j ) : i(i), j(j) {
			son[0]= son[1]= NULL, val= 0LL;
		}
	};
	cell *rt= NULL;
	value_type _access( const cell *x, const size_type pos ) const {
		assert( x->i <= pos && pos <= x->j );
		if ( x->i == x->j )
			return x->val;
		auto k= (x->i+x->j)>>1;
		return _access(x->son[pos<=k?0:1],pos);
	}
	void _create_new( cell **z, const cell *x, const size_type pos, const value_type val ) const {
		assert( x );
		if ( x->i <= pos && pos <= x->j ) {
			*z= new cell(x);
			assert( x->i <= x->j );
			if ( x->i == x->j ) {
				assert( x->i == pos );
				(*z)->val= val;
				return ;
			}
			_create_new(&((*z)->son[0]),x->son[0],pos,val);
			_create_new(&((*z)->son[1]),x->son[1],pos,val);
		}
	}
	cell *build( size_type i, size_type j ) {
		cell *x= new cell(i,j);
		if ( i == j ) {
			x->val= 0;
			return x;
		}
		auto k= (i+j)>>1;
		x->son[0]= build(i,k), x->son[1]= build(k+1,j);
		return x;
	}
public:
	persistent_array() {}
	persistent_array( const size_type n ) : n(n) { rt= build(0,n-1); }
	value_type probe( const size_type pos ) const {
		assert( rt && rt->i <= pos && pos <= rt->j );
		return _access(rt,pos);
	}
	void create_new( persistent_array &new_array, const size_type pos, const value_type val ) const {
		assert( rt );
		_create_new(&(new_array.rt),rt,pos,val);
	}
};

vector<persistent_array *> arr;

class hpd {
	vector<chain> chains;
	vector<range_tree *> v[N];
	size_type n,which_chain[N],card[N];
	node_type best_son[N];
	const ctree *T;

	size_type dfs( node_type x ) {
		assert( !card[x] );
		card[x]= 1, best_son[x]= null;
		const vector<node_type> &child= T->children(x);
		for ( auto y: child ) {
			card[x]+= dfs(y);
			if ( best_son[x] == null || card[y] > card[best_son[x]] )
				best_son[x]= y;
		}
		return card[x];
	}

	void decompose( node_type x, node_type from ) {
		if ( from == null ) {
			size_type id= chains.size();
			chains.push_back(chain(id));
		}
		assert( chains.size() );
		auto sz= chains.size();
		chains[which_chain[x]= sz-1].append(x);

		const vector<node_type> &ch= T->children(x);
		if ( best_son[x] != null )
			decompose(best_son[x],x);
		for ( node_type y: ch )
			if ( y != best_son[x] )
				decompose(y,null);
	}
	void extract_path( vector<pair<size_type,pair<size_type,size_type>>> &path, node_type px, node_type x ) const {
		// "px" is ancestor of "x"
		assert( T->is_ancestor(px,x) );
		size_type id, pid= which(px);
		for ( path.clear(); (id= which(x)) != pid; ) {
			const chain &ch= get_chain(x);
			path.push_back({id,{0,ch.pos(x)}});
			x= T->level_ancestor(ch.get(0),1);
		}
		const chain &ch= get_chain(x);
		if ( ch.pos(px)+1 <= ch.pos(x) )
			path.push_back({pid,{ch.pos(px)+1,ch.pos(x)}});
	}

	/* t0 refers to the version of the entire data structure */
	size_type find_idx( const size_type id ) const {
		if ( is_being_changed && v[id].back()->get_time() == tot )
			return v[id].size()-1;
		/*
		if ( v[id].back()->get_time() > go_back )
			return v[id].size()-1;
			*/
		auto relevant_change_time= arr[timestamp]->probe(id); 
		assert( relevant_change_time <= timestamp );
		size_type low= 0, high= v[id].size()-1, mid;
		assert( v[id][high]->get_time() >= relevant_change_time );
		if ( v[id][high]->get_time() == relevant_change_time )
			return high;
		assert( v[id][low]->get_time() <= relevant_change_time );
		assert( v[id][high]->get_time() > relevant_change_time );
		for (;low+1 < high;) {
			mid= (low+high)>>1;
			if ( v[id][mid]->get_time() <= relevant_change_time )
				low= mid;
			else high= mid;
		}
		assert( v[id][low]->get_time() == relevant_change_time );
		assert( v[id][high]->get_time() > relevant_change_time );
		return low;
	}

public:
	size_type num_chains() const { return chains.size(); }
	void init( const ctree *T ) {
		n= (this->T= T)->size(), chains.clear();
		for ( node_type x= root; x < n; card[x++]= 0 ) ;
	}
	hpd() {}
	void preprocess() {
		dfs(root), decompose(root,null);
		for ( size_type i= 0; i < (size_type)chains.size(); ++i ) 
			v[i].clear(), v[i].push_back(new range_tree(chains[i].len(),0));
	}

	size_type which( const node_type x ) const { return which_chain[x]; }
	const chain &get_chain( const node_type x ) const { return chains[which(x)]; }

	void update( const size_type cid, const node_type x, const node_type y, 
				 const value_type a, const value_type b, vector<size_type> &cpath ) {
		node_type z= T->lca(x,y); cpath.clear();
		vector<pair<size_type,pair<size_type,size_type>>> x2z, y2z;
		extract_path(x2z,z,x), extract_path(y2z,z,y);
		value_type aa= a, bb= b;
		for ( vector<pair<size_type,pair<size_type,size_type>>>::iterator it= x2z.begin(); it != x2z.end(); ++it ) {
			size_type id= it->first; cpath.push_back(id);
			size_type ii= it->second.first, jj= it->second.second;
			size_type idx= find_idx(id);
			range_tree &rt= (v[id].push_back(new range_tree(null,cid)),*v[id].back());
			v[id][idx]->update(rt,ii,jj,aa+bb*(jj-ii),-bb), aa+= bb*(jj-ii+1);
		}
		size_type id= which(z); cpath.push_back(id);
		size_type ii= chains[id].pos(z), jj= ii;
		size_type idx= find_idx(id);
		range_tree &rt= (v[id].push_back(new range_tree(null,cid)),*v[id].back());
		v[id][idx]->update(rt,ii,jj,aa,bb), aa+= bb*(jj-ii+1);
		for ( vector<pair<size_type,pair<size_type,size_type>>>::reverse_iterator it= y2z.rbegin(); it != y2z.rend(); ++it ) {
			size_type id= it->first; cpath.push_back(id);
			size_type ii= it->second.first, jj= it->second.second;
			size_type idx= find_idx(id);
			range_tree &rt= (v[id].push_back(new range_tree(null,cid)),*v[id].back());
			v[id][idx]->update(rt,ii,jj,aa,bb), aa+= bb*(jj-ii+1);
		}
	}

	value_type query( const node_type x, const node_type y ) const {
		node_type z= T->lca(x,y);
		vector<pair<size_type,pair<size_type,size_type>>> x2z, y2z;
		extract_path(x2z,z,x), extract_path(y2z,z,y);

		value_type res= 0LL;
		for ( vector<pair<size_type,pair<size_type,size_type>>>::iterator it= x2z.begin(); it != x2z.end(); ++it ) {
			size_type id= it->first;
			size_type ii= it->second.first, jj= it->second.second;
			size_type idx= find_idx(id);
			res+= v[id][idx]->query(ii,jj);
		}
		size_type id= which(z);
		size_type ii= chains[id].pos(z), jj= ii;
		size_type idx= find_idx(id);
		res+= v[id][idx]->query(ii,jj);
		for ( vector<pair<size_type,pair<size_type,size_type>>>::reverse_iterator it= y2z.rbegin(); it != y2z.rend(); ++it ) {
			size_type id= it->first;
			size_type ii= it->second.first, jj= it->second.second;
			size_type idx= find_idx(id);
			res+= v[id][idx]->query(ii,jj);
		}
		return res;
	}
};

cgraph G;
hpd H;

void update_arrays( const vector<size_type> &cpath ) {
	arr.push_back(arr[timestamp]);
	assert( arr.size() == tot+1 ) ;
	for ( auto id: cpath ) {
		persistent_array *p= new persistent_array();
		arr[tot]->create_new(*p,id,tot), arr[tot]= p;
	}
}

int main() {
	int i,j,k,ts,n,qr,l;
	char tmp[0x10];
	node_type x,y;
	value_type a,b,lastans,x1,y1;
	vector<size_type> cpath;
	for (;2==scanf("%d %d",&n,&qr);) {
		for ( G.init(n), k= 0; k < n-1; scanf("%d %d",&x,&y), G.add_arcs(--x,--y), ++k ) ;
		T.init(n), G.preprocess(T), T.preprocess(), H.init(&T), H.preprocess();
		arr.clear(), arr.push_back(new persistent_array(H.num_chains())), go_back= null;
		for(is_being_changed=false,l=0,timestamp=0,lastans=0LL,tot=0;l<qr&&1==scanf("%s",tmp); ++l)
			switch ( 0[tmp] ) {
				case 'c': scanf("%lld %lld %lld %lld",&x1,&y1,&a,&b);
						  x= ((x1+lastans)%n);
						  y= ((y1+lastans)%n);
						  ++tot, is_being_changed= true ;
						  //printf("Updating path %d --> %d with %lld, %lld\n",x,y,a,b);
						  H.update(tot,x,y,a,b,cpath);
						  update_arrays(cpath);
						  timestamp= tot, is_being_changed= false ;
						  break ;
				case 'q': scanf("%lld %lld",&x1,&y1);
						  x= ((x1+lastans)%n);
						  y= ((y1+lastans)%n);
						  lastans= H.query(x,y);
						  //printf("Querying: %d --> %d\n",x,y);
						  printf("%lld\n",lastans);
						  break ;
				case 'l': scanf("%lld",&x1);
						  timestamp= (x1+lastans)%(tot+1);
						  //printf("Rolling back to %d\n",timestamp);
						  go_back= tot;
						  break ;
				default:  assert( 0 );
			}
		break ;
	}
	return 0;
}

