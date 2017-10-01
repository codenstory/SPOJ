/*
 * DNALAB
 * TOPIC: Aho-Corasić
 */
#include <unordered_map>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define S 7
#define N (BIT(S))
#define A 4
#define M 15
#define L(k) ((k) & (~(k)+1ULL))
#define BIT(k) (1ULL<<(k))
#define Q (N*M)
#define oo (1<<30)
#define MASK(k) (BIT(k)-1ULL)
#define SH (12)
#define enc(x,u) (((x)-pool)|((u)<<SH))
using namespace std;

int n,m = M,len[M],yes;
char str[M][N],which[1<<21],id[256],rmap[A],buff[0x400];
const char *a = "ACGT";
unordered_map<unsigned int,unsigned int> parent;
unordered_map<unsigned int,char> _move;

typedef struct cell {
	struct cell *slink, *son[A], *tr[A];
	unsigned int u,v,mask;
	short d[1<<M], seen[1<<M];
} cell ;

cell *ptr,pool[Q],*root,*q[Q],**head,**tail,*q0;
unsigned int queue[Q*(1<<M)],*h,*t;

int who( unsigned int u ) { return u>=BIT(20)?20+which[u>>20]:which[u]; }

cell *init() {
	cell *x = ptr++;
	x->u = x->v = x->mask = 0, x->slink = NULL;
	return x;
}

void push( char *s, int idx ) {
	cell *x = root;
	for ( ;*s && *s != '\n'; x = x->son[id[*s++]] )
		if ( !(x->u & BIT(id[*s])) ) 
			x->u |= BIT(id[*s]), x->son[id[*s]] = init();
	x->mask |= BIT(idx);
}

cell *calc_failure_transition( cell *x, int ch ) {
	assert( x );
	if ( x->v & BIT(ch) )
		return x->tr[ch];
	if ( x->u & BIT(ch) ) {
		x->v |= BIT(ch), x->tr[ch] = x->son[ch];
		return x->tr[ch];
	}
	x->v |= BIT(ch), x->tr[ch] = calc_failure_transition(x->slink,ch);
	return x->tr[ch];
}

void dump( unsigned int u ) {
	if ( !(u&MASK(SH)) ) return ;
	dump(parent[u]), putchar(_move[u]);
}

int main() {
	int i,j,k,nk,ch,best,cs = 0,ts;
	unsigned int u,v,w,ww,best_u;
	cell *x,*y,*z;
	for ( i = 0; i < A; rmap[id[a[i]] = i] = a[i], ++i ) ;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( sscanf(fgets(buff,sizeof buff,stdin),"%d",&ts); ts-- && sscanf(fgets(buff,sizeof buff,stdin),"%d",&n); ) {
		ptr = pool, root = init(), q0 = init(), root->slink = q0;
		for ( i = 0; i < A; q0->son[i] = root, q0->u |= BIT(i), ++i ) ;
		for ( i = 0; i < n; fgets(str[i],sizeof str[i],stdin), len[i] = strlen(str[i]), push(str[i],i), ++i ) ;
		for ( w = 0, i = 0; i < M; ++i )
			for ( j = 0; j < len[i] && str[i][j] != '\n'; w |= BIT(id[str[i][j++]]) ) ;
		for ( head = tail = q, *tail++ = root; head < tail; ) 
			for ( u = (x = *head++)->u; u && (ch = who(L(u))) >= 0; y->mask|=(y->slink=z)->mask, *tail++ = y, u &= ~L(u) ) 
				y = x->son[ch], z = calc_failure_transition(x->slink,ch);
		parent.clear(), _move.clear();
		for ( best=+oo, h=t=queue, root->seen[0]=++yes, root->d[0]=0, *t++=enc(root,0); h<t; ) {
			u = *h++, x = pool+(u&MASK(SH)), k = (u>>SH);
			if ( k == MASK(n) && x->d[k] < best ) {
				best = x->d[k], best_u = u;
				break ;
			}
			if ( k == MASK(n) || x->d[k] >= best ) continue ;
			for ( ww = w; ww && (ch = who(L(ww))) >= 0; ww &= ~L(ww) ) {
				if ( x->u & BIT(ch) ) y = x->son[ch];
				else y = calc_failure_transition(x,ch);
				nk = y->mask|k, v = enc(y,nk);
				if ( y->seen[nk] != yes || y->d[nk] > x->d[k]+1 )
					*t++=v, y->d[nk]=x->d[k]+1, y->seen[nk]=yes, _move[v]=rmap[ch], parent[v]=u;
			}
		}
		printf("Scenario #%d:\n",++cs), dump(best_u), putchar('\n');
	}
	return 0;
}

