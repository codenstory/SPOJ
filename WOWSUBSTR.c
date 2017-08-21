/*
 * WOWSUBSTR
 * TOPIC: suffix automata
 * status: TLE
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A 26
#define L(k) ((k)&((~(k))+1ULL))
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define is_solid(x,ch) ((x)->son[ch]->len == (x)->len+1)
#define N (1<<20)
#define find(x,ch) (((x)->u&BIT(ch))?x->son[ch]:NULL)
#define MOD (100007ULL)
typedef unsigned long long u64;

typedef struct cell {
	struct cell *slink, *son[A];
	int len, pos, cnt, freq;
	unsigned int u,mask;
	char is_wow,visited,seen;
} cell;

cell pool[N<<1], *ptr, *root, *sink, *q[N<<1], **head, **tail;
char which[1<<21],s[N];
u64 ans;

int who( u64 u ) {
	if ( u >= BIT(40) ) return 40+which[u>>40];
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

void add_son( cell *x, int ch, cell *y ) { x->u|=BIT(ch),x->son[ch]=y; }

cell *init( int len, int pos ) {
	cell *x = ptr++;
	x->len=len, x->freq=0, x->pos=pos, x->seen=x->u=x->mask=x->is_wow=x->visited=x->cnt=0, x->slink=NULL;
	return x;
}

cell *split( cell *x, int ch, int pos ) {
	unsigned int u;
	cell *z = init(x->len+1,pos), *y = find(x,ch);
	for ( z->slink=y->slink,y->slink=z,++z->cnt, add_son(x,ch,z); (x=x->slink) && find(x,ch) && find(x,ch) == y; add_son(x,ch,z) ) ;
	for ( u=z->u=y->u; u; add_son(z,who(L(u)),find(y,who(L(u)))), u&=~L(u) ) ;
	return z;
}

cell *update( int ch, int pos ) {
	cell *new_sink = init(sink->len+1,pos), *x;
	for ( new_sink->freq=1,add_son(x=sink,ch,new_sink); (x=x->slink) && !find(x,ch); add_son(x,ch,new_sink) ) ;
	++((new_sink->slink=(!x?root:is_solid(x,ch)?find(x,ch):split(x,ch,pos)))->cnt);
	return new_sink;
}

u64 f( u64 x, u64 y ) {
	u64 a,b;
	assert( x <= y );
	if ( x == y ) return 0;
	a = (x+y-1), b = (y-x);
	assert( (!(a&1) || !(b&1)) && (a&1)^(b&1) );
	return (!(a&1)?(a>>1)*b:(b>>1)*a)%MOD;
}

void visit( cell *x ) {
	int i,j;
	if ( !x || x->visited )
		return ;
	if ( x->seen ) {
		x->visited = 1, visit(x->slink);
		return ;
	}
	x->visited = 1, visit(x->slink);
	if ( !x->slink || !(x->slink->is_wow) ) return ;
	assert( x->len > x->slink->len );
	assert( x->pos >= x->slink->len );
	for ( j = x->pos-x->slink->len, x->mask=x->slink->mask, i=x->slink->len+1; i<=x->len; ++i, --j )
		if ( !(x->mask&BIT(s[j]-'a')) )
			x->mask|=BIT(s[j]-'a');
		else break ;
	ans += (x->freq*f(x->slink->len+1,i))%MOD, ans %= MOD, x->is_wow = (i==x->len+1);
}

int main() {
	int i,j,k,ts,cs = 0,n;
	cell *x,*y;
	for ( k = 0; k < 21; which[1<<k] = k, ++k ) ;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%s",s); ) {
		for ( ptr = pool, sink = root = init(0,-1), n = 0; s[n]; sink = update(s[n]-'a',n), ++n ) ;
		for ( head = tail = q, x = pool; x < ptr; ++x )
			if ( !x->cnt )
				*tail++ = x;
		for ( ;head < tail; ) 
			if ( y=(x=*head++)->slink ) {
				if ( y->pos<x->pos )
					y->pos=x->pos;
				y->freq += x->freq;
				if ( !--y->cnt ) *tail++=y;
			}
		for ( ans=0, root->is_wow=root->visited=1, x=pool+1; x<ptr; visit(x++) ) ;
		printf("Case %d: %llu\n",++cs,ans);
	}
	return 0;
}

