/*
 * ALCATRAZ3
 * TOPIC: backtracking
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
#define N 0x80
using namespace std;
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)

char g[N][N];
int m,n,moves,seen[N][N],yes,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};

int dfs( int sx, int sy, int x, int y, int len ) {
	int nx,ny,i;
	if ( x == sx && y == sy ) {
		if ( len >= moves )
			return 1;
		return 0;
	}
	if ( seen[x][y] == yes ) return 0;
	for ( seen[x][y] = yes, i = 0; i < 4; ++i )
		if ( vc(nx=x+dx[i],ny=y+dy[i]) && g[nx][ny] == '.' )
			if ( dfs(sx,sy,nx,ny,len+1) )
				return 1;
	return 0;
}

int main() {
	int i,j,k,sx,sy,ok,x,y;
	char tmp[0x20];
	for ( ;2 == scanf("%d %d",&m,&n); ) {
		scanf("%d",&moves);
		scanf("%d %d",&sx,&sy), --sx, --sy;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				scanf("%s",tmp), g[i][j] = 0[tmp];
		if ( !moves ) {
			puts("YES");
			continue ;
		}
		for ( ok = 0, i = 0; i < 4 && !ok; ++i )
			ok |= (vc(x=sx+dx[i],y=sy+dy[i]) && ++yes && dfs(sx,sy,x,y,1));
		puts(ok?"YES":"NO");
	}
	return 0;
}

