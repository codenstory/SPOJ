
/*
 * ANTTT: The Ant
 * TOPIC: union-find, segment intersection
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
#define tol (1e-13)
#define N 0x400
//typedef long long int;
using namespace std;

typedef struct cell {
	int x,y;
	cell( int x, int y ) {
		this->x = x, this->y = y;
	}
	cell() { x=y=0; }
} cell;

inline cell operator - ( const cell &a, const cell &b ) {
	return cell(a.x-b.x,a.y-b.y);
}

inline int operator , ( const cell &a, const cell &b ) {
	return a.x*b.x+a.y*b.y;
}

inline int operator ^ ( const cell &a, const cell &b ) {
	return a.x*b.y-a.y*b.x;
}

inline bool on_segment( const cell &p, const cell &a, const cell &b ) {
		/*
	cell u = a-p, v = b-p;
	return (u^v) == 0 && (u,v) <= 0;*/
	int A = a.x-p.x, B = a.y-p.y,
		C = b.x-p.x, D = b.y-p.y;
	return A*D==B*C && A*C+B*D <= 0;
}

inline bool f( const cell &a, const cell &b, const cell &p, const cell &q ) {
	int det, dt, dtau,
		   A = q.x-p.x, B = -(b.x-a.x),
		   C = q.y-p.y, D = -(b.y-a.y),
		   Q = a.x-p.x, R = a.y-p.y;
	double t,tau;

	det = A*D-B*C, dt = Q*D-B*R, dtau = A*R-Q*C;
	if ( det == 0 ) 
		return on_segment(a,p,q) || on_segment(b,p,q) || on_segment(p,a,b) || on_segment(q,a,b);
	else {
		t = ((double)dt)/det, tau = ((double)dtau)/det;
		return 0 <= t && t <= 1 && 0 <= tau && tau <= 1;
	}
}

int n,rank[N],p[N],g[N][N],yes,
	scc[N],mark,seen[N],deg[N],adj[N][N];
cell a[N],b[N];

inline void link( int x, int y ) {
	if ( rank[x] > rank[y] )
		p[y] = x;
	else {
		p[x] = y;
		if ( rank[x] == rank[y] ) 
			++rank[y];
	}
}

inline int find( int x ) {
	return p[x] = (p[x] == x?x:find(p[x]));
}

inline void merge( int x, int y ) {
	link(find(x),find(y));
}

int st[N],*top;

inline void dfs( int x ) {
	int i;
	if ( seen[x] == yes )
		return ;
	for ( seen[x] = yes, scc[x] = mark, i = 0; i < deg[x]; ++i )
		dfs(adj[x][i]);
}

inline int getint() {
	int ch,n=0,sign=1;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch == '-' || '0'<=ch && ch<='9'); );
	if ( ch == '-' ) 
		sign = -1, ch = getchar_unlocked();
	for ( n = ch-'0'; (ch = getchar_unlocked()) != EOF && '0'<=ch && ch <= '9'; n = 10*n+ch-'0' );
	return sign*n;
}

#include <set>

int x[N],y[N];
set<int> s;

int main() {
	int i,j,k,ts,qr;
	for ( ts = getint(); ts--; ) {
		for ( n = getint(), qr = getint(), i = 0; i < n; ++i )
			rank[p[i]=i]=0;
		for ( i = 0; i < n; ++i ) 
			a[i].x = getint(), a[i].y = getint(), b[i].x = getint(), b[i].y = getint();
		for ( i = 0; i < n; ++i )
			for ( j = i+1; j < n; ++j )
				if ( f(a[i],b[i],a[j],b[j]) )
					if ( find(i) != find(j) )
						merge(i,j);
		for ( ;qr--; i = getint(), j = getint(), puts(find(--i)==find(--j)?"YES":"NO") ) ;
		/*
		for ( s.clear(), k = 0, ++yes; k < qr; ++k ) {
			i = getint()-1, j = getint()-1;
			x[k] = i, y[k] = j;
			s.insert(x[k]), s.insert(y[k]);
		}
		for ( set<int>::iterator it = s.begin(); it != s.end(); ++it )
			if ( seen[*it] != yes )
				++mark, dfs(*it);
		for ( k = 0; k < qr; ++k ) {
			puts(scc[x[k]] == scc[y[k]]?"YES":"NO");
		}
		*/
	}
	return 0;
}

