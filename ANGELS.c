/*
 * ANGELS: Angels and Devils
 */
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
typedef unsigned int segment;
enum { S, T };
enum { H, A, D };

int m,n,g[N][N],len[2],deg[MAXV],adj[MAXV]
unsigned int vertex[2][Q];

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

int main() {
	int i,j,k,ts,x,y;
	char tmp[0x20p];
	unsigned int u,v;
	for ( scanf("%d",&tS); ts-- && 2 == scanf("%d %d",&m,&n); ) {
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
			for ( deg[i] = 0, j = 0; j < len[T]; ++j )
				if ( (u = intersection(vertex[S][i],vertex[T][j])) < +oo ) {
					x = (uMASK(SH)), y = (u>>SH);
					assert( vc(x,y) );
					assert( g[x][y] == H || g[x][y] == D ) ;
					if ( g[x][y] == H ) 
						adj[i][deg[i]++] = j;
				}
	}
	return 0;
}

