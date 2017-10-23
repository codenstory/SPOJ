/*
 * GEN2
 * TOPIC: AC automaton, matrix exponentiation
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
#include <queue>
#define MAXN (0x100)
#define SIGMA 26
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define TST(u,k) ((u) & BIT(k))
#define L(k) ((k)&((~(k))+1ULL))
#define MOD (8000)
typedef unsigned long long u64;
using namespace std;

char which[1<<21];
inline int who( unsigned int u ) { 
	if ( u >= BIT(21) ) return which[u>>21]+21;
	return which[u];
}

class AC {
private:
	struct cell {
		cell *slink, *son[SIGMA];
		int cnt;
		unsigned int u, mask;
	} pool[MAXN], *ptr, *q[MAXN], **head, **tail, *root, *q0;
	cell *init_cell() {
		cell *x = ptr++;
		x->u = 0, x->cnt = 0, x->mask = 0, x->slink = NULL;
		return x;
	}
	int id[1<<20], seen[MAXN][8], yes;
	unsigned int Q[MAXN*8],*h,*t;
public:
	void init() {
		ptr = pool, q0 = init_cell(), root = init_cell();
		for ( int ch = 0; ch < SIGMA; ++ch )
			q0->son[ch] = root, q0->u |= BIT(ch);
		root->slink = q0;
		memset(seen,0,sizeof seen), yes = 0;
	}
	AC() { init(); }
	void preprocess() {
		cell *x,*y,*z;
		int i,j,k,ch;
		unsigned int u;
		for ( head = tail = q, *tail++ = root; head < tail; ) 
			for ( u = (x = *head++)->u; u && (ch = who(L(u))) >= 0; u &= ~L(u) ) {
				for ( y=x->son[ch], z = x->slink; z && !(z->u & BIT(ch)); z = z->slink ) ;
				assert( z );
				y->slink = z->son[ch], y->mask |= (y->slink->mask), *tail++ = y;
			}
	}
	void push( const char *s, int idx ) {
		cell *x,*y;
		for ( x = root; *s && 'a' <= *s && *s <= 'z'; x = x->son[*s++-'a'] )
			if ( !(x->u & BIT(*s-'a')) )
				x->son[*s-'a'] = init_cell(), x->u |= BIT(*s-'a');
		x->mask |= BIT(idx);
	}
	void construct_matrix( int ***A, int *m, vector<int> &good_states ) {
		int n = 0, i,j,k,ch;
		unsigned int u,msk;
		cell *x,*y,*z;
#define ENCODE(x,m) (((x)-pool) | ((m) << 7))
		for ( ++yes, h = t = Q, x = pool+1; x < ptr; ++x ) 
			if ( (!(x->mask>>3)) )
				seen[x-pool][x->mask] = yes, id[*t++ = ENCODE(x,x->mask)] = n++;
		good_states.clear();
		for (;h<t;) {
			for ( x = ((u=*h)&127)+pool, msk = ((*h++)>>7),  ch = 0; ch < SIGMA; ++ch ) {
				for ( z = x; z && !(z->u&BIT(ch)); z = z->slink ) ;
				assert( z );
				y = z->son[ch];
				if ( y->mask >> 3 ) continue ;
				x->u |= BIT(ch), x->son[ch] = y;
				if ( seen[y-pool][y->mask|msk] != yes )
					seen[y-pool][y->mask|msk] = yes, id[*t++ = ENCODE(y,y->mask|msk)] = n++;
			}
			if ( msk == 7 ) good_states.push_back(id[u]);
		}

		(*A) = (int **)malloc(n*sizeof **A);
		for ( *m = n, i = 0; i < n; ++i )
			(*A)[i] = (int *)calloc(n,sizeof **A[i]);

		for ( ++yes, h = t = Q, x = pool+1; x < ptr; ++x ) 
			if ( (!(x->mask>>3)) )
				seen[x-pool][x->mask] = yes, *t++ = ENCODE(x,x->mask);
		for (;h<t;) 
			for ( x = ((*h)&127)+pool, msk = ((*h++)>>7), ch = 0; ch < SIGMA; ++ch ) {
				for ( z = x; z && !(z->u&BIT(ch)); z = z->slink ) ;
				assert( z );
				y = z->son[ch];
				if ( y->mask >> 3 ) continue ;
				x->u |= BIT(ch), x->son[ch] = y;
				if ( seen[y-pool][y->mask|msk] != yes )
					seen[y-pool][y->mask|msk] = yes, *t++ = ENCODE(y,y->mask|msk);
				++(*A)[id[ENCODE(x,msk)]][id[ENCODE(y,y->mask|msk)]];
			}
	}
} ac;

inline int f( int x, int y ) {
	int z = (x+y);
	if ( z >= MOD ) return z-MOD;
	return z;
}

inline int g( int x, int y ) {
	int z = x*y;
	if ( z >= MOD )
		return z%MOD;
	return z;
}

void matrix_power( int ***A, int n, int len ) {
	int ax[MAXN][MAXN],x[MAXN][MAXN],t[MAXN][MAXN],i,j,k;
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			ax[i][j] = (i==j?1:0), x[i][j] = (*A)[i][j];
	for ( ;len; len >>= 1 ) {
		if ( len & 1 ) {
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					for ( t[i][j] = 0, k = 0; k < n; ++k )
						t[i][j] = f(t[i][j],g(ax[i][k],x[k][j]));
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					ax[i][j] = t[i][j];
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( t[i][j] = 0, k = 0; k < n; ++k )
					t[i][j] = f(t[i][j],g(x[i][k],x[k][j]));
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				x[i][j] = t[i][j];
	}
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			(*A)[i][j] = ax[i][j];
}

int main() {
	int i,j,k,len,m,**A,ans,n;
	char s[0x20];
	vector<int> good_states;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i );
	for ( ;2 == scanf("%d %d",&len,&n); ) {
		for ( ac.init(), i = 3; i < n+3; ++i )
			scanf("%s",s), ac.push(s,i);
		ac.push("hl",0), ac.push("hj",1), ac.push("fgd",2);
		ac.preprocess(), ac.construct_matrix(&A,&m,good_states), matrix_power(&A,m,len);
		for ( ans = 0, i = 0; i < (int)good_states.size(); ++i )
			ans = f(ans,A[0][good_states[i]]);
		printf("%d\n",ans/8);
	}
	return 0;
}

