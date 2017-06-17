/*
 * KOZE: Sheep
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x100
#define enc(x,y) ((x)|((y)<<8))
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)

int m,n,seen[N][N],yes,wolf,sheep,
	num[N][N],mark,w[N*N],s[N*N];
char g[N][N],
	 dx[] = {-1,0,1,0},
	 dy[] = {0,1,0,-1};

void floodfill( int x, int y ) {
	int nx,ny,i;
	if ( seen[x][y] == yes )
		return ;
	if ( g[x][y] == 'k' ) ++s[mark];
	else if ( g[x][y] == 'v' ) ++w[mark];
	for ( seen[x][y] = yes, num[x][y] = mark, i = 0; i < 4; ++i )
		if ( vc(nx = x+dx[i],ny = y+dy[i]) && seen[nx][ny] != yes )
			if ( g[nx][ny] != '#' ) 
				floodfill(nx,ny);
}

int main() {
	int i,j,k;
	while ( 2 == scanf("%d %d",&m,&n) ) {
		memset(g,'.',sizeof g);
		for ( i = 1; i <= m; scanf("%s",g[i++]+1), g[i-1][n+1] = '.' ) ;
		memset(num,0,sizeof num), mark = 0;
		m += 2, n += 2;
		for ( ++yes, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( seen[i][j] != yes && g[i][j] != '#' )
					++mark, floodfill(i,j);
		assert( mark >= 1 );
		for ( sheep = s[1], wolf = w[1], i = 2; i <= mark; ++i )
			if ( s[i] > w[i] )
				sheep += s[i];
			else wolf += w[i];
		printf("%d %d\n",sheep,wolf);
	}
	return 0;
}

