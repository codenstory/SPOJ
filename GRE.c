/*
 * GRE
 * TOPIC: Segment tree with lazy propagation, Aho-Corasick
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A (26)
#define id(x) ((x)-pool)
#define N (1<<20)
#define Q (N)
#define L(k) ((k)&((~(k))+1ULL))
#define SQRTN (0x400)
#define MASK(k) (BIT(k)-1ULL)
#define BIT(k) (1ULL<<(k))
#define MAXE (Q<<1)
#define M (1<<17)
#define int2cell(i) ((i)+pool)
#define LEFT(k) ((k)<<1)
#define RIGHT(k) (1|LEFT(k))
#define oo (1LL<<60)
#pragma comment( linker, "/STACK:1024000000,1024000000")
typedef long long i64;

i64 max( i64 x, i64 y ) { return x<y?y:x; }

typedef struct cell {
	struct cell *son[A], *slink;
	int cnt,u;
} cell;

cell pool[Q], *ptr, *root, *q0,
	 *q[Q], **head, **tail,
	 *st[Q], **top;
int tin[Q],tout[Q],last[Q],next[MAXE],to[MAXE],E,tick,
	BS,weight[M];
i64 block[SQRTN],tr[Q],value[Q];
char has_update[Q];
char which[1<<21], buff[N], *start[M], text[N], *cur, str[N];
int who( unsigned int u ) { return u>=BIT(20)?20+which[u>>20]:which[u]; }

cell *init() {
	cell *x = ptr++;
	x->cnt = x->u = 0, x->slink = NULL, last[id(x)] = tin[id(x)] = -1;
	return x;
}

void push( char *s, int idx ) {
	cell *x = root;
	for ( ;*s && 'a' <= *s && *s <= 'z'; x = x->son[*s++-'a'] )
		if ( !(x->u & BIT(*s-'a')) )
			x->u |= BIT(*s-'a'), x->son[*s-'a'] = init();
}

void add_arcs( int x, int y ) {
	int i = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
}

void dfs( int x ) {
	int i;
	assert( tin[x] == -1 );
	for ( tin[x] = ++tick, i = last[x]; i >= 0; dfs(to[i]), i = next[i] ) ;
	tout[x] = tick;
}

void max_update( int, int, int, i64 ) ;

void push_down( int v, int l, int r ) {
	int k = (l+r)>>1;
	if ( has_update[v] ) {
		max_update(LEFT(v),l,k,value[v]), max_update(RIGHT(v),k+1,r,value[v]);
		has_update[v] = 0;
	}
}

void push_up( int v ) {
	tr[v] = max(tr[LEFT(v)],tr[RIGHT(v)]);
}

void build( int v, int l, int r ) {
	int k = (l+r)>>1;
	tr[v] = -oo, has_update[v] = 0;
	assert( l <= r );
	if ( l == r ) {
		tr[v] = 0;
		return ;
	}
	build(LEFT(v),l,k), build(RIGHT(v),k+1,r), push_up(v);
}

void max_update( int v, int l, int r, i64 val ) {
	if ( has_update[v] )
		val = max(val, value[v]);
	has_update[v] = 1, value[v] = val;
	tr[v] = max(tr[v],val);
}

void assign( int v, int l, int r, int qi, int qj, i64 val ) {
	int k = (l+r)>>1;
	if ( qi > r || qj < l ) return ;
	if ( qi <= l && r <= qj ) {
		max_update(v,l,r,val);
		return ;
	}
	push_down(v,l,r);
	assign(LEFT(v),l,k,qi,qj,val), assign(RIGHT(v),k+1,r,qi,qj,val);
	push_up(v);
}

i64 query( int v, int l, int r, int qi, int qj ) {
	int k = (l+r)>>1;
	if ( qi > r || qj < l )
		return -oo;
	if ( qi <= l && r <= qj ) 
		return tr[v];
	push_down(v,l,r);
	return max(query(LEFT(v),l,k,qi,qj),query(RIGHT(v),k+1,r,qi,qj));
}

i64 process( int idx ) {
	cell *x = root;
	char *p;
	i64 ans = 0;
	for ( p = start[idx]; *p && 'a' <= *p && *p <= 'z'; ) {
		assert( (x->u & BIT(*p-'a')) );
		x = x->son[*p++-'a'];
		ans = max(ans,query(1,1,tick,tin[id(x)],tin[id(x)]));
	}
	assign(1,1,tick,tin[id(x)],tout[id(x)],ans+weight[idx]);
	return ans+weight[idx];
}

int main() {
	int i,j,k,ts,cs = 0,u,n;
	cell *x,*y,*z;
	i64 ans;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	for ( sscanf(fgets(buff,sizeof buff,stdin),"%d",&ts); ts--; printf("Case #%d: %lld\n",++cs,ans) ) {
		sscanf(fgets(buff,sizeof buff,stdin),"%d",&n), E = 0, tick = 0;
		for ( cur=text, i=0; i<n; start[i]=cur, sscanf(fgets(buff,sizeof buff,stdin),"%s %d",str,weight+i),cur+=sprintf(cur,"%s",str), *cur++='\0', ++i ) ;
		for ( ptr=pool, root=init(), root->slink=q0=init(),++q0->cnt,q0->u=MASK(A), i=0; i<A; q0->son[i++]=root ) ;
		for ( i = 0; i < n;	push(start[i],i), ++i ) ;
		for ( head = tail = q, *tail++ = root; head < tail; )
			for ( u = (x=*head++)->u; u && (i=who(L(u)))>=0; ++(y->slink=z->son[i])->cnt, *tail++=y,  u&=~L(u) )
				for ( y=x->son[i], z = x->slink; !(z->u & BIT(i)); z = z->slink ) ;
		for ( x = pool; x < ptr; ++x )
			if ( !x->cnt ) *tail++ = x;
		for ( top = st; head < tail; ) {
			*++top = x = *head++;
			if ( y=x->slink ) add_arcs(id(y),id(x));
			if ( y && !--y->cnt )
				*tail++ = y;
		}
		for ( ;top > st; --top )
			if ( tin[id(*top)] == -1 ) dfs(id(*top));
		build(1,1,tick);
		for ( ans = 0, i = 0; i < n; ans = max(ans,process(i++)) ) ;
	}
	return 0;
}

