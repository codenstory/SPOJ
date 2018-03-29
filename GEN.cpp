/*
 * GEN
 * TOPIC: AC-automaton, matrix expo
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N 0x10
#define SIGMA (26)
#define BIT(k) (1ULL<<(k))
#define L(k) ((k) & ((~(k))+1ULL))
#define MAXN (0x80)
#define MOD (10007LL)
using namespace std;
typedef int i64;

char which[1<<21];

int who( unsigned int u ) {
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

i64 f( i64 x, i64 y ) {
	i64 z = x+y;
	if ( z >= MOD )
		return z%MOD;
	if ( z < 0 ) {
		for ( ;z < 0; z += MOD ) ;
		return z;
	}
	return z;
}

class AC {

private:

	struct cell {
		cell *son[SIGMA], *slink;
		unsigned int u;
		bool is_accepting;
		cell() {};
	} pool[MAXN], *ptr, *q[MAXN], **head, **tail, *root, *q0;
	cell *init_cell() {
		cell *x = ptr++;
		x->slink = NULL, x->u = 0, x->is_accepting = false ;
		return x;
	}
	int id[MAXN];

public:

	void preprocess() {
		cell *x,*y,*z;
		unsigned int u,ch;
		for ( head = tail = q, *tail++ = root; head < tail; ) {
			for ( u = (x = *head++)->u; u && (ch = who(L(u))) >= 0; *tail++ = y, u &= ~L(u) ) {
				for ( y=x->son[ch], z = x->slink; z && !(z->u & BIT(ch)); z = z->slink ) ;
				assert( z );
				y->slink = z->son[ch], y->is_accepting |= y->slink->is_accepting;
			}
		}
	}

	void init() {
		ptr = pool, q0 = init_cell(), root = init_cell(), root->slink = q0;
		for ( int ch = 0; ch < SIGMA; ++ch )
			q0->u |= BIT(ch), q0->son[ch] = root;
	}
	AC() { init(); }
	void push( char *s ) {
		cell *x,*y;
		for ( x = root; *s && isupper(*s); x = x->son[*s++-'A'] )
			if ( !(x->u & BIT(*s-'A')) )
				x->u |= BIT(*s-'A'), x->son[*s-'A'] = init_cell();
		x->is_accepting = true ;
	}
	void construct_matrix( i64 ***A, int *n ) {
		int i,j,k,m = ptr-pool,ch;
		cell *x,*y,*z;


		for ( m = 0, x = pool+1; x < ptr; ++x )
			if ( !x->is_accepting )
				id[x-pool] = m++;

		*n = m, *A = (i64 **)malloc(m*sizeof **A);
		for ( i = 0; i < m; ++i )
			(*A)[i] = (i64 *)calloc(m,sizeof **A[i]);

		for ( x = pool+1; x < ptr; ++x ) {
			if ( x->is_accepting ) continue ;
			for ( ch = 0; ch < SIGMA; ++ch ) {
				for ( z = x; !(z->u & BIT(ch)); z = z->slink ) ;
				y = z->son[ch];
				if ( y->is_accepting ) continue ;
				++(*A)[id[x-pool]][id[y-pool]];
			}
		}
	}
} ac;

i64 **A;
int n;

i64 matrixpow( i64 len ) {
	int i,j,k;
	i64 ax[MAXN][MAXN],x[MAXN][MAXN],tmp[MAXN][MAXN], ans = 0, AX = 1LL, X = SIGMA;

	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			x[i][j] = A[i][j], ax[i][j] = (i==j?1:0);

	for ( ;len; len >>= 1 ) {
		if ( len & 1 ) {
			AX *= X, AX %= MOD;
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					for ( tmp[i][j] = 0, k = 0; k < n; ++k )
						tmp[i][j] = f(tmp[i][j],(ax[i][k]*x[k][j]));
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					ax[i][j] = tmp[i][j];
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( tmp[i][j] = 0, k = 0; k < n; ++k )
					tmp[i][j] = f(tmp[i][j],(x[i][k]*x[k][j]));
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				x[i][j] = tmp[i][j];
		X *= X, X %= MOD;
	}
	for ( i = 0; i < n; ++i )
		ans = f(ans,ax[0][i]);
	return (AX-ans+MOD)%MOD;
}

int main() {
	int i,j,k;
	i64 len;
	char s[0x10];
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	for ( ;2 == scanf("%d %d",&n,&len); ) {
		for ( ac.init(), i = 0; i < n; ++i ) 
			scanf("%s",s), ac.push(s);
		ac.preprocess();
		ac.construct_matrix(&A,&n);
		printf("%d\n",matrixpow(len));
	}
	return 0;
}

