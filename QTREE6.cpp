/*
 * QTREE6
 * TOPIC: Heavy-light decomposition, BIT, segment tree with segment-wise updates, lazy propagation, predecessor data structure
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define L(k) ((k)&((~(k))+1ULL))
#define left(x) ((x)<<1)
#define right(x) (1|left(x))
#define oo (1<<30)
#define MAXN (1<<MAXK)
#define MAXE (N<<1)
#define N MAXN
#define MAXK (17)
#define NONE (-1)
#define DBG 1
enum { WHITE, BLACK };
using namespace std;
typedef long long i64;

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return NONE;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

class BIT {
private:
	int *tr, K, n;
	int prefix( unsigned int i ) {
		int ans = 0;
		for ( ;i; ans += tr[i], i &= ~L(i) ) ;
		return ans;
	}
public:
	BIT( int n ) {
		for ( K = 0; (1<<K) <= n; ++K ) ;
		tr = new int[1<<K], this->n = n;
		memset(tr,0,sizeof tr);
	}
	int sum( unsigned int i, unsigned int j ) { return prefix(j)-(i==0?0:prefix(i-1)); }
	BIT( const int *a, int n ) : BIT(n) {
		for ( int i = 0; i < n; ++i )
			if ( a[i] ) 
				add(i+1,1);
	}
	void add( unsigned int i, int dt ) {
		for ( ;i < (1<<K); tr[i] += dt, i += L(i) ) ;
	}
	int predecessor( unsigned int x ) {
		if ( x == 0 || prefix(x-1) == 0 )
			return +oo;
		int low=0, high=x-1, mid, rnk = prefix(x-1);
		for (;low+1<high;prefix(mid=(low+high)>>1)>=rnk?(high=mid):(low=mid));
		assert( high >= 1 );
		return high;
	}
	~BIT() { delete tr; }
};

#if DBG
class ST {
private:
	const int *a;
	i64 *sum, *bucket;
	int m,n;
	inline void add_value( int v, int i, int j, i64 dt ) {
		bucket[v] += dt;
		sum[v] += dt;
	}
	inline void push_down( int v, int i, int j ) {
		if ( bucket[v] && i < j ) {
			int k = ((i+j)>>1);
			//assert( !(bucket[v] % (j-i+1)) );
			i64 dt = bucket[v]/(j-i+1);
			add_value(left(v),i,k,dt*(k-i+1)), add_value(right(v),k+1,j,dt*(j-k));
			bucket[v] = 0;
			//assert( sum[v] == sum[left(v)]+sum[right(v)] );
		}
	}
	inline void push_up( int v, int i, int j ) {
		if ( i < j ) 
			sum[v] = sum[left(v)]+sum[right(v)];
	}
	void update( int v, int i, int j, int qi, int qj, int val ) {
		if ( qi > j || qj < i ) return ;
		for ( int k = qi; k <= qj; sum[k++] += val ) ;
	}
	i64 query( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return 0;
		i64 ans = 0;
		for ( int k = qi; k <= qj; ans += sum[k++] ) ;
		return ans;
	}
	void build( int v, int i, int j ) {
		for ( int k = i; k <= j; ++k )
			sum[k] = a[k];
	}
public:
	ST( const int *a, int n ) {
		this->n = n, this->m = (n*3)+0x80, this->a = a;
		sum = new i64[m], bucket = new i64[m];
		build(1,0,n-1);
	}
	void update( int qi, int qj, int val ) {
		if ( qi < 0 || qi > qj || qj >= n ) return ;
		update(1,0,n-1,qi,qj,val);
	}
	void update_suffix( int qi, int val ) {
		if ( qi >= 0 && qi <= n-1 )
			update(1,0,n-1,qi,n-1,val);
	}
	void update_prefix( int qi, int val ) {
		if ( qi >= 0 )
			update(1,0,n-1,0,qi,val);
	}
	i64 query( int qi, int qj ) {
		if ( qi > qj ) return 0LL;
		return query(1,0,n-1,qi,qj);
	}
	~ST() { delete sum, delete bucket; }
};
#else
class ST {
private:
	const int *a;
	i64 *sum, *bucket;
	int m,n;
	inline void add_value( int v, int i, int j, i64 dt ) {
		bucket[v] += dt;
		sum[v] += dt;
	}
	inline void push_down( int v, int i, int j ) {
		if ( bucket[v] && i < j ) {
			int k = ((i+j)>>1);
			// assert( !(bucket[v] % (j-i+1)) );
			i64 dt = bucket[v]/(j-i+1);
			add_value(left(v),i,k,dt*(k-i+1)), add_value(right(v),k+1,j,dt*(j-k));
			bucket[v] = 0;
			//assert( sum[v] == sum[left(v)]+sum[right(v)] );
		}
	}
	inline void push_up( int v, int i, int j ) {
		if ( i < j ) 
			sum[v] = sum[left(v)]+sum[right(v)];
	}
	void update( int v, int i, int j, int qi, int qj, int val ) {
		if ( qi > j || qj < i ) return ;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) {
			add_value(v,i,j,(j-i+1LL)*val);
			return ;
		}
		int k = (i+j)>>1;
		update(left(v),i,k,qi,qj,val), update(right(v),k+1,j,qi,qj,val);
		push_up(v,i,j);
	}
	i64 query( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return 0;
		push_down(v,i,j);
		if ( qi <= i && j <= qj )
			return sum[v];
		int k = (i+j)>>1;
		i64 ans = query(left(v),i,k,qi,qj)+query(right(v),k+1,j,qi,qj);
		return ans;
	}
	void build( int v, int i, int j ) {
		bucket[v] = 0;
		if ( i == j ) {
			add_value(v,i,j,a[i]), bucket[v] = 0;
			return ;
		}
		int k = (i+j)>>1;
		build(left(v),i,k), build(right(v),k+1,j);
		push_up(v,i,j);
	}
public:
	ST( const int *a, int n ) {
		this->n = n, this->m = (n*5)+0x80, this->a = a;
		sum = new i64[m], bucket = new i64[m];
		build(1,0,n-1);
	}
	void update( int qi, int qj, int val ) {
		if ( qi < 0 || qi > qj || qj >= n ) return ;
		update(1,0,n-1,qi,qj,val);
	}
	void update_suffix( int qi, int val ) {
		if ( qi >= 0 && qi <= n-1 )
			update(1,0,n-1,qi,n-1,val);
	}
	void update_prefix( int qi, int val ) {
		if ( qi >= 0 )
			update(1,0,n-1,0,qi,val);
	}
	i64 query( int qi, int qj ) {
		if ( qi > qj ) return 0LL;
		return query(1,0,n-1,qi,qj);
	}
	~ST() { delete sum, delete bucket; }
};
#endif

class Graph {
private:
	int to[MAXE],last[MAXN],next[MAXE],E,n,
		seen[N] = {0},yes = 0,K,p[N],
		d[N],anc[N][MAXK],card[N],
		best_son[N],
		chain_id, which_chain[N], pos[N],
		*head[N],*tail[N],*ptr,chain[N];
	void add_edge( int x, int y ) {
		int i = E++, j = E++;
		to[i] = y, next[i] = last[x], last[x] = i;
		to[j] = x, next[j] = last[y], last[y] = j;
	}
	int dfs( int x ) {
		int i,y,k;
		assert( seen[x] != yes );
		seen[x] = yes, card[x] = 1, best_son[x] = NONE;
		for ( i = last[x]; i != NONE; i = next[i] )
			if ( seen[y = to[i]] != yes ) {
				for ( p[y] = i, anc[y][0] = x, k = 1; anc[y][k-1] != NONE; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
				d[y] = d[x]+1, card[x] += dfs(y);
				if ( best_son[x] == NONE || card[to[best_son[x]]] < card[y] )
					best_son[x] = i;
			}
		return card[x];
	}
	inline void put_into_chain( int x ) {
		pos[x] = ptr-head[chain_id];
		which_chain[*ptr++ = x] = chain_id;
		assert( *(head[chain_id]+pos[x]) == x );
	}
	void hld( int x, int edge_id ) {
		int i,y;
		assert( seen[x] != yes );
		seen[x] = yes;
		if ( edge_id == NONE )
			head[++chain_id] = ptr;
		put_into_chain(x);
		if ( (i = best_son[x]) != NONE ) hld(to[i],i);
		for ( i = last[x]; i != NONE; i = next[i] )
			if ( i != best_son[x] && seen[y = to[i]] != yes )
				hld(y,NONE);
	}
	int up( int x, unsigned int u ) {
		for ( int k = 0; u; u >>= 1, ++k )
			if ( u & 1 ) x = anc[x][k];
		return x;
	}
	int lca( int x, int y ) {
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

	BIT **B[2];
	ST **st[2];
	int actual_color[N];

	int find_closest_ancestor( int x, const int color, bool consider_itself ) {
		int t,ch,z;
		if ( x == NONE )
			return NONE;
		ch = which_chain[x];
		if ( (t = B[color][ch]->predecessor(pos[x]+1+(consider_itself?1:0))) == +oo )
			return find_closest_ancestor(anc[*head[ch]][0],color,true);
		return *(head[ch]+t-1);
		/*
		for ( z = anc[x][0]; z != NONE && actual_color[z] != color; z = anc[z][0] ) ;
		if ( z == NONE ) {
			if ( consider_itself && color == actual_color[x] )
				return x;
			return NONE;
		}
		if ( consider_itself && color == actual_color[x] )
			return x;
		return z;*/
	}

	int find_shallowest_ancestor_of_same_color( int x ) {
		int z,ch = which_chain[x], t = actual_color[x];
		assert( t == BLACK || t == WHITE );
		assert( B[t^1][ch]->sum(pos[x]+1,pos[x]+1) == 0 );
		if ( (z = find_closest_ancestor(x,t^1,false)) == NONE )
			return *chain;
		assert( actual_color[z]^actual_color[x] );
		z = up(x,d[x]-d[z]-1);
		return z;
		/*
		int z,pre = x;
		for ( z = anc[pre][0]; z != NONE && actual_color[z] == actual_color[x]; pre = z, z = anc[pre][0] ) ;
		return pre;*/
	}

public:
	bool read() {
		int i,j,k;
		if ( (n = getnum()) == NONE	|| !n ) 
			return false ;
		for ( E = 0, i = 0; i < n; last[i++] = NONE ) ;
		for ( k = 1; k < n; ++k ) {
			i = getnum(), j = getnum();
			add_edge(--i,--j);
		}
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( k = 0; k < K; anc[i][k++] = NONE ) ;
		return true ;
	}
	void preprocess() {
		++yes, dfs(0);
		++yes, chain_id = -1, ptr = chain, hld(0,NONE);
		for ( int i = 0; i < chain_id; ++i )
			tail[i] = head[i+1]-1;
		tail[chain_id] = ptr-1;
	}
	void problem_specific_initialisation() {
		int i,j,k,t;
		for ( t = WHITE; t <= BLACK; ++t ) {
			B[t] = new BIT* [chain_id+1];
			for ( i = 0; i <= chain_id; ++i ) {
				B[t][i] = new BIT(tail[i]-head[i]+1);
				if ( t == BLACK )
					for ( j = 1; j <= tail[i]-head[i]+1; ++j )
						B[t][i]->add(j,1);
			}
		}
		for ( t = WHITE; t <= BLACK; ++t ) {
			st[t] = new ST* [chain_id+1];
			for ( i = 0; i <= chain_id; ++i ) {
				int *a = (int *)malloc((tail[i]-head[i]+1)*sizeof *a);
				for ( int *qtr = head[i]; qtr <= tail[i]; ++qtr )
					a[qtr-head[i]] = t==BLACK?card[*qtr]:1;
				st[t][i] = new ST(a,tail[i]-head[i]+1);
			}
		}
		for ( i = 0; i < n; actual_color[i++] = BLACK ) ;
	}

	void toggle( int x ) {
		int t = actual_color[x], z, ch, val, u;

		z = find_closest_ancestor(x,t^1,false), val = -st[t][which_chain[x]]->query(pos[x],pos[x]);
		if ( z == NONE ) {
			ch = which_chain[x];
			st[t][ch]->update_prefix(pos[x]-1,val);
			for ( u = anc[*head[ch]][0]; u != NONE; ch=which_chain[u], st[t][ch]->update_prefix(pos[u],val), u = anc[*head[ch]][0] ) ;
		}
		else {
			ch = which_chain[x];
			if ( which_chain[x] == which_chain[z] ) {
				st[t][ch]->update(pos[z],pos[x]-1,val);
			}
			else {
				st[t][ch]->update_prefix(pos[x]-1,val);
				for ( u = anc[*head[ch]][0]; which_chain[u] != which_chain[z]; ch=which_chain[u], st[t][ch]->update_prefix(pos[u],val), u=anc[*head[ch]][0] ) ;
				ch = which_chain[z];
				st[t][ch]->update(pos[z],pos[u],val);
			}
		}

		z = find_closest_ancestor(x,t,false), val = st[t^1][which_chain[x]]->query(pos[x],pos[x]);
		if ( z == NONE ) {
			ch = which_chain[x];
			st[t^1][ch]->update_prefix(pos[x]-1,val);
			for ( u = anc[*head[ch]][0]; u != NONE; ch=which_chain[u], st[t^1][ch]->update_prefix(pos[u],val), u = anc[*head[ch]][0] ) ;
		}
		else {
			ch = which_chain[x];
			if ( which_chain[x] == which_chain[z] ) {
				st[t^1][ch]->update(pos[z],pos[x]-1,val);
			}
			else {
				st[t^1][ch]->update_prefix(pos[x]-1,val);
				for ( u = anc[*head[ch]][0]; which_chain[u] != which_chain[z]; ch=which_chain[u], st[t^1][ch]->update_prefix(pos[u],val), u=anc[*head[ch]][0] ) ;
				ch = which_chain[z];
				st[t^1][ch]->update(pos[z],pos[u],val);
			}
		}
		
		B[t][which_chain[x]]->add(pos[x]+1,-1), B[t^1][which_chain[x]]->add(pos[x]+1,1);
		actual_color[x] ^= 1;
	}

	i64 query( int x ) {
		int t = actual_color[x], z = find_shallowest_ancestor_of_same_color(x), ch = which_chain[x];
		//printf("%d %d, with colors %d %d\n",x+1,z+1,actual_color[x],actual_color[z]);
		if ( z == NONE ) 
			return st[t][ch]->query(pos[x],pos[x]);
		else {
			//printf("Here %d\n",pos[z]);
			return st[t][which_chain[z]]->query(pos[z],pos[z]);
		}
	}

	void check( int x, int t ) {
		printf("THIS %lld\n",st[t][which_chain[x]]->query(pos[x],pos[x]));
	}

	void summary() {
		return ;
		for ( int x = 0; x < n; ++x )
			printf("color(%d) = %d, ",x+1,actual_color[x]);
		puts("");
	}

} g;

int main() {
	int i,qr,comm;
	for (;g.read();) {
		g.preprocess(), g.problem_specific_initialisation();
		for ( qr = getnum(); qr--; comm = getnum(), i = getnum(), --i, g.summary(), (comm==0?printf("%lld\n",g.query(i)):(g.toggle(i),1)) );
	}
	return 0;
}

