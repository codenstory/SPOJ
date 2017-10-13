/*
 * SPIKES
 * TOPIC: BFS
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x40)
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)
#define enc(x,y,t,k) ((x) | ((y)<<6) | ((t)<<12) | ((k)<<13))
#define Q (1<<(12+1+5))
#define oo 0xfffffffful

int m,n,limit,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
char g[N][N];
unsigned int d[Q],*head,*tail,q[Q];

int main() {
	int i,j,k,t,sx,sy,x,y,nx,ny,tx,ty,ok;
	unsigned int u,v;
	for (;3==scanf("%d %d %d",&m,&n,&limit);) {
		for ( i = 0; i < m; scanf("%s",g[i++]) ) ;
		for ( sx = -1, i = 0; i < m && sx == -1; ++i )
			for ( j = 0; j < n && sx == -1; ++j )
				if ( g[i][j] == '@' ) 
					sx = i, sy = j;
		for ( tx = -1, i = 0; i < m && tx == -1; ++i )
			for ( j = 0; j < n && tx == -1; ++j )
				if ( g[i][j] == 'x' ) 
					tx = i, ty = j;
		if ( sx == -1 || tx == -1 ) {
			puts("IMPOSSIBLE");
			continue ;
		}
		for ( ok = 0, sx = 0; sx < m && !ok; ++sx )
			for ( sy = 0; sy < n && !ok; ++sy )
				if ( g[sx][sy] == '@' )
		for ( memset(d,0xff,sizeof d), head = tail = q, d[*tail++ = enc(sx,sy,0,limit)] = 0; head < tail && !ok; ) {
			u = *head++, x = (u&63), y = ((u>>6)&63), t = ((u>>12)&1), k = (u>>13);
			if ( t && g[x][y] == '@' ) {
				ok = 1;
				break ;
			}
			for ( i = 0; i < 4; ++i )
				if ( vc(nx=x+dx[i],ny=y+dy[i]) && g[nx][ny] != '#' ) {
					if ( g[nx][ny] == 's' ) {
						if ( k ) {
							v = enc(nx,ny,t,k-1);
							if ( d[v] > d[u]+1 )
								d[*tail++ = v] = d[u]+1;
						}
					}
					else if ( g[nx][ny] == '.' ) {
						v = enc(nx,ny,t,k);
						if ( d[v] > d[u]+1 )
							d[*tail++ = v] = d[u]+1;
					}
					else if ( g[nx][ny] == 'x' ) {
						if ( !t ) {
							v = enc(nx,ny,t^1,k);
							if ( d[v] > d[u]+1 )
								d[*tail++ = v] = d[u]+1;
						}
						else {
							v = enc(nx,ny,t,k);
							if ( d[v] > d[u]+1 )
								d[*tail++ = v] = d[u]+1;
						}
					}
					else {
						v = enc(nx,ny,t,k);
						if ( d[v] > d[u]+1 )
							d[*tail++ = v] = d[u]+1;
					}
				}
		}
		puts(ok?"SUCCESS":"IMPOSSIBLE");
	}
	return 0;
}

