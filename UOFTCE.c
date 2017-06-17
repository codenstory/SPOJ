/*
 * UOFTCE: A Brief Expedition
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH 7
#define N (1<<SH)
#define enc(x,y) ((x)|((y)<<SH))
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define Q (1<<21)

unsigned int q[Q],*head,*tail,*mark,src,dist;
int seen[Q],m,n,K,yes;
char g[N][N],
	 dx[] = {-1,0,1,0},
	 dy[] = {0,1,0,-1};

int main() {
	int i,j,k,ts,x,y,nx,ny,time;
	unsigned int u,v,mx;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&m,&n);
		for ( i = 0; i < m; scanf("%s",g[i++]) );
		for ( src = -1, K = 0, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == 'C' ) 
					src = enc(i,j);
		assert( src != -1 );
		for ( mx=0, dist=0, time = 0, K=0, head=tail=q, seen[*tail++ = src] = ++yes, mark = tail+1; head < tail; ) {
			u = *head++, x = (u&127), y = (u>>SH);
			if ( head == mark )
				++dist, mark = tail+1;
			if ( g[x][y] == 'S' ) {
				time += dist, ++K;
				if ( dist > mx ) 
					mx = dist;
			}
			for ( i = 0; i < 4; ++i )
				if ( vc(nx = x+dx[i], ny = y+dy[i]) && seen[v = enc(nx,ny)] != yes )
					if ( g[nx][ny] != '#' ) 
						seen[*tail++ = v] = yes;
		}
		printf("%u\n",time*2-mx+60*K);
	}
	return 0;
}

