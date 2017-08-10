/*
 * NSUBSTR
 * TOPIC: suffix automaton, stack, Pareto-optimality
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A 26
#define L(k) ((k)&((~(k))+1ULL))
#define N (250100)
#define Q (3*N)
#define SQ (512)
#define is_primary(x,ch) ((x)->son[ch]->len == (x)->len+1)
#define BIT(k) (1LL<<(k))
typedef unsigned long long u64;
int max( int x, int y ) { return x<y?y:x; }

typedef struct cell {
	struct cell *son[A],*slink;
	unsigned int u;
	int freq,cnt,len;
} cell ;

cell pool[Q], *ptr = pool, *root, *sink, *q[Q],**head,**tail;
char which[1<<21],s[N];
int n,freq[N];

int who( u64 u ) {
	assert( !(u&(u-1)) );
	if ( u >= BIT(60) ) return 60+which[u>>60];
	if ( u >= BIT(40) ) return 40+which[u>>40];
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

cell *init( int len ) {
	cell *x = ptr++;
	x->slink = NULL, x->u = x->freq = x->cnt = 0, x->len = len;
	return x;
}

void add_son( cell *x, int ch, cell *y ) { x->u|=BIT(ch), x->son[ch]=y; }
cell *find( cell *x, int ch ) { return (x->u)&BIT(ch)?x->son[ch]:NULL; }
int dp[N],st[N],*top;

cell *split( cell *x, int ch ) {
	unsigned int v,i;
	cell *y = find(x,ch), *z = init(x->len+1);
	for ( z->slink=y->slink,y->slink=z,++z->cnt, add_son(x,ch,z); (x=x->slink) && find(x,ch) && find(x,ch) == y; add_son(x,ch,z) ) ;
	for ( v=z->u=y->u; v&&(i=who(L(v)))>=0; add_son(z,i,find(y,i)), v&=~L(v) ) ;
	return z;
}

cell *update( int ch ) {
	cell *x,*new_sink = init(sink->len+1);
	for ( new_sink->freq=1, add_son(x=sink,ch,new_sink); (x=x->slink) && !find(x,ch); add_son(x,ch,new_sink) ) ;
	if ( !x )
		new_sink->slink = root;
	else new_sink->slink = is_primary(x,ch)?find(x,ch):split(x,ch);
	++new_sink->slink->cnt;
	return new_sink;
}

int main() {
	int i,j,k,l;
	cell *x,*y;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for (;1==scanf("%s",s);) {
		for ( ptr = pool, root = sink = init(0), i = 0; s[i]; sink = update(s[i++]-'a') ) ;
		for ( n = i, i = 0; i <= n; dp[i] = -1, freq[i++] = -1 ) ;
		for ( head = tail = q, x = pool; x < ptr; ++x )
			if ( !x->cnt ) *tail++ = x;
		for (;head<tail;) 
			if ( (y=(x=*head++)->slink) && (y->freq+=x->freq)>=0 && !--y->cnt && (*tail++=y) ) ;
		for ( x = pool+1; x < ptr; ++x ) 
			if ( dp[k = x->len] < x->freq )
				dp[k] = x->freq;
		for(top=st,i=1;i<=n;*++top=i++)
			for(;top>st&&dp[*top]<dp[i];--top);
		for(k=1,j=1;j<=top-st;++j) 
			for(;k<=st[j];freq[k++]=dp[st[j]]);
		for ( i = 1; i <= n; printf("%d\n",freq[i++]) ) ;
	}
	return 0;
}

