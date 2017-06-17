/*
 * ADACLEAN: Ada and Spring Cleaning
 * TOPIC: suffix automaton
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
#define is_solid(x,y) ((x)->len+1 == (y)->len)
#define A 26
#define N (100100)
#define Q (2*N)
#define L(k) ((k)&((~(k))+1UL))
#define BIT(k) (1ULL<<(k))
#define TST(u,k) (((u)>>(k))&1)
#define SET(u,k) ((u)|=BIT(k))
#define MASK(k) (BIT(k)-1ULL)
using namespace std;

char which[BIT(21)];

int who( unsigned int u ) {
	return u>=BIT(20)?20+which[u>>20]:which[u];
}

typedef struct cell {
	cell *son[A],*slink;
	unsigned int a;
	int len;
	cell() {
		a = 0, slink = NULL, len = 0;
	}
} cell;

cell pool[Q], *pool_ptr = pool;

cell *init( int l ) {
	cell *x = pool_ptr++;
	x->a = 0, x->slink = NULL, x->len = l;
	return x;
}

int m,n;
char str[N],*ptr;
cell *root,*sink;

void set_transition( cell *x, int ch, cell *y ) { SET(x->a,ch), x->son[ch]=y; }

cell *split( cell *x, int ch ) {
	assert( TST(x->a,ch) ) ;
	cell *y = x->son[ch], *z = init(x->len+1), *cur;
	z->slink = y->slink, y->slink = z, set_transition(x,ch,z);
	for ( unsigned int v = y->a; v; set_transition(z,who(L(v)),y->son[who(L(v))]), v &= ~L(v) ) ;
	assert( z->a == y->a );
	for ( cur = x; cur != root; ) {
		cur = cur->slink;
		if ( cur && TST(cur->a,ch) && cur->son[ch] == y && !is_solid(cur,cur->son[ch]) )
			set_transition(cur,ch,z);
		else break ;
	}
	return z;
}

cell *update( int ch ) {
	cell *suf = sink->slink, *newsink = init(sink->len+1);
	for(set_transition(sink,ch,newsink);suf&&!TST(suf->a,ch);set_transition(suf,ch,newsink),suf=suf->slink) ;
	newsink->slink=!suf?root:(is_solid(suf,suf->son[ch])?suf->son[ch]:split(suf,ch));
	return newsink;
}

int main() {
	int i,j,k,ts,ans;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	for ( scanf("%d\n",&ts); ts--; printf("%d\n",ans) ) {
		scanf("%d %d\n",&n,&m), pool_ptr=pool;
		for(ptr=fgets(str,sizeof str,stdin), sink=root=init(0);*ptr&&*ptr!='\n';sink=update(*ptr++-'a')) ;
		for ( ans = 0, i = 0; i < pool_ptr-pool; ++i )
			if ( pool[i].len>=m && (!pool[i].slink||pool[i].slink->len<m) )
				++ans;
	}
	return 0;
}

