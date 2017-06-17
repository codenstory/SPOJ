/*
 * MECHO
 * TOPIC: binary search, bfs with twist
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 801
#define SH 10
#define BIT(k) (1<<(k))
#define MASK(k) (BIT(k)-1)
#define enc(x,y) ((x)|((y)<<SH))
#define Q (1<<21)
#define oo 0xfffffffful
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)
#define min(x,y) ((x)<(y)?(x):(y))

int n,S,seen[Q],yes,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
unsigned int q[Q],*head,*tail,d[Q],src,dst,dist[Q], hive[Q],len;
char g[N][N];

void init() {
	unsigned int u,v;
	int i,nx,ny,x,y;
	for ( ++yes, head=tail=q, i=0; i<len; seen[*tail++=hive[i++]]=yes, d[hive[i-1]]=0 ) ; 
	for (;head<tail;) 
		for ( u=*head++, x=(u&MASK(SH)), y=(u>>SH), i = 0; i < 4; ++i )
			if ( vc(nx=x+dx[i],ny=y+dy[i]) && g[nx][ny]!='T' && g[nx][ny]!='D' ) 
				if ( seen[v = enc(nx,ny)] != yes || d[v] > d[u]+1 )
					seen[*tail++ = v] = yes, d[v] = d[u]+1;
	for ( x = 0; x < n; ++x )
		for ( y = 0; y < n; ++y )
			if ( seen[u = enc(x,y)] == yes )
				 dist[u] = d[u];
			else dist[u] = +oo;
}

int f( const unsigned int t0 ) {
	unsigned int u,v;
	int i,j,k,x,y,nx,ny;

	if ( dist[src] <= t0 ) return 0;

	for ( seen[src] = ++yes, head = tail = q, d[*tail++ = src] = 0; head < tail; ) 
		for ( u=*head++, x=(u&MASK(SH)), y=(u>>SH), i = 0; i < 4 && g[x][y] != 'D'; ++i ) 
			if ( vc(nx=x+dx[i],ny=y+dy[i]) && (seen[v = enc(nx,ny)] != yes || d[u]+1 < d[v]) )
				if ( g[nx][ny] != 'T' && t0+(d[u]+1)/S < dist[v] ) 
					d[*tail++=v]=d[u]+1, seen[v] = yes;
	return seen[dst] == yes;
}

int main() {
	int i,j,k,ts;
	unsigned int good,bad,mid;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&S); ) {
		for ( i = 0; i < n; scanf("%s",g[i++]) ) ;
		for ( src = dst = +oo, len = 0, i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == 'H' )
					hive[len++] = enc(i,j);
				else if ( g[i][j] == 'M' )
					src = enc(i,j);
				else if ( g[i][j] == 'D' )
					dst = enc(i,j);
		assert( src < +oo );
		assert( dst < +oo );
		init();
		if ( !f(good = 0) ) { puts("-1"); continue ; }
		for ( bad=n*n; good+1<bad; f(mid=(good+bad)/2)?(good=mid):(bad=mid) );
		printf("%u\n",good);
	}
	return 0;
}

