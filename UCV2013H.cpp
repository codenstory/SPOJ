/*
 * UCV2013H: Slick
 * TOPIC: floodfill
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x200)
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
using namespace std;

int m,n,c[N][N],seen[N][N],yes,a[N*N],
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
vector<pair<int,int> > vec;

int dfs( int x, int y ) {
	int nx,ny,i,res = 0;
	if ( seen[x][y] == yes || !c[x][y] )
		return res;
	for ( res = 1, seen[x][y] = yes, i = 0; i < 4; ++i )
		if ( vc(nx = x+dx[i],ny = y+dy[i]) && c[nx][ny] )
			res += dfs(nx,ny);
	return res;
}

int main() {
	int i,j,k,ts;
	for ( ;2 == scanf("%d %d",&m,&n) && (m||n); ) {
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				scanf("%d",&c[i][j]);
		memset(a,0,sizeof a);
		for ( ++yes, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( seen[i][j] != yes && c[i][j] )
					++a[dfs(i,j)];
		for ( k = 0, vec.clear(), i = 1; i <= m*n; ++i )
			if ( a[i] )
				vec.push_back(make_pair(i,a[i])), k += a[i];
		printf("%d\n",k);
		for ( i = 0; i < (int)vec.size(); ++i )
			printf("%d %d\n",vec[i].first,vec[i].second);
	}
	return 0;
}

