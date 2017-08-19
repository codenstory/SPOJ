/*
 * LONGCS
 * TOPIC: suffix automata
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXM (10)
#define A (26+MAXM)
#define L(k) ((k)&((~(k))+1ULL))
#define FG (ptr = fgets(buff,sizeof buff,stdin))
#define N (1<<17)
#define BIT(k) (1ULL<<(k))
#define is_solid(x,ch) ((x)->son[ch]->len+1 == (x)->len)
#define find(x,ch) (((x)->u)&BIT(ch)?x->son[ch]:NULL)
#define MASK(k) (BIT(k)-1ULL)
typedef unsigned long long u64;

typedef struct cell {
	struct cell *slink, *son[A];
	u64 u,mask;
	int len,cnt;
} cell;

cell pool[N<<1],*qtr,*sink,*root,*q[N<<1],**head,**tail;
char *ptr,buff[N],which[1<<21];
int who( u64 u ) {return u>=BIT(20)?20+which[u>>20]:which[u];}
int m,n;

cell *init( int len, int idx ) {
	cell *x = qtr++;
	int i,j,k;
	x->len = len, x->mask = BIT(idx), x->u = 0, x->slink = NULL, x->cnt = 0;
	return x;
}

void add_son( cell *x, int ch, cell *y ) { x->u|=BIT(ch), x->son[ch]=y; }

cell *split( cell *x, int ch, int idx ) {
	cell *z = init(x->len+1,idx), *y = find(x,ch);
	u64 u;
	for ( z->slink = y->slink, y->slink = z, ++z->cnt, add_son(x,ch,z); (x=x->slink) && find(x,ch) && find(x,ch) == y; add_son(x,ch,z) ) ;
	for ( u = z->u = y->u; u; add_son(z,who(L(u)),find(y,who(L(u)))), u &= ~L(u) ) ;
	return z;
}

cell *update( int ch, int idx ) {
	cell *new_sink = init(sink->len+1,idx),*x;
	for ( add_son(x=sink,ch,new_sink); (x=x->slink) && !find(x,ch); add_son(x,ch,new_sink) ) ;
	++(new_sink->slink = (!x?root:is_solid(x,ch)?(x->mask|=BIT(idx),find(x,ch)):split(x,ch,idx)))->cnt;
	return new_sink;
}

int main() {
	int i,j,k,ts,marker;
	cell *x,*y;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( sscanf(FG,"%d",&ts); ts--; ) {
		qtr = pool, sink = root = init(0,0);
		for ( sscanf(FG,"%d",&m), marker = 26, i = 0; i < m; ++i ) {
			for ( FG, j = 0; 'a' <= buff[j] && buff[j] <= 'z'; sink = update(buff[j++]-'a',i) ) ;
			sink = update(marker++,i);
		}
		for ( head = tail = q, x = pool; x < qtr; ++x )
			if ( !x->cnt ) *tail++ = x;
		for ( ;head<tail; )
			if ( (y=(x=*head++)->slink) && (y->mask|=x->mask) >= 0 && !--y->cnt && (*tail++ = y) ) ;
		for ( k = 0, x = pool; x < qtr; ++x )
			if ( x->mask == MASK(m) )
				if ( x->len > k ) k = x->len;
		printf("%d\n",k);
	}
	return 0;
}

