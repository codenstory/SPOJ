/*
 * ADFRUITS: Advanced Fruits
 * TOPIC: dp
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
using namespace std;
#define N 0x80
#define oo (1<<30)

char x[N],y[N];
int m,n,z[N][N];
pair<int,int> way[N][N];

int main() {
	int i,j,k;
	for ( ;2 == scanf("%s %s",x+1,y+1); ) {
		m = strlen(x+1), n = strlen(y+1);
		for ( i = 0; i <= m; ++i )
			for ( j = 0; j <= n; ++j )
				z[i][j] = +oo;
		for ( i = 1; i <= m; ++i )
			z[i][0] = i;
		for ( j = 1; j <= n; ++j )
			z[0][j] = j;
		for ( z[0][0] = 0, i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j ) {
				if ( x[i] == y[j] ) {
					z[i][j] = min(z[i-1][j],z[i][j-1]);
					if ( z[i][j] == z[i-1][j] )
						way[i][j] = make_pair(i-1,j);
					else way[i][j] = make_pair(i,j-1);
				}
				if ( z[i][j] > 1+z[i-1][j] )
					z[i][j] = 1+z[i-1][j], way[i][j] = make_pair(i-1,j);
				if ( z[i][j] > 1+z[i][j-1] )
					z[i][j] = 1+z[i][j-1], way[i][j] = make_pair(i,j-1);
			}
		printf("%d\n",z[m][n]);
	}
	return 0;
}

