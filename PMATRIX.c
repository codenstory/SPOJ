/*
 * PMATRIX
 * TOPIC: scc
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<15)
#define MAXE (N<<1)
enum { D, T };

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int E[2],last[2][N],next[2][MAXE],to[2][MAXE],st[N],*top,scc[N],mark,
	seen[N],yes,outdeg[N],indeg[N],n,m;

void add_arcs( int x, int y ) {
	int i = E[D]++, j = E[T]++;
	to[D][i] = y, next[D][i] = last[D][x], last[D][x] = i;
	to[T][j] = x, next[T][j] = last[T][y], last[T][y] = j;
}

void dfs( const int t, int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = last[t][x]; i >= 0; i = next[t][i] )
		if ( seen[y = to[t][i]] != yes )
			dfs(t,y);
	t?(*++top=x):(scc[x]=mark);
}

void traverse( int x ) {
	int i,y;
	assert( seen[x] != yes );
	for ( seen[x] = yes, i = last[D][x]; i >= 0; i = next[D][i] ) {
		if ( seen[y = to[D][i]] != yes )
			traverse(y);
		if ( scc[x] != scc[y] )
			++indeg[scc[y]], ++outdeg[scc[x]];
	}
}

int main() {
	int i,j,k,l,t,ts = getnum();
	for (;ts--;) {
		n = getnum(), m = getnum();
		for ( t = D; t <= T; ++t )
			for ( E[t] = 0, i = 0; i < n; last[t][i++] = -1 ) ;
		for ( l = 0; l < m; ++l ) {
			i = getnum()-1, j = getnum()-1;
			add_arcs(i,j);
		}
		for ( ++yes, top = st, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				dfs(T,i);
		for ( ++yes, mark = -1; top > st; )
			if ( seen[i = *top--] != yes )
				++mark, dfs(D,i);
		memset(indeg,0,sizeof indeg), memset(outdeg,0,sizeof outdeg);
		for ( ++yes, i = 0; i < n; ++i )
			if ( seen[i] != yes )
				traverse(i);
		for ( k = l = 0, i = 0; i <= mark; ++i ) {
			if ( !outdeg[i] ) ++k;
			if ( !indeg[i] ) ++l;
		}
		if ( mark == 0 ) k = l = 0;
		printf("%d\n",k<l?l:k);
	}
	return 0;
}

