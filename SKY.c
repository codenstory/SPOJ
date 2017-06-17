/*
 * SKY: Sky Lift
 * TOPIC: bfs, binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N BIT(SH)
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)
#define SH 10
#define BIT(k) (1<<(k)) 
#define MASK(k) (BIT(k)-1)
#define enc(x,y) ((x)|((y)<<SH))
#define Q (BIT(21))
typedef long long i64;

int n,seen[Q],yes,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
unsigned int q[Q],*head,*tail;
i64 h[Q];

int bfs( unsigned int src, unsigned int sink, int lowest_level ) {
	unsigned int u,v;
	int i,x,y,nx,ny;
	for ( head=tail=q, seen[*tail++=src]=++yes; head < tail && seen[sink] != yes; )
		for ( u = *head++, x=(u&MASK(SH)), y = (u>>SH), i = 0; i < 4; ++i ) 
			if ( vc(nx=x+dx[i],ny=y+dy[i]) && h[v=enc(nx,ny)] >= lowest_level && seen[v] != yes )
				seen[*tail++ = v] = yes;
	return seen[sink] == yes;
}

int main() {
	int i,j,k,ts,xa,xb,ya,yb;
	unsigned int src,sink;
	i64 A,B,C,good,bad,mid;
	for (;1==scanf("%d",&n);) {
		scanf("%d %d %d %d",&xa,&ya,&xb,&yb), --xa, --xb, --ya, --yb;
		src = enc(xa,ya), sink = enc(xb,yb);
		scanf("%lld %lld %lld",&A,&B,&C);
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				h[enc(i,j)] = ((i*n+j)*A+B)%C;
		for ( good = 0, bad = h[src]+1; good+1<bad; bfs(src,sink,mid=(good+bad)/2)?(good=mid):(bad=mid) ) ;
		printf("%lld\n",h[src]-good+h[sink]-good);
	}
	return 0;
}

