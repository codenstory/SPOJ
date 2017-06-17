/*
 * RIOI_2_3: Path of the righteous man
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N BIT(SH)
#define SH 6
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)
#define enc(x,y,c) (((x)|((y)<<SH))|((c)<<(SH+SH)))
#define Q BIT(SH+SH+10)
#define oo 0xfffffffful
#define BIT(k) (1UL<<(k))
typedef unsigned int state;

int n,a[N][N],seen[Q],yes,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
state q[Q],*head,*tail,d[Q];

unsigned int bfs( const state src, const state dst ) {
	state u,v;
	int i,j,k,x,y,nx,ny;
	unsigned int c,nc,ans = +oo,dw;
	for ( head=tail=q, seen[*tail++=src]=++yes, d[src] = 1; head < tail; ) {
		u = *head++, x = (u&63), y = ((u>>SH)&63), c = (u>>(SH+SH));
		if ( x == (dst&63) && y == ((dst>>6)&63) ) 
			if ( d[u] < ans )
				ans = d[u];
		if ( ans <= d[u] ) continue ;
		for ( i = 0; i < 4; ++i )
			if ( vc(nx = x+dx[i],ny = y+dy[i]) ) {
				nc = c|BIT(a[nx][ny]), dw = (c&BIT(a[nx][ny]))?0:1;
				if ( seen[v=enc(nx,ny,nc)]!=yes || d[v]>d[u]+dw )
					seen[*tail++=v]=yes, d[v]=d[u]+dw;
			}
	}
	return ans;
}

int main() {
	int i,j,k,ts;
	state src,dst;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				scanf("%d",a[i]+j);
		scanf("%d %d",&i,&j), src = enc(i,j,BIT(a[i][j]));
		scanf("%d %d",&i,&j), dst = enc(i,j,0);
		printf("%u\n",bfs(src,dst));
	}
	return 0;
}

