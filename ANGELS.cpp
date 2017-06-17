/*
 * ANGELS: Angels and Devils
 * TOPIC: bipartite matching
 * status: Accepted
 */
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH 9
#define N 301
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define enc(x,y) ((x)|((y)<<SH))
#define e(t,x,y,z) ((t)|(enc(x,y)<<1)|((z)<<(SH+SH+1)))
#define Q (1<<20)
#define oo (1<<30)
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define MAXV (N*(N-1)/2)
#define NEXT(x)( ((x)+1)&(Q-1ULL))
#define PREV(x)( ((x)+Q-1)&(Q-1ULL))
typedef unsigned int segment;
enum { S, T };
enum { H, A, D };
#define NONE (-1)
using namespace std;

int m,n,g[N][N],len[2],mate[2][MAXV],seen[MAXV],yes,
	head,tail,q[Q],cnt;
unsigned int vertex[2][Q];
vector<int> adj[MAXV];

segment intersection( segment a, segment b ) {
	int x,y,z,t,
		p,q,r,s;
	assert( (a&1)^(b&1) );
	assert( S == (a&1) );
	a >>= 1, b >>= 1;
	x = (a&MASK(SH)), y = ((a>>SH)&MASK(SH)), z = x, t = ((a>>(SH+SH))&MASK(SH));
	p = (b&MASK(SH)), q = ((b>>SH)&MASK(SH)), r = ((b>>(SH+SH))&MASK(SH)), s = q;
	if ( p <= x && z <= r && y <= q && s <= t ) 
		return enc(x,q);
	return +oo;
}

int alt_path( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x] = yes, i = 0; i < (int)adj[x].size(); ++i )
		if ( mate[T][y = adj[x][i]] == NONE || alt_path(mate[T][y]) ) {
			mate[S][mate[T][y] = x] = y;
			return 1;
		}
	return 0;
}

int bpm() {
	int x,y,i,k;
	bool flag;
	for ( cnt = 0, head = tail = 0, x = 0; x < len[S]; ++x )
		q[tail] = x, ++cnt, tail = NEXT(tail);
	for ( i = S; i <= T; ++i )
		for ( x = 0; x < len[i]; mate[i][x++] = NONE ) ;
	do {
		for ( flag = false, k = cnt, ++yes; k--; ) {
			x = q[head], head = NEXT(head), --cnt;
			assert( mate[S][x] == NONE );
			if ( alt_path(x) ) 
				flag = true;
			else q[tail] = x, tail = NEXT(tail), ++cnt;
		}
	} while ( flag ) ;
	return len[S]-cnt;
}

int main() {
	int i,j,k,ts,x,y;
	char tmp[0x20];
	unsigned int u,v;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; scanf("%s",tmp), g[i][j] = 0[tmp]=='H'?H:(0[tmp]=='A'?A:D), ++ j ) ;
		for ( len[S] = 0, i = 0; i < m; ++i ) {
			for ( j = 0; j < n && g[i][j] == A; ++j ) ;
			for ( ;j < n; j = k ) {
				assert( g[i][j] != A );
				for ( k = j+1; k < n && g[i][k] != A; ++k ) ;
				vertex[S][len[S]++] = e(S,i,j,k-1);
				for ( ;++k < n && g[i][k] == A; ) ;
			}
		}
		for ( len[T] = 0, j = 0; j < n; ++j ) {
			for ( i = 0; i < m && g[i][j] == A; ++i ) ;
			for ( ;i < m; i = k ) {
				assert( g[i][j] != A );
				for ( k = i+1; k < m && g[k][j] != A; ++k ) ;
				vertex[T][len[T]++] = e(T,i,j,k-1);
				for ( ;++k < m && g[k][j] == A; ) ;
			}
		}
		for ( i = 0; i < len[S]; ++i )
			for ( adj[i].clear(), j = 0; j < len[T]; ++j )
				if ( (u = intersection(vertex[S][i],vertex[T][j])) < +oo ) {
					x = (u&MASK(SH)), y = (u>>SH);
					assert( vc(x,y) );
					assert( g[x][y] == H || g[x][y] == D ) ;
					if ( g[x][y] == H ) 
						adj[i].push_back(j);
				}
		printf("%d\n",bpm());
	}
	return 0;
}

