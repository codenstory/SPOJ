/*
 * LCS2: Longest Common Substring 2
 * TOPIC: suffix automata
 * status:
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol (1e-13)
#define BIT(k) (1ULL<<(k))
#define SET(u,k) ((u)|=BIT(k))
#define TST(u,k) ((u)&BIT(k))
using namespace std;
#include <map>
#define oo 0xfffffffful
#define MAXN (10)
#define MAXL (100100)
#define Q (MAXN*MAXL)
#include <set>
#define A 26
#define MASK(k) (BIT(k)-1ULL)
#define is_solid(x,y) ((x)->len+1 == (y)->len)
#define L(k) ((k)&(~(k)+1ULL))
#include <queue>
typedef unsigned long long u64;

typedef struct cell {
	cell *slink;
	unsigned int u;
	u64 a;
	char cnt,ch;
	int len;
	cell *son[A+MAXN];
	set<cell *> up;
	cell() {
		slink = NULL, u = +oo, len = 0, a = 0;
	}
} cell;

cell pool[2*Q], *pool_ptr = pool;

inline cell *init( int l, int ch ) {
	assert( pool_ptr-pool < sizeof(pool)/sizeof *pool );
	cell *x = pool_ptr++;
	x->len = l, x->slink = NULL, x->u = +oo, x->a = 0, x->up.clear(), x->cnt = -1, x->ch = ch;
	return x;
}

char which[1<<21],bts[1<<21];
int n,len,ans;
cell *sink,*root;

inline int card( u64 u ) {
	return bts[u&MASK(20)] + bts[(u>>20)&MASK(20)] + bts[(u>>40)&MASK(20)] + bts[(u>>60)&MASK(20)];
}

inline int who( u64 u ) {
	if ( u >= (1ULL<<40) )
		return 40+which[u>>40];
	if ( u >= (1ULL<<20) )
		return 20+which[u>>20];
	return which[u];
}

inline cell *split( cell *x, int ch ) {
	u64 v;
	cell *y = x->son[ch], *z = init(x->len+1,ch);
	z->slink = y->slink, y->slink = z, x->son[ch] = z, z->up.insert(x);
	y->up.erase(x);
	for ( v=z->a=y->a; v; z->son[who(L(v))]=y->son[who(L(v))], y->son[who(L(v))]->up.insert(z), v&=~L(v) ) ;
	return z;
}

inline cell *update( int ch ) {
	cell *new_sink = init(sink->len+1,ch), *suf = sink->slink;
	sink->son[ch] = new_sink, new_sink->up.insert(sink), SET(sink->a,ch);
	for (;suf != NULL && !TST(suf->a,ch); suf->son[ch] = new_sink, new_sink->up.insert(suf), SET(suf->a,ch), suf = suf->slink ) ;
	if ( !suf ) {
		new_sink->slink = root;
		return new_sink;
	}
	if ( is_solid(suf,suf->son[ch]) ) 
		new_sink->slink = suf->son[ch];
	else 
		new_sink->slink = split(suf,ch);
	return new_sink;
}

int q[Q],*head,*tail;

int main() {
	int i,j,k,ch,last;
	cell *x,*y;
	for ( i = 0; i < (1<<21); bts[i] = bts[i>>1]+(i&1), ++i ) ;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	ch = getchar_unlocked()-'a';
	root = init(0,'$'), sink = init(root->len+1,ch), root->son[ch] = sink, SET(root->a,ch), sink->slink = root;
	for ( n = 0; (ch = getchar_unlocked()) != EOF; last = ch, sink = update(ch=='\n'?(A+(n++)):ch-'a') ) ;
	if ( last != '\n' )
		sink = update(A+(n++));
	assert( n <= MAXN );
	ans = 0;
	head = tail = q;
	for ( i = 0; i < pool_ptr-pool; ++i )
		if ( !pool[i].a ) {
			*tail++ = i;
			if ( pool[i].ch < A )
				pool[i].u = 0;
		}
	while ( head < tail ) {
		x = pool + *head++;
		if ( x->ch >= A )
			x->u = BIT(x->ch-A);
		for ( set<cell *>::iterator it = x->up.begin(); it != x->up.end(); ++it ) {
			y = *it;
			if ( y->cnt < 0 )
				y->cnt = card(y->a), y->u = 0;
			y->u |= x->u;
			if ( !--y->cnt )
				*tail++ = y-pool;
		}
		if ( x->u == MASK(n) )
			ans = max(ans,x->len);
	}
	printf("%d\n",ans);
	return 0;
}

