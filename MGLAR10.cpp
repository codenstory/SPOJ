/*
 * MGLAR10
 * TOPIC: Aho-Corasick, DAG, DP
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
#include <unordered_set>
#define A (26)
#define N (1<<21)
#define BIT(k) (1ULL<<(k))
#define L(k) ((k)&((~(k))+1ULL))
#define MASK(k) (BIT(k)-1ULL)
#define FG (qtr=fgets(buff,sizeof buff,stdin))
using namespace std;

typedef struct cell {
	struct cell *slink, *son[A];
	unordered_set<int> idx;
	unsigned int u;
	bool is_terminal;
	int id;
} cell;

cell pool[N],*ptr,*q[N],**head,**tail;
char which[1<<21];

int who( unsigned int u ) { return u>=BIT(20)?20+which[u>>20]:which[u]; }

cell *init() {
	cell *x = ptr++;
	x->slink = NULL, x->u = 0, x->idx.clear(), x->is_terminal = false;
	return x;
}

class ACAutomaton {
private:
	cell *root,*q0;
	void calc_slinks() {
		cell *x,*y,*z;
		unsigned int u,ch;
		for ( head=tail=q, *tail++=root; head<tail; )
			for ( u=(x=*head++)->u; u; *tail++=y, u&=~L(u) ) {
				for ( y=x->son[ch=who(L(u))],z=x->slink; !(z->u&BIT(ch)); z=z->slink ) ;
				y->slink = z->son[ch];
				for ( unordered_set<int> :: iterator it=y->slink->idx.begin(); it!=y->slink->idx.end(); y->idx.insert(*it++) ) ;
			}
	}
public:
	ACAutomaton() {
		root=init(), q0=init(), root->slink = q0, q0->u = MASK(A);
		for ( int i = 0; i < A; q0->son[i++] = root ) ;
	}
	void push( char *s, int idx ) {
		cell *x;
		for ( x = root; x && *s && *s != '\n'; x=x->son[*s++-'a'] )
			if ( !(x->u&BIT(*s-'a')) )
				x->son[*s-'a']=init(),x->u|=BIT(*s-'a');
		x->idx.insert(idx), x->is_terminal = true, x->id = idx;
	}
	bool find( char *s ) {
		cell *x;
		for ( x = root; x && *s && *s != '\n'; x = x->son[*s++-'a'] )
			if ( !(x->u & BIT(*s-'a')) )
				return false ;
		return (!*s||*s=='\n') && x->is_terminal;
	}
	void collect( char *s, int id, unordered_set<int> &st ) {
		cell *x;
		for ( x = root; *s && *s != '\n'; x = x->son[*s++-'a'] ) 
			for ( unordered_set<int> :: iterator it = x->idx.begin(); it != x->idx.end(); ++it ) 
				if ( *it != id )
					st.insert(*it);
		for ( unordered_set<int> :: iterator it = x->idx.begin(); it != x->idx.end(); ++it ) 
			if ( *it != id )
				st.insert(*it);
	}
	void preprocess() { calc_slinks(); }
} *acc;

char buff[1<<20],*qtr,o[1<<20],*cur,*start[1<<14];
int n,z[1<<14],seen[1<<14],yes;
unordered_set<int> adj[1<<14];

int calc_z( int x ) {
	if ( seen[x] == yes )
		return z[x];
	z[x] = 1, seen[x] = yes;
	for ( unordered_set<int> :: iterator it = adj[x].begin(); it != adj[x].end(); z[x] = max(z[x],1+calc_z(*it++)) ) ;
	return z[x];
}

int main() {
	int i,j,k,m,ans;
	for ( k = 0; k < 21; which[BIT(k)] = k, ++k ) ;
	for (;FG&&1==sscanf(buff,"%d",&n)&&n; printf("%d\n",ans) ) {
		for ( cur = o, m = 0, ptr = pool, acc = new ACAutomaton(), i = 0; i < n; ++i ) 
			if ( !acc->find(FG) )
				acc->push(qtr,m++), start[m-1] = cur, cur += sprintf(cur,"%s",qtr), *cur++ = '\0';
		for ( acc->preprocess(), i = 0; i < m; adj[i].clear(), acc->collect(start[i],i,adj[i]), ++i ) ;
		for ( ++yes, ans = 0, i = 0; i < m; ans = max(ans,calc_z(i++)) ) ;
	}
	return 0;
}

