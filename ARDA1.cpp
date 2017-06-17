/*
 * ARDA1: The hunt for Gollum
 * TOPIC: 2-D pattern matching, Aho-Corasick
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
#include <set>
#include <queue>
#define BIT(k) (1ULL<<(k))
#define TST(u,k) (((u)>>(k))&1)
#define SET(u,k) ((u)|=BIT(k))
#define tol (1e-13)
#define A 26
#define L(k) ((k)&((~(k))+1ULL))
using namespace std;
enum { P, T };

typedef struct cell {
	char ch;
	cell *son[A],*slink;
	unsigned int u;
	set<int> endings;
	cell() {
		endings.clear(), u = 0;
	}
} cell;

cell *q0;

cell *init_cell( char ch ) {
	cell *x = (cell *)malloc(sizeof *x);
	x->u = 0, x->endings.clear(), x->slink = q0, x->ch = ch;
	return x;
}

void push( cell *root, char *s, int idx ) {
	cell *x,*y,*z;
	for ( x=root, y=NULL; *s; x=x->son[*s++-'a'] ) 
		if ( !TST(x->u,*s-'a') ) {
			x->son[*s-'a'] = y = init_cell(*s-'a'), SET(x->u,*s-'a');
			for ( z = x->slink; z != q0 && !TST(z->u,*s-'a'); z = z->slink ) ;
			if ( z == q0 ) y->slink = q0;
			else 
				y->slink = z->son[*s-'a'];
		}
	x->endings.insert(idx);
}

int m[2],n[2];
char pattern[0x400],which[1<<21],
	 text[0x800];
set<int> pos[0x800][0x400];
queue<cell *> q;

int who( unsigned int u ) {
	if ( u < (1<<20) )
		return which[u];
	return 20+which[u>>20];
}

bool check( int i, int j, const int p ) {
	if ( j >= m[P] ) return true ;
	if ( i >= m[T] ) return false ;
	assert( i < m[T] && j < m[P] );
	return pos[i][j].find(p) != pos[i][j].end() && check(i+1,j+1,p);
}

int main() {
	int i,j,k;
	cell *root,*x,*y,*z;
	unsigned int u;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for (;2==scanf("%d %d",m+P,n+P);) {
		root=init_cell('$'), root->slink=q0=init_cell('\0');
		for ( i = 0; i < A; ++i ) 
			SET(q0->u,i), q0->son[i] = root;
		for ( i = 0; i < m[P]; ++i )
			scanf("%s",pattern), push(root,pattern,i);
		assert( q.empty() );
		for ( u = root->u; u; u &= ~L(u) )
			x = root->son[who(L(u))], x->slink = root, q.push(x);
		for (;!q.empty();) 
			for ( x = q.front(), q.pop(), u=x->u; u; u &= ~L(u), q.push(y) ) {
				y = x->son[who(L(u))];
				assert( y->ch == who(L(u)));
				for ( z = x->slink; !TST(z->u,y->ch); z = z->slink ) ;
				y->slink = z->son[y->ch];
				for ( set<int>::iterator it = y->slink->endings.begin(); it != y->slink->endings.end(); y->endings.insert(*it++) ) ;
			}

		scanf("%d %d",m+T,n+T);
		for ( i = 0; i < m[T]; ++i )
			for ( j = 0; j < m[P]; pos[i][j++].clear() ) ;
		for ( i = 0; i < m[T]; ++i ) 
			for ( scanf("%s",text), x=root, j=0; j<n[T]; ++j ) {
				if ( TST(x->u,text[j]-'a') ) 
					x=x->son[text[j]-'a'];
				else {
					for ( x=x->slink; !TST(x->u,text[j]-'a'); x=x->slink ) ;
					x=x->son[text[j]-'a'];
				}
				for ( set<int>::iterator it = x->endings.begin(); it != x->endings.end(); pos[i][*it++].insert(j) ) ;
			}
		set<pair<int,int> > ans;
		for ( ans.clear(), i = 0; i < m[T]; ++i )
			for ( set<int>::iterator it = pos[i][0].begin(); it != pos[i][0].end(); ++it )
				if ( check(i+1,1,*it) )
					ans.insert(make_pair(i+1,*it-n[P]+2));
		if ( !ans.size() ) {
			puts("NO MATCH FOUND...");
			continue ;
		}
		for ( set<pair<int,int> >::iterator it = ans.begin(); it != ans.end(); ++it ) 
			printf("(%d,%d)\n",it->first,it->second);
	}
	return 0;
}

