/*
 * TOPIC: path counting queries, heavy-path decomposition, hpd with explicit stack
 * status: Accepted
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <unordered_map>
#define N (BIT(MAXK))
#define MAXE (N<<1)
#pragma comment(linker,"/STACK:16777216")
#define MAXK (23)
using namespace std;
typedef int node_type;
typedef int size_type;
typedef int value_type;
#define null (-1)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
#define A 8
#define Q (1<<MAXK)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define LOG (20)
typedef unsigned long long u64;
typedef long long i64;
#define nil (0)

u64 enc( u64 x, u64 y, u64 z ) { return x | (y<<LOG) | (z << (LOG+LOG)); }
node_type root;

size_type margin[N];
inline size_type border( size_type id ) {
	return id?margin[id-1]:0;
}

class color_matcher {
	string s[A]= {"alphalt","red","orange","yellow","green","blue","indigo","violet"};
	mutable unordered_map<string,value_type> mp;
	i64 cnt[A];
public:
	void init() { 
		for ( int i= 0; i < A; ++i )
			cnt[mp[s[i]]= i]= 0;
	}
	color_matcher() { init(); }
	value_type color2int( const string t ) const {
		assert( mp.count(t) );
		return mp[t];
	}
	void inc( const value_type x, const size_type dx ) { cnt[x]+= dx; }
	i64 operator [] ( const value_type x ) const { return cnt[x]; }
	string int2str( const value_type i ) const { return s[i]; }
	value_type str2int( const string t ) const { return mp[t]; }
};

/*
struct summary {
	size_type *cnt;
	summary() : cnt(new size_type[A]) {
		for ( value_type x= 0; x < A; cnt[x++]= 0 ) ;
	}
	~summary() noexcept { delete[] cnt; }
	size_type operator [] ( const int i ) const { 
		return cnt[i];
	}
	void assign( value_type val, size_type c ) {
		for ( value_type x= 0; x < A; cnt[x++]= 0 ) ;
		cnt[val]= c;
	}
	summary &operator += ( const summary &other ) {
		for ( value_type i= 0; i < A; ++i )
			cnt[i]+= other[i];
		return *this;
	}
	summary ( const summary &other ) : cnt(new value_type[A]) {
		std::memcpy(cnt,other.cnt,A*sizeof *cnt);
	}
	summary( summary &&that ) noexcept : cnt(that.cnt) {
		that.cnt= nullptr;
	}
	summary &operator = ( const summary &other ) {
		summary tmp(other);
		*this= std::move(tmp);
		return *this;
	}
	summary &operator = ( summary &&other ) noexcept {
		if ( this == &other )
			return *this;
		delete []cnt;
		cnt= other.cnt;
		other.cnt= nullptr;
		return *this;
	}
};
*/
struct summary {
	value_type cnt[A];
	void copy( summary &other ) {
		for ( size_type i= 0; i < A; cnt[i]= other[i], ++i ) ;
	}
	summary() {
		memset(cnt,0,sizeof cnt);
	}
	size_type operator [] ( const int i ) const { 
		return cnt[i];
	}
	void assign( value_type val, size_type c ) {
		memset(cnt,0,sizeof cnt);
		cnt[val]= c;
	}
	summary &operator += ( const summary &other ) {
		for ( value_type i= 0; i < A; ++i )
			cnt[i]+= other[i];
		return *this;
	}
	/*
	summary ( const summary &other ) : cnt(new value_type[A]) {
		std::memcpy(cnt,other.cnt,A*sizeof *cnt);
	}
	summary( summary &&that ) noexcept : cnt(that.cnt) {
		that.cnt= nullptr;
	}
	summary &operator = ( const summary &other ) {
		summary tmp(other);
		*this= std::move(tmp);
		return *this;
	}
	summary &operator = ( summary &&other ) noexcept {
		if ( this == &other )
			return *this;
		delete []cnt;
		cnt= other.cnt;
		other.cnt= nullptr;
		return *this;
	}
	*/
};


const summary zero_summary;

summary operator + ( const summary &a, const summary &b ) {
	summary s;
	for ( size_type i= 0; i < A; ++i )
		s.cnt[i]= a[i]+b[i];
	return s;
}

size_type SZ[N],lleft[N],cur[N];
node_type storage[N];
u64 stack_frames[N],*top;

class ctree {
	size_type n,K,d[N],tin[N],tout[N],tick;
	node_type anc[N][MAXK],*adj[N];
	bool seen[N];

	void dfs() {
		for ( top= stack_frames, d[root]= 0, seen[root]= true, *++top= enc(root,0,nil); top > stack_frames; ) {
			u64 u= *top--;
			node_type x= (u&MASK(LOG));
			size_type i= (u>>LOG)&MASK(LOG);
			pair<const node_type *,size_type> r= children(x);
			if ( i == r.second ) continue ;
			node_type y= r.first[i++];
			assert( !seen[y] );
			d[y]= d[anc[y][0]= x]+1, seen[y]= true ;
			for ( size_type k= 1; anc[y][k-1] != null; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
			*++top= enc(x,i,nil), *++top= enc(y,0,nil);
		}
	}

	node_type up( node_type x, size_type u ) const {
		for ( size_type k= 0; u; u>>= 1, ++k )
			if ( u&1 )
				x= anc[x][k];
		return x;
	}
public:
	void reserve( node_type x, size_type k ) {
		SZ[x]= (x?SZ[x-1]:0)+k;
		lleft[x]= x?SZ[x-1]:0;
		adj[x]= storage+lleft[x];
		cur[x]= 0;
	}
	size_type size() const { return n; }
	size_type depth( node_type x ) {
		return d[x];
	}
	node_type lca( node_type x, node_type y ) const { 
		if ( d[x] > d[y] )
			return lca(up(x,d[x]-d[y]),y);
		if ( d[y] > d[x] )
			return lca(y,x);
		if ( x == y ) 
			return x;
		for ( size_type k= K-1; k; --k ) {
			assert( anc[x][k] == anc[y][k] ) ;
			if ( anc[x][k-1] != anc[y][k-1] )
				x= anc[x][k-1], y= anc[y][k-1];
		}
		return anc[x][0];
	}
	node_type level_anc( node_type x, size_type k ) const {
		return up(x,k);
	}
	pair<const node_type *,size_type> children( const node_type x ) const {
		pair<node_type *,size_type> r= {adj[x],x?SZ[x]-SZ[x-1]:SZ[x]};
		return r;
	}
	void add_arc( node_type px, node_type x ) {
		adj[px][cur[px]++]= x;
	}
	void init( size_type n ) {
		for ( this->n= n, K= 0; (1 << K) <= n; ++K ) ;
		for ( node_type x= 0; x < n; seen[x++]= false )
			for ( size_type k= 0; k < K; anc[x][k++]= null ) ;
	}
	void preprocess() { dfs(); }
};

node_type q[N],*head,*tail;

class cgraph {
	node_type to[MAXE];
	size_type n,E,last[N],next[MAXE],p[N],card[N];
	bool seen[N];
	/*
	void dfs( node_type x ) {
		assert( 0 <= x && x < n );
		assert( !seen[x] ); seen[x]= true ;
		for ( size_type i= last[x]; i != null; i= next[i] )
			if ( !seen[to[i]] )
				p[to[i]]= i, dfs(to[i]);
	}
	*/
	void bfs_orient() {
		for ( head= tail= q, seen[*tail++= root]= true; head < tail; ) {
			node_type x= *head++;
			for ( size_type i= last[x]; i != null; i= next[i] ) {
				assert( 0 <= to[i] && to[i] < n );
				if ( !seen[to[i]] )
					p[*tail++= to[i]]= i, seen[to[i]]= true ;
			}
		}
	}
public:
	void add_edge( node_type x, node_type y ) {
		size_type i= E++, j= E++;
		assert( j < MAXE );
		assert( 0 <= min(x,y) && max(x,y) < n );
		to[i]= y, next[i]= last[x], last[x]= i;
		to[j]= x, next[j]= last[y], last[y]= j;
	}
	void init( size_type n ) {
		this->n= n, E= 0;
		for ( node_type x= 0; x < n; p[x]= null, seen[x]= false, last[x++]= null ) ;
	}
	void preprocess( ctree &t ) {
		bfs_orient(), t.init(n);
		memset(card,0,sizeof card);
		assert( p[root] == null );
		for ( node_type x= 0; x < n; ++x ) {
			if ( x == root ) continue ;
			assert( p[x] != null );
			++card[to[p[x]^1]];
		}
		for ( node_type x= 0; x < n; ++x )
			t.reserve(x,card[x]);
		for ( node_type x= 0; x < n; ++x ) 
			if ( x != root ) {
				//printf("adding arc %d --> %d\n",to[p[x]^1],x);
				t.add_arc(to[p[x]^1],x);
			}
		t.preprocess();
	}
};

node_type global_chain[N];
size_type global_pos[N],llen[N],_left[N];

size_type offset( size_type id ) { return id?_left[id-1]:0; }

class chain {
public:
	size_type id;
	void append( node_type x ) {
		global_pos[x]= llen[id]++, global_chain[_left[id]++]= x;
	}
	size_type node2pos( node_type x ) const { 
		return global_pos[x];
	}
	node_type pos2node( size_type pos ) const {
		return global_chain[pos+offset(id)];
	}
	size_type len() const { return llen[id]; }
	chain() {};
	chain( size_type id ) : id(id) { llen[id]= 0, _left[id]= offset(id); };
};

summary global_tr[Q];
bool global_flag[Q];
value_type global_h[Q];

class range_tree {
	const chain *c;
	const ctree *t;
	summary *tr;
	value_type *h;
	bool *flag;
	size_type n,sz;

	void push_down( size_type v, size_type i, size_type j ) {
		if ( !flag[v] ) return ;
		if ( i < j ) {
			flag[L(v)]= flag[R(v)]= true ;
			h[L(v)]= h[R(v)]= h[v];
		}
		assert( 1 <= h[v] && h[v] < A );
		tr[v].assign(h[v],j-i+1), flag[v]= false ;
	}

	void push_up( size_type v, size_type i, size_type j ) {
		if ( i < j ) {
			tr[v].copy(tr[L(v)]);
			tr[v]+= tr[R(v)];
		}
	}

	void _update( size_type v, size_type i, size_type j, 
				  size_type qi, size_type qj, value_type col ) {
		push_down(v,i,j);
		if ( qi > j || qj < i ) return ;
		if ( qi <= i && j <= qj ) {
			flag[v]= true, h[v]= col, push_down(v,i,j);
			return ;
		}
		auto k= (i+j)>>1;
		_update(L(v),i,k,qi,qj,col), _update(R(v),k+1,j,qi,qj,col);
		push_up(v,i,j);
	}

	void _query( size_type v, size_type i, size_type j, size_type qi, size_type qj, summary &acc ) {
		push_down(v,i,j);
		if ( qi > j || qj < i )
			return ;
		if ( qi <= i && j <= qj ) {
			acc+= tr[v];
			return ;
		}
		auto k= (i+j)>>1;
		_query(L(v),i,k,qi,qj,acc), _query(R(v),k+1,j,qi,qj,acc);
		push_up(v,i,j);
	}

	void build( size_type v, size_type i, size_type j ) {
		tr[v].assign(0,j-i+1), flag[v]= false ;
		if ( i < j ) {
			auto k= (i+j)>>1;
			build(L(v),i,k), build(R(v),k+1,j);
		}
	}

	void calc_size( size_type v, size_type i, size_type j ) {
		if ( i > j ) return ;
		if ( sz < v ) sz= v;
		if ( i == j ) 
			return ;
		auto k= (i+j)>>1;
		assert( i < j );
		calc_size(L(v),i,k),calc_size(R(v),k+1,j);
	}

public:
	range_tree( const chain *c, const ctree *t ) : c(c), t(t) {
		sz= 0, calc_size(1,0,c->len()-1);
		tr= global_tr+border(c->id);
		h= global_h+border(c->id);
		flag= global_flag+border(c->id);
		margin[c->id]= border(c->id)+sz+1;
		n= c->len();
		assert( n >= 1 );
		build(1,0,n-1);
	}
	void update( size_type qi, size_type qj, value_type col ) {
		_update(1,0,n-1,qi,qj,col);
	}
	summary query( size_type qi, size_type qj ) {
		summary res;
		_query(1,0,n-1,qi,qj,res);
		return res;
	}
};

class hpd {
	chain chains[N];
	vector<range_tree *> rt;
	size_type which_chain[N], card[N], chain_id, chlen;
	node_type best[N];
	const ctree *t;
	bool in_chain[N];

	void calc_subtree_sizes() {
		for ( top= stack_frames, *++top= enc(root,0,nil); top > stack_frames; ) {
			u64 u= *top--;
			node_type x= (u&MASK(LOG));
			size_type i= (u>>LOG)&MASK(LOG);
			auto r= t->children(x);
			if ( i == r.second ) {
				for ( size_type j= 0; j < r.second; ++j ) {
					card[x]+= card[r.first[j]];
					if ( best[x] == null || card[r.first[j]] > card[best[x]] )
						best[x]= r.first[j];
				}
				//printf("best[%d]= %d\n",x,best[x]);
			}
			else {
				assert( i < r.second );
				node_type y= r.first[i++];
				assert( !card[y] );
				*++top= enc(x,i,nil), ++card[y], *++top= enc(y,0,nil);
			}
		}
		for ( node_type x= 0; x < t->size(); ++x ) {
			auto r= t->children(x);
			if ( !(best[x] != null || !r.second) ) {
				printf("This %d %d\n",x,r.second);
			}
			assert( best[x] != null || !r.second );
		}
	}

	void hld() {
		for ( top= stack_frames, *++top= enc(root,0,t->size()); top > stack_frames; ) {
			u64 u= *top--;
			node_type x= (u&MASK(LOG));
			size_type i= (u>>LOG)&MASK(LOG);
			node_type from= (u>>(LOG+LOG));
			if ( from == t->size() ) 
				chains[chlen++]= chain(++chain_id);
			if ( !in_chain[x] ) {
				chains[which_chain[x]= chlen-1].append(x), in_chain[x]= true ;
				*++top= enc(x,0,nil);
				if ( best[x] != null ) 
					*++top= enc(best[x],0,nil);
				continue ;
			}
			pair<const node_type *,size_type> r= t->children(x);
			assert( best[x] != null || !r.second );
			if ( i < r.second ) {
				node_type y= r.first[i++];
				*++top= enc(x,i,nil);
				if ( y != best[x] ) {
					assert( !in_chain[y] );
					*++top= enc(y,0,t->size());
				}
			}
		}
	}

	size_type which( const node_type x ) const { return which_chain[x]; }
	const chain &get_chain( const node_type x ) { return chains[which(x)]; }

	u64 cpath[N];

	void extract_path( u64 *cpath, size_type &nn, node_type px, node_type x ) {
		size_type pid= which(px), id;
		for ( nn= 0; (id= which(x)) != pid; ) {
			const chain &ch= get_chain(x);
			id= which(x);
			assert( nn < N );
			cpath[nn++]= enc(id,0,ch.node2pos(x));
			assert( ch.pos2node(0) != root );
			x= t->level_anc(ch.pos2node(0),1);
			assert( x != null );
		}
		const chain &ch= get_chain(x);
		id= which(x);
		assert( id == pid );
		if ( ch.node2pos(px)+1 <= ch.node2pos(x) ) {
			assert( nn < N );
			cpath[nn++]= enc(id,ch.node2pos(px)+1,ch.node2pos(x));
		}
	}

public:

	void init( const ctree *t ) {
		this->t= t, chlen= 0, chain_id= null;
		for ( node_type x= 0; x < t->size(); best[x]= null, in_chain[x]= false, card[x++]= 0 ) ;
		/*
		if ( rt.size() )
			for ( size_type i= 0; i < (size_type)rt.size(); delete rt[i++] ) ;*/
		rt.clear(), calc_subtree_sizes(), hld();
		for ( size_type i= 0; i < chlen; ++i ) {
			assert( chains[i].len() >= 1 );
			rt.push_back(new range_tree(&chains[i],t));
		}
	}

	void update( node_type x, node_type y, value_type new_color ) {
		node_type z= t->lca(x,y);
		size_type nn;
		extract_path(cpath,nn,z,x);
		for ( size_type i= 0; i < nn; ++i ) {
			auto id= cpath[i]&MASK(LOG);
			auto qi= (cpath[i]>>LOG)&MASK(LOG), qj= (cpath[i]>>(LOG+LOG));
			rt[id]->update(qi,qj,new_color);
		}
		extract_path(cpath,nn,z,y);
		for ( size_type i= 0; i < nn; ++i ) {
			auto id= cpath[i]&MASK(LOG);
			auto qi= (cpath[i]>>LOG)&MASK(LOG), qj= (cpath[i]>>(LOG+LOG));
			rt[id]->update(qi,qj,new_color);
		}
	}

	summary query( node_type x, node_type y ) {
		summary res;
		size_type nn;
		node_type z= t->lca(x,y);
		extract_path(cpath,nn,z,x);
		for ( size_type i= 0; i < nn; ++i ) {
			auto id= cpath[i]&MASK(LOG);
			auto qi= (cpath[i]>>LOG)&MASK(LOG), qj= (cpath[i]>>(LOG+LOG));
			res+= rt[id]->query(qi,qj);
		}
		extract_path(cpath,nn,z,y);
		for ( size_type i= 0; i < nn; ++i ) {
			auto id= cpath[i]&MASK(LOG);
			auto qi= (cpath[i]>>LOG)&MASK(LOG), qj= (cpath[i]>>(LOG+LOG));
			res+= rt[id]->query(qi,qj);
		}
		return res;
	}
};

cgraph G;
ctree T;
hpd H;
color_matcher cm;

int main() {
	size_type i,j,k,ts,cs= 0,n,qr,oqr;
	node_type x,y;
	char tmp[0x10];
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( G.init(n), k= 0; k++ < n-1 && 2 == scanf("%d %d",&x,&y); G.add_edge(x,y) ) ;
		//printf("N= %d\n",n);
		//root= rand()%n;
		root= 0;
		//printf("root= %d\n",root);
		T.init(n), G.preprocess(T), H.init(&T);
		for ( cm.init(), scanf("%d",&qr), oqr= qr, k= 0; qr-- && 3 == scanf("%d %d %s",&x,&y,tmp); ++k ) {
			string str(tmp);
			//puts("Going to query");
			summary summ= H.query(x,y);
			//puts("query done");
			value_type new_color= cm.str2int(str);
			assert( 1 <= new_color && new_color < A );
			for ( value_type i= 0; i < A; ++i ) {
				//printf("On the path %x --> %d there are %d of color %s\n",x,y,summ[i],cm.int2str(i).c_str());
				if ( i != new_color )
					cm.inc(new_color,summ[i]);
			}
			//puts("Going to update");
			H.update(x,y,new_color);
			//puts("Update done");
		}
		for ( value_type i= 1; i < A; ++i )
			printf("%s %lld\n",cm.int2str(i).c_str(),cm[i]);
		if ( ts ) putchar('\n');
	}
	return 0;
}

