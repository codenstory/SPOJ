/*
 * LCS2: Longest Common Substring 2
 * TOPIC: suffix automata, dp on suffix automata
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
#define tol (1e-13)
#define BIT(k) (1ULL<<(k))
#define SET(u,k) ((u)|=BIT(k))
#define TST(u,k) ((u)&BIT(k))
#include <map>
#define oo (Q+1)
#define MAXN (10)
#define MAXL (100100)
#define Q (MAXN*MAXL)
#include <set>
#define A 26
#define MASK(k) (BIT(k)-1ULL)
#define is_solid(x,y) ((x)->len+1 == (y)->len)
#define L(k) ((k)&(~(k)+1ULL))
using namespace std;
typedef unsigned long long u64;

typedef struct cell {
	cell *slink;
	unsigned int a,u;
	int len,maxlen[MAXN];
	cell *son[A];
	cell() {
		slink = NULL, len = 0, u = a = 0;
	}
} cell;

cell pool[2*MAXL], *pool_ptr = pool;

inline cell *init( int l ) {
	assert( pool_ptr-pool < sizeof(pool)/sizeof *pool );
	cell *x = pool_ptr++;
	x->len = l, x->slink = NULL, x->a = x->u = 0;
	memset(x->maxlen,0,sizeof x->maxlen);
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
	cell *y = x->son[ch], *z = init(x->len+1), *cur;
	z->slink = y->slink, y->slink = z, x->son[ch] = z;
	for ( v=z->a=y->a; v; v &= ~L(v) ) {
		z->son[who(L(v))]=y->son[who(L(v))];
		assert( !is_solid(z,z->son[who(L(v))]) );
	}
	for ( cur = x; cur != root; ) {
		cur = cur->slink;
		if ( TST(cur->a,ch) && cur->son[ch] == y && !is_solid(cur,y) ) {
			cur->son[ch] = z;
			assert( !is_solid(cur,z) );
		}
		else break ;
	}
	return z;
}

inline cell *update( int ch ) {
	cell *new_sink = init(sink->len+1), *suf = sink->slink;
	sink->son[ch] = new_sink, SET(sink->a,ch);
	for (;suf != NULL && !TST(suf->a,ch); suf->son[ch] = new_sink, SET(suf->a,ch), suf = suf->slink ) ;
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

char buff[MAXL], *ptr;

int main() {
	int i,j,k,t,ch,last;
	cell *x,*y,*cur;
	for ( i = 0; i < (1<<21); bts[i] = bts[i>>1]+(i&1), ++i ) ;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	ch = getchar_unlocked()-'a';
	if ( !(0 <= ch & ch < A) ) {
		puts("0");
		return 0;
	}
	root = init(0), sink = init(root->len+1), root->son[ch] = sink, SET(root->a,ch), sink->slink = root;
	for ( n = 0; (ch = getchar_unlocked()) != EOF && ch != '\n'; sink = update(ch-'a') ) ;
	for (;n < MAXN-1 && (ptr = fgets(buff,sizeof buff,stdin)); ++n ) 
		for ( t=0, cur=root, SET(cur->u,n), cur->maxlen[n]=max(cur->maxlen[n],t); *ptr && *ptr != '\n'; ) {
			assert( t <= cur->len ) ;
			assert( 'a' <= *ptr && *ptr <= 'z' ) ;
			for ( ch = *ptr++-'a'; cur && !TST(cur->a,ch); cur = cur->slink ) ;
			if ( !cur ) cur = root, t = 0;
			else {
				if ( t > cur->len ) t = cur->len;
				SET(cur->u,n), cur->maxlen[n] = max(cur->maxlen[n],t);
				cur = cur->son[ch], ++t;
				//SET(cur->u,n), cur->maxlen[n] = max(cur->maxlen[n],t);
				for ( k = t, x = cur; x; ) {
					if ( TST(x->u,n) && x->maxlen[n] >= k ) break ;
					SET(x->u,n), x->maxlen[n] = max(x->maxlen[n],k);
					if ( (x = x->slink) && x->len < k ) k = x->len;
				}
			}
		}
	for ( ans = 0, i = 0; i < pool_ptr-pool; ++i )
		if ( card(pool[i].u) == n ) {
			for ( k = pool[i].len, j = 0; j < n; ++j )
				k = min(k,pool[i].maxlen[j]);
			ans = max(ans,k);
		}
	printf("%d\n",ans);
	return 0;
}

