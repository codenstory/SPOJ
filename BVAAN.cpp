/*
 * BVAAN
 * TOPIC: dp
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
#include <map>
#define tol (1e-13)
#define N 0x80
using namespace std;

map<int,int> z[N][N];
char x[N],y[N];
int m,n;

int main() {
	int i,j,k,ts,cs = 0,l,t,w;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%s %s",x+1,y+1), scanf("%d",&k), m = strlen(x+1), n = strlen(y+1);
		for ( i = 0; i <= m; ++i )
			for ( j = 0; j <= n; ++j )
				z[i][j].clear();
		for ( i = 0; i <= m; ++i )
			z[i][0][0] = 0;
		for ( j = 0; j <= n; ++j )
			z[0][j][0] = 0;
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j ) {
				if ( x[i] == y[j] ) {
					for ( map<int,int>::iterator it = z[i-1][j-1].begin(); it != z[i-1][j-1].end(); ++it ) {
						l = it->first, w = it->second;
						if ( z[i][j].find(l+1) == z[i][j].end() || z[i][j][l+1] < w+(int)(x[i]) ) 
							z[i][j][l+1] = w+((int)x[i]);
					}
				}
				for ( map<int,int> :: iterator it = z[i-1][j].begin(); it != z[i-1][j].end(); ++it ) {
					l = it->first, w = it->second;
					if ( z[i][j].find(l) == z[i][j].end() || z[i][j][l] < w )
						z[i][j][l] = w;
				}
				for ( map<int,int> :: iterator it = z[i][j-1].begin(); it != z[i][j-1].end(); ++it ) {
					l = it->first, w = it->second;
					if ( z[i][j].find(l) == z[i][j].end() || z[i][j][l] < w )
						z[i][j][l] = w;
				}
			}
		if ( z[m][n].find(k) == z[m][n].end() )
			puts("0");
		else printf("%d\n",z[m][n][k]);
	}
	return 0;
}

