/*
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <set>
#define N (0x80)
#define MAXV (N*N*4)
#define MAXH (N)
#define Q (1<<21)
#define enc(truefalse,type,idx,height) ((truefalse) | ((type)<<1) | ((idx)<<2) | ((height)<<9))
enum { NEGATIVE, POSITIVE };
using namespace std;
enum { D, T };
enum { ROW, COL };

int m,n,a[N][N],V,E[2],seen[MAXV],yes,mark,scc[MAXV],
	next[2][Q],to[2][Q],last[2][Q],mx[2][N];
stack<int> st;
set<int> vertices;

void add_arcs( int x, int y ) {
	int i= E[D]++, j=E[T]++;
	assert( i < Q && j < Q );
	to[D][i]= y, next[D][i]= last[D][x], last[D][x]= i;
	to[T][j]= x, next[T][j]= last[T][y], last[T][y]= j;
}

void dfs( int t, int x ) {
	seen[x]= yes;
	for ( int y, i= last[t][x]; i >= 0; i= next[t][i] )
		if ( seen[y= to[t][i]] != yes )
			dfs(t,y);
	if ( t == T )
		st.push(x);
	else scc[x]= mark;
}

int main() {
	int i,j,k,cs= 0,ts,t,l,u,v;
	for ( cin >> ts; ts--; ) {
		cin >> m >> n;
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j )
				cin >> a[i][j];
		for ( t= ROW; t <= COL; ++t )
			for ( i= 0; i < N; ++i )
				mx[t][i]= -(1<<30);
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j )
				mx[ROW][i]= max(mx[ROW][i],a[i][j]);
		for ( j= 0; j < n; ++j )
			for ( i= 0; i < m; ++i )
				mx[COL][j]= max(mx[COL][j],a[i][j]);
		E[D]= E[T]= 0, V= 2*(m+n)*MAXH, vertices.clear();
		for ( i= 0; i < m; ++i )
			for ( k= 1; k < MAXH; ++k )
				for ( t= D; t <= T; ++t )
					for ( l= NEGATIVE; l <= POSITIVE; ++l )
						last[t][u= enc(l,ROW,i,k)]= -1, vertices.insert(u);
		for ( j= 0; j < n; ++j )
			for ( k= 1; k < MAXH; ++k )
				for ( t= D; t <= T; ++t )
					for ( l= NEGATIVE; l <= POSITIVE; ++l )
						last[t][v= enc(l,COL,j,k)]= -1, vertices.insert(v);
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j ) {
				for ( k= 1; k <= a[i][j]-1; ++k ) {
					add_arcs(enc(POSITIVE,ROW,i,a[i][j]),enc(NEGATIVE,ROW,i,k));
					add_arcs(enc(POSITIVE,COL,j,a[i][j]),enc(NEGATIVE,COL,j,k));
				}
				add_arcs(enc(NEGATIVE,ROW,i,a[i][j]),enc(POSITIVE,COL,j,a[i][j]));
				add_arcs(enc(NEGATIVE,COL,j,a[i][j]),enc(POSITIVE,ROW,i,a[i][j]));
			}
		for ( i= 0; i < m; ++i )
			for ( k= 1; k <= mx[ROW][i]-1; ++k )
				add_arcs(enc(POSITIVE,ROW,i,k),enc(NEGATIVE,ROW,i,k));
		for ( j= 0; j < n; ++j )
			for ( k= 1; k <= mx[COL][j]-1; ++k )
				add_arcs(enc(POSITIVE,COL,j,k),enc(NEGATIVE,COL,j,k));
		++yes, mark= -1;
		for ( set<int> :: iterator it= vertices.begin(); it != vertices.end(); ++it )
			if ( seen[*it] != yes )
				dfs(T,*it);
		for ( ++yes; !st.empty(); st.pop() )
			if ( seen[t= st.top()] != yes )
				++mark, dfs(D,t);
		bool ok= true ;
		for ( set<int> :: iterator it= vertices.begin(); it != vertices.end() && ok; ++it )
			if ( scc[*it] == scc[1^*it] )
				ok= false;
		printf("Case #%d: %s\n",++cs,ok?"YES":"NO");
	}
	return 0;
}

