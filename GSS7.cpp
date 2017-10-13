/*
 * GSS7
 * TOPIC: heavy-light decomposition, lazy propagation segment tree
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define L(x) ((x)<<1)
#define R(x) (1|L(x))
#define oo (1LL<<30)
#define SH 19
#define N (1<<SH)
#define MAXE (N<<1)
#define NONE (-1)
#define MAXK (SH)
using namespace std;
typedef long long i64;	

struct bundle {
	i64 prefix, suffix, sum, bestsum;
	bundle() { prefix=suffix=sum=bestsum=0; }
	bundle( i64 p, i64 s, i64 sm, i64 bs ) : prefix(p), suffix(s), sum(sm), bestsum(bs) {};
	void update_with( const bundle &a, const bundle &b ) {
		if ( a.bestsum == -oo && b.bestsum == -oo ) 
			return ;
		if ( a.bestsum == -oo ) {
			bestsum = b.bestsum;
			prefix = b.prefix;
			suffix = b.suffix;
			sum = b.sum;
			return ;
		}
		if ( b.bestsum == -oo ) {
			bestsum = a.bestsum;
			prefix = a.prefix;
			suffix = a.suffix;
			sum = a.sum;
			return ;
		}

		bestsum = max(max(a.bestsum,b.bestsum),a.suffix+b.prefix);

		prefix = max(0LL,max(a.prefix,a.sum+b.prefix));
		suffix = max(0LL,max(b.suffix,b.sum+a.suffix));

		sum = a.sum+b.sum;
	}
	void reverse() {
		swap(prefix,suffix);
	}
};

class ST {
private:
	i64 *a;
	i64 *prefix,*suffix,*sum,*bestsum,*value;
	bool *updates_pending;
	int m,n;

	void assign_value( int v, int i, int j, i64 nv ) {
		assert( i <= j );
		updates_pending[v]=(i<j);
		if ( i == j ) assert( !updates_pending[v] );
		sum[v] = (j-i+1)*(value[v]=nv);
		bestsum[v]=suffix[v]=prefix[v]=max(0LL,sum[v]);
	}

	void push_up( int v, int i, int j ) {
		if ( i < j ) {
			sum[v] = sum[L(v)]+sum[R(v)];
			prefix[v] = max(0LL,max(prefix[L(v)],sum[L(v)]+prefix[R(v)]));
			suffix[v] = max(0LL,max(suffix[R(v)],sum[R(v)]+suffix[L(v)]));
			bestsum[v] = max(0LL,max(bestsum[L(v)],bestsum[R(v)]));
			bestsum[v] = max(bestsum[v],suffix[L(v)]+prefix[R(v)]);
		}
	}

	void init( int v, int i, int j ) {
		assert( i <= j );
		updates_pending[v] = false;
		if ( i == j ) {
			assign_value(v,i,j,a[i]);
			assert( !updates_pending[v] );
			return ;
		}
		int k = (i+j)>>1;
		init(L(v),i,k), init(R(v),k+1,j);
		push_up(v,i,j);
	}
	void push_down( int v, int i, int j ) {
		if ( !updates_pending[v] ) return ;
		assert( i < j );
		int k = (i+j)>>1;
		assign_value(L(v),i,k,value[v]), assign_value(R(v),k+1,j,value[v]);
		updates_pending[v] = false;
	}
	void update( int v, int i, int j, int qi, int qj, i64 newval ) {
		if ( qi == i && j == qj ) {
			assign_value(v,i,j,newval);
			return ;
		}
		//assert( i <= qi && qj <= j );
		int k = (i+j)>>1;
		push_down(v,i,j);

		if ( qj <= k ) update(L(v),i,k,qi,qj,newval);
		else if ( qi >= k+1 ) update(R(v),k+1,j,qi,qj,newval);
		else 
			update(L(v),i,k,qi,k,newval), update(R(v),k+1,j,k+1,qj,newval);
		push_up(v,i,j);
	}
	bundle query( int v, int i, int j, int qi, int qj ) {
		bundle res;
		int k = (i+j)>>1;
		//assert( i <= qi && qj <= j );
		if ( updates_pending[v] ) {
			//assert( 0 );
			bundle res;
			res.prefix = res.suffix = res.bestsum = max(0LL,res.sum=(qj-qi+1)*value[v]);
			return res;
		}
		push_down(v,i,j);
		if ( qi == i && j == qj ) 
			return bundle(prefix[v],suffix[v],sum[v],bestsum[v]);
		assert( i < j );
		if ( qj <= k )
			return query(L(v),i,k,qi,qj);
		else if ( qi >= k+1 )
			return query(R(v),k+1,j,qi,qj);
		else {
			bundle a = query(L(v),i,k,qi,k), b = query(R(v),k+1,j,k+1,qj);
			push_up(v,i,j);
			res.update_with(a,b);
			return res;
		}
	}
public:
	ST( i64 *a, int n ) {
		m = (n*6)+0x20;
		prefix = new i64[m];
		suffix = new i64[m];
		sum = new i64[m];
		bestsum = new i64[m];
		value = new i64[m];
		updates_pending = new bool[m];
		for ( int i = 0; i < m; updates_pending[i++] = false ) ;
		this->n = n;
		this->a = a;
		init(1,0,n-1);
	}
	~ST() {
		delete prefix;
		delete suffix;
		delete sum;
		delete value;
		delete bestsum;
		delete updates_pending;
	}
	void batch_assign( int qi, int qj, i64 newval ) { update(1,0,n-1,qi,qj,newval); }
	bundle query( int qi, int qj ) { 
		return query(1,0,n-1,qi,qj);
	}
};

class Graph {
private:
	int K,E,last[N],next[MAXE],to[MAXE],n,seen[N],yes,
		p[N],card[N],best_son[N],
		anc[N][MAXK],d[N],
		chain_id,chain[N],*head[N],*ptr,*tail[N],pos_in_chain[N],
		which_chain[N];
	ST *st[N];

	void add_arcs( int x, int y ) {
		int i = E++, j = E++;
		to[i] = y, next[i] = last[x], last[x] = i;
		to[j] = x, next[j] = last[y], last[y] = j;
	}
	i64 a[N];
	int dfs( int x ) {
		int i,y,k;
		assert( seen[x] != yes );
		for ( best_son[x] = NONE, seen[x] = yes, card[x] = 1, i = last[x]; i != NONE; i = next[i] )
			if ( seen[y = to[i]] != yes ) {
				for ( p[y] = i, anc[y][0] = x, k = 1; anc[y][k-1] != NONE; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
				d[y] = d[x]+1, card[x] += dfs(y);
				if ( best_son[x] == NONE || card[y] > card[to[best_son[x]]] )
					best_son[x] = i;
			}
		return card[x];
	}

	void put_into_chain( int x ) {
		pos_in_chain[x] = ptr-head[chain_id], which_chain[*ptr++ = x] = chain_id, tail[chain_id] = ptr-1;
	}

	void hld( int x, int edge_id ) {
		int i,y;
		assert( seen[x] != yes );
		seen[x] = yes;
		if ( (i=edge_id) == NONE ) 
			head[++chain_id] = ptr;

		put_into_chain(x);

		if ( best_son[x] != NONE ) 
			hld(to[best_son[x]],best_son[x]);

		for ( i = last[x]; i != NONE; i = next[i] )
			if ( seen[y = to[i]] != yes && i != best_son[x] )
				hld(y,NONE);
	}

	void populate_chains() {
		for ( int k,j,i = 0; i <= chain_id; ++i ) {
			k = tail[i]-head[i]+1;
			assert( k >= 1 );
			i64 *c = new i64[k];
			assert( c ) ;
			for ( int *p = head[i]; p <= tail[i]; ++p )
				c[p-head[i]] = a[*p];
			st[i] = new ST(c,k);
			/*
			printf("[%d:]",i);
			for ( int *p = head[i]; p <= tail[i]; ++p )
				printf(" %d",1+*p);
			puts("");*/
		}
	}

	int up( int x, unsigned int u ) {
		for ( int k = 0; u; u >>= 1, ++k )
			if ( u & 1 ) x = anc[x][k];
		assert( 0 <= x && x < n );
		return x;
	}

	int lca( int x, int y ) {
		assert( 0 <= x && x < n );
		assert( 0 <= y && y < n );
		if ( d[x] > d[y] )
			return lca(up(x,d[x]-d[y]),y);
		if ( d[x] < d[y] )
			return lca(y,x);
		if ( x == y )
			return x;
		for ( int k = K-1; k; --k ) {
			assert( anc[x][k] == anc[y][k] );
			if ( anc[x][k-1] != anc[y][k-1] )
				x = anc[x][k-1], y = anc[y][k-1];
		}
		return anc[x][0];
	}

	void _update( int ancestor, int x, i64 newval ) {
		if ( x == ancestor ) {
			ST *sx = st[which_chain[x]];
			sx->batch_assign(pos_in_chain[x],pos_in_chain[x],newval);
			return ;
		}
		for ( int h; x != NONE && d[ancestor] < d[x]; x = anc[h][0] ) {
			ST *sx = st[which_chain[x]];
			h = *head[which_chain[x]];
			if ( d[h] > d[ancestor] )
				sx->batch_assign(pos_in_chain[h],pos_in_chain[x],newval);
			else
				sx->batch_assign(pos_in_chain[ancestor]+1,pos_in_chain[x],newval);
		}
	}

	bundle _query( int ancestor, int x ) {
		bundle candidate,tmp,res;
		if ( ancestor == x ) {
			ST *sx = st[which_chain[x]];
			res = sx->query(pos_in_chain[x],pos_in_chain[x]);
			return res;
		}
		for ( int h; x != NONE && d[ancestor] < d[x]; x = anc[h][0] ) {
			ST *sx = st[which_chain[x]];
			h = *head[which_chain[x]];
			if ( d[h] > d[ancestor] )
				candidate = sx->query(pos_in_chain[h],pos_in_chain[x]);
			else 
				candidate = sx->query(pos_in_chain[ancestor]+1,pos_in_chain[x]);
			tmp = res, res.update_with(candidate,tmp);
		}
		return res;
	}

public:
	Graph() {
		memset(seen,0,sizeof seen), yes = 0;
	}
	bool read_graph() {
		int i,j,k;
		if ( 1 != scanf("%d",&n) )
			return false ;
		for ( E = 0, i = 0; i < n; last[i++] = NONE ) ;
		for ( i = 0; i < n; ++i )
			if ( 1 != scanf("%lld",&a[i]) )
				return false ;
		for ( k = 0; k < n-1; ++k ) {
			if ( 2 != scanf("%d %d",&i,&j) )
				return false ;
			add_arcs(--i,--j);
		}
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( k = 0; k < K; anc[i][k++] = NONE ) ;
		return true ;
	}
	void preprocess() {
		++yes, d[0] = 0, dfs(0);
		++yes, chain_id = NONE, ptr = chain, hld(0,NONE);
		populate_chains();
	}

	void update( int x, int y, i64 newval ) {
		assert( 0 <= min(x,y) && max(x,y) < n );
		if ( x == y ) {
			_update(x,x,newval);
			return ;
		}
		int z = lca(x,y);
		//printf("lca(%d,%d) = %d\n",x,y,z);
		_update(z,x,newval), _update(z,y,newval), _update(z,z,newval);
	}

	i64 query( int x, int y ) {
		assert( 0 <= min(x,y) && max(x,y) < n );
		int z = lca(x,y);
		//printf("lca(%d,%d) = %d\n",x,y,z);
		bundle a = _query(z,x), b = _query(z,y), c = _query(z,z), res, rx,ry, rrx, rry;
		if ( x == y ) 
			return c.bestsum;
		if ( z == x ) {
			//printf("%lld %lld %lld\n",c.sum,b.sum,b.prefix);
			return max(max(b.bestsum,c.bestsum),c.sum+b.prefix);
		}
		else if ( z == y ) {
			return max(max(a.bestsum,c.bestsum),c.sum+a.prefix);
		}
		else {
			rx.update_with(c,a);
			ry = b, ry.reverse();
			res.update_with(ry,rx);
			return max(0LL,res.bestsum);
		}
	}
} G;

int main() {
	int i,j,k,qr,comm,x,y,cs = 0,cas = 0;
	i64 newval;
	for ( ;G.read_graph(); ) {
		++cas;
		for ( G.preprocess(), scanf("%d",&qr); qr--; ) {
			scanf("%d %d %d",&comm,&x,&y), --x, --y;
			if ( comm == 2 ) {
				assert( 1 == scanf("%lld",&newval) );
				//printf("Coloring the path %d --> %d to %lld\n",x+1,y+1,newval);
				G.update(x,y,newval);
			}
			else {
				printf("%lld\n",G.query(x,y));
			}
		}
	}
	return 0;
}

