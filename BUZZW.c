/*
 * BUZZW
 * TOPIC: suffix automata, stacks
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define A 26
#define BIT(k) (1ULL<<(k))
#define L(k) ((k)&((~(k))+1ULL))
#define is_solid(x,ch) ((x)->son[ch]->len == (x)->len+1)

typedef struct cell {
	struct cell *slink, *son[A];
	unsigned int u;
	int len, freq, cnt;
} cell;

cell *ptr,pool[N<<1],*root,*sink,*q[N<<1],**head,**tail;
char which[1<<21];

int who( unsigned int u ) {
	return u>=BIT(20)?20+which[u>>20]:which[u];
}

cell *init( int len ) {
	cell *x=ptr++;
	x->u=x->cnt=x->freq=0,x->len=len,x->slink=NULL;
	return x;
}

void add_son( cell *x, int ch, cell *y ) { x->u|=BIT(ch), x->son[ch]=y; }
cell *find( cell *x, int ch ) { return (x->u)&BIT(ch)?x->son[ch]:NULL; }

cell *split( cell *x, int ch ) {
	unsigned u,i;
	cell *z = init(x->len+1), *y = find(x,ch);
	for ( z->slink=y->slink,y->slink=z,++z->cnt, add_son(x,ch,z); (x=x->slink) && find(x,ch) && find(x,ch) == y; add_son(x,ch,z) ) ;
	for ( u=z->u=y->u; u && (i=who(L(u))) >= 0; add_son(z,i,find(y,i)), u &= ~L(u) ) ;
	return z;
}

cell *update( int ch ) {
	cell *new_sink = init(sink->len+1), *x;
	for ( new_sink->freq=1, add_son(x=sink,ch,new_sink); (x=x->slink) && !find(x,ch); add_son(x,ch,new_sink) ) ;
	new_sink->slink=(!x?root:is_solid(x,ch)?find(x,ch):split(x,ch)), ++new_sink->slink->cnt;
	return new_sink;
}

int is_empty( char *ptr ) {
	for ( ;*ptr && *ptr != '\n' && *ptr == ' '; ++ptr ) ;
	return !*ptr || !('A'<=*ptr && *ptr<='Z');
}

char s[N],*p;
int freq[N],st[N],*top,n;

int main() {
	int i,j,k;
	cell *x,*y;
	for ( i = 0; i < 21; which[1<<i] = i , ++i ) ;
	for ( ;(p=fgets(s,sizeof s,stdin)) && !is_empty(p); ) {
		for ( ptr = pool, root = sink = init(0), n = 0; ; ) {
			for ( ;s[n] && s[n] != '\n' && !('A' <= s[n] && s[n] <= 'Z'); ++n ) ;
			if ( s[n] && 'A' <= s[n] && s[n] <= 'Z' ) {
				sink = update(s[n++]-'A');
				continue ;
			}
			break ;
		}
		for ( head=tail=q, x=pool; x<ptr; ++x )
			if ( !x->cnt ) 
				*tail++=x;
		for ( ;head<tail; )
			if ( (y=(x=*head++)->slink) && (y->freq+=x->freq)>=0 && !--y->cnt )
				*tail++=y;
		for ( i = 0; i <= n; freq[i++] = -1 ) ;
		for ( x = pool; x < ptr; ++x ) 
			if ( freq[k=x->len] < x->freq )
				freq[k] = x->freq;
		for ( top = st, i = 1; i <= n; *++top = i++ ) 
			for ( ;top > st && freq[*top] <= freq[i]; --top ) ;
		if ( top == st || freq[st[1]] <= 1 ) {
			puts("");
			continue ;
		}
		for(k=1,i=1;i<=top-st;++i) 
			for (;k<=st[i];freq[k++]=freq[st[i]]) ;
		for ( i = 1; i <= n && freq[i] >= 2; printf("%d\n",freq[i++]) ) ;
		puts("");
	}
	return 0;
}

