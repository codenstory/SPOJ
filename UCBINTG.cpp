/*
 * UCBINTG
 * TOPIC: ad hoc
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
#define N 11
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
using namespace std;

int m,n,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
char g[N][N],e[N][N];

int main() {
	int i,j,k,l,up,down,left,right;
	for ( ;2 == scanf("%d %d",&m,&n) && (m||n); ) {
		for ( i = 0; i < m; scanf("%s",g[i++]) ) ;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j ) {
				e[i][j] = '.';
				if ( g[i][j] == 'X' ) {
					for ( k = 0, l = 0; l < 4; ++l )
						if ( !vc(i+dx[l],j+dy[l]) || g[i+dx[l]][j+dy[l]] == '.' )
							++k;
					if ( k >= 3 ) e[i][j] = '.';
					else e[i][j] = 'X';
				}
			}
		for ( up = m, i = 0; i < m; ++i ) {
			for ( j = 0; j < n && e[i][j] == '.'; ++j ) ;
			if ( j < n ) {
				up = i;
				break ;
			}
		}
		for ( down = -1, i = m-1; i >= 0; --i ) {
			for ( j = 0; j < n && e[i][j] == '.'; ++j ) ;
			if ( j < n ) {
				down = i;
				break ;
			}
		}
		for ( left = n, j = 0; j < n; ++j ) {
			for ( i = 0; i < m && e[i][j] == '.'; ++i ) ;
			if ( i < m ) {
				left = j;
				break ;
			}
		}
		for ( right = -1, j = n-1; j >= 0; --j ) {
			for ( i = 0; i < m && e[i][j] == '.'; ++i ) ;
			if ( i < m ) {
				right = j;
				break ;
			}
		}
		for ( i = up; i <= down; ++i ) {
			for ( j = left; j <= right; ++j )
				putchar(e[i][j]);
			puts("");
		}
	}
	return 0;
}

