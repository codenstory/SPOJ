/*
 * NATALIAG
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)
#define enc(x,y) ((x)|((y)<<7))
#define Q (1<<21)
#define oo 0xfffffffful

char g[N][N];
int m,n,seen[Q],yes,
   	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
unsigned int q[Q],*head,*tail,*mark,dist;

int main() {
	int i,j,k,ts,x,y,nx,ny;
	unsigned int src,dst,u,v,ans;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i = 0; i < m; scanf("%s",g[i++]) );
		for ( src = dst = +oo, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == '$' ) src = enc(i,j);
				else if ( g[i][j] == '#' ) dst = enc(i,j);
		if ( src == +oo || dst == +oo ) {
			puts("-1");
			continue ;
		}
		assert( src < +oo );
		assert( dst < +oo );
		for ( dist = 0, ans = +oo, head=tail=q, seen[*tail++=src]=++yes, mark=tail+1; head < tail; ) {
			u = *head++, x = (u&127), y = (u>>7);
			if ( head == mark ) 
				++dist, mark = tail+1;
			if ( u == dst ) { ans = dist; break ; }
			for ( i = 0; i < 4; ++i )
				if ( vc(nx = x+dx[i],ny = y+dy[i]) && g[nx][ny] != '*' && seen[v = enc(nx,ny)] != yes ) 
					seen[*tail++ = v] = yes;
		}
		if ( ans < +oo ) printf("%u\n",ans);
		else puts("-1");
	}
	return 0;
}

