/*
 * van Emde Boas Tree implementation
 * status: Accepted
 * NOTES: thanks to Morris Yang
 */
#include <bits/stdc++.h>
#define oo (1<<30)
using namespace std;
#define likely(x)		__builtin_expect((x),1)
#define unlikely(x)		__builtin_expect((x),1)
#define BIT(k) 			(1ULL<<(k))
#define MASK(k) 		(BIT(k)-1ULL)

static inline char log2int( int x ) {
	return __builtin_clz((int)1) - __builtin_clz(x);
}
static inline void divHL_bit( int u, int x, int &H, int &L ) {
	int t = log2int(u)>>1;
	H=(x>>t), L=(x&MASK(t));
}
static inline int index_bit( int u, int H, int L ) {
	int t = log2int(u)>>1;
	return (H<<t)|L;
}
char *mem_pool = (char *)malloc(64 << 20);
struct node {
	int mn,mx,u;
	node *summary,**sub;
	static node *init( int logn ) {
		node *x = (node *)mem_pool;
		mem_pool += sizeof *x, x->build(logn);
		return x;
	}
	static node **init_sons( int n ) {
		node **x = (node **)mem_pool;
		mem_pool += n*sizeof *x;
		return x;
	}
	node( int logn ) { build(logn); }
	void build( int logn ) {
		mn = mx = -1;
		if ( logn <= 1 ) {
			u = 2, summary = NULL, sub = NULL;
			return ;
		}
		u = BIT(logn);
		int auxn = (logn+1)>>1;
		summary = init(auxn), sub = init_sons(1<<auxn);
		for ( int i = 0; i < (1<<auxn); ++i )
			sub[i] = init(logn>>1);
	}
	static inline bool just_one_element( node *u ) {
		return u->mn == u->mx && u->mn >= 0;
	}
	static inline bool is_empty( node *u ) { return u->mn == u->mx && u->mn == -1; }
};

class vEB {
private:
	static inline int _min( node *u ) { return u->mn; }
	static inline int _max( node *u ) { return u->mx; }
	node *root;
	static void _insert( node *u, int x ) {
		assert( u );
		if ( u->mn == x || u->mx == x ) return ;
		if ( u->mn == -1 ) {
			u->mn = u->mx = x;
			return ;
		}
		if ( u->mn == u->mx ) {
			u->mn = x<u->mn?x:u->mn;
			u->mx = x>u->mx?x:u->mx;
			assert( u->mn < u->mx );
			return ;
		}
		if ( x < u->mn )
			swap(x,u->mn);
		else if ( x > u->mx )
			swap(x,u->mx);
		int H,L;
		divHL_bit(u->u,x,H,L);
		_insert(u->sub[H],L);
		if ( node::just_one_element(u->sub[H]) )
			_insert(u->summary,H);
	}

	static void _erase( node *u, int x ) {
		if ( u->mn == u->mx ) {
			u->mn = u->mx = -1;
			return ;
		}
		//assert( u->mn < u->mx );
		if ( unlikely(u->u == 2) || node::is_empty(u->summary) ) {
			if ( x == u->mn )
				u->mn = u->mx;
			else if ( x == u->mx ) {
				//assert( x == u->mx );
				u->mx = u->mn;
			}
			return ;
		}
		if ( x == u->mn ) {
			int first = _min(u->summary);
			assert( first != -1 );
			u->mn = x = index_bit(u->u,first,_min(u->sub[first]));
		}
		else if ( x == u->mx ) {
			int last = _max(u->summary);
			assert( last != -1 );
			u->mx = x = index_bit(u->u,last,_max(u->sub[last]));
		}
		int H,L;
		divHL_bit(u->u,x,H,L);
		_erase(u->sub[H],L);
		if ( node::is_empty(u->sub[H]) ) 
			_erase(u->summary,H);
	}

	static int _predecessor( node *u, int x ) {
		if ( unlikely(u->u == 2) ) 
			return (x==1&&u->mn==0)?0:-oo;
		if ( u->mx != -1 && x > u->mx ) 
			return u->mx;
		int H,L, Hmm;
		divHL_bit(u->u,x,H,L);
		Hmm = _min(u->sub[H]);
		if ( Hmm != -1 && L > Hmm )
			return index_bit(u->u,H,_predecessor(u->sub[H],L));
		int prev_cluster = _predecessor(u->summary,H);
		if ( prev_cluster > -oo ) 
			return index_bit(u->u,prev_cluster,_max(u->sub[prev_cluster]));
		return (u->mn!=-1&&x>u->mn)?u->mn:-oo;
	}
	static int _successor( node *u, int x ) {
		if ( unlikely(u->u == 2) ) 
			return x==0&&u->mx==1?1:+oo;
		if ( u->mn != -1 && x < u->mn )
			return u->mn;
		int H,L,Hmx;
		divHL_bit(u->u,x,H,L);
		Hmx = _max(u->sub[H]);
		if ( Hmx != -1 && L < Hmx )
			return index_bit(u->u,H,_successor(u->sub[H],L));
		int next_cluster = _successor(u->summary,H);
		if ( next_cluster < +oo )
			return index_bit(u->u,next_cluster,_min(u->sub[next_cluster]));
		return (u->mx!=-1&&x<u->mx)?u->mx:+oo;
	}
	static int _count( node *u, int x ) {
		if ( x == u->mn || x == u->mx )
			return 1;
		if ( u->u == 2 || u->mn == u->mx )
			return 0;
		int H,L;
		divHL_bit(u->u,x,H,L);
		return _count(u->sub[H],L);
	}
public:
	void build( int n ) {
		int logn = log2int(n)+1;
		if ( !(n&(n-1)) ) ++logn;
		root = node::init(logn);
	}
	int min() { return _min(root); }
	int max() { return _max(root); }
	void insert( int x ) { _insert(root,x); }
	void erase( int x ) { _erase(root,x); }
	int successor( int x ) 	 { return _successor(root,x); 	}
	int predecessor( int x ) { return _predecessor(root,x); }
	int count( int x ) { return _count(root,x); }
};

namespace {
	inline int readchar() {
		const int N = 1048576;
		static char buf[N], *p = buf, *end = buf;
		if ( p == end ) {
			if ( (end = buf+fread(buf,1,N,stdin)) == buf )
				return EOF;
			p = buf;
		}
		return *p++;
	}
	inline int read_int( int *x ) {
		static char c, neg = 1;
		for ( ;(c = readchar()) != EOF && c < '-'; ) ;
		if ( c == EOF ) return 0;
		if ( c == '-' ) {
			neg = -1;
			c = readchar();
		}
		for ( *x = c-'0'; (c = readchar()) >= '0'; *x = ((*x)<<3)+((*x)<<1)+c-'0' ) ;
		*x *= neg;
		return 1;
	}
	class Print {
	public:
		static const int N = 1048576;
		char buf[N], *p, *end;
		Print() { p = buf, end = buf+N-1; }
		void print_int( int x, char padding ) {
			if ( x < 0 ) {
				*p++ = '-';
				if ( p == end ) 
					*p = '\0', printf("%s",buf), p = buf;
				*p++ = '1';
				if ( p == end ) 
					*p = '\0', printf("%s",buf), p = buf;
				*p++ = padding;
				if ( p == end )
					*p = '\0', printf("%s",buf), p = buf;
				return ;
			}
			static char stk[16];
			int idx = 0;
			stk[idx++] = padding;
			do {
				stk[idx++] = '0'+(x%10);
			} while ( x /= 10 );
			while ( idx )  {
				if ( p == end )
					*p = '\0', printf("%s",buf), p = buf;
				*p++ = stk[--idx];
			}
		}
		static inline void online_print_int( int x ) {
			static char ch[16];
			static int idx;
			idx = 0;
			do { ch[++idx] = x%10; } while ( x/= 10 );
			for ( ;idx; putchar(ch[idx--]+48) ) ;
		}
		~Print() {
			*p = '\0', printf("%s",buf);
		}
	} bprint;
};

vEB tree;

int main() {
	int n, m, cmd, x, k = 0;
	read_int(&n), read_int(&m);
	tree.build(n);
	while (m--) {
		++k;
		read_int(&cmd);
		if (cmd == 1) {
			read_int(&x);
			if (!tree.count(x))
				tree.insert(x);
		} else if (cmd == 2) {
			read_int(&x);
			if (tree.count(x))
				tree.erase(x);
		} else if (cmd == 3) {
			bprint.print_int(tree.min(),'\n');
		} else if (cmd == 4) {
			bprint.print_int(tree.max(),'\n');
		} else if (cmd == 5) {
			read_int(&x);
			int res = tree.predecessor(x);
			bprint.print_int(res>-oo?res:-1,'\n');
		} else if (cmd == 6) {
			read_int(&x);
			int res = tree.successor(x);
			bprint.print_int(res<+oo?res:-1,'\n');
		} else if (cmd == 7) {
			read_int(&x);
			bprint.print_int(tree.count(x)?1:-1,'\n');
		}
	}
	return 0;
}

