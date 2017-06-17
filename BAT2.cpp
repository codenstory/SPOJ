/*
 * BAT2
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
#define tol (1e-13)
#define N 0x80
using namespace std;
#define oo (1<<30)
enum { NONE, INC, DEC };

int n,c[N],z[N][N];

int main() {
	int i,j,k,ans,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; scanf("%d",&c[i++]) ) ; 
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j <= n; z[i][j++] = -oo ) ;
		for ( z[0][0] = 0, i = 1; i <= n; ++i )
			for ( j = 0; j <= i-1; ++j )
				if ( j == 0 || c[j] < c[i] )
					z[i][0] = max(z[i][0],z[j][0]+1);
		for ( j = 1; j <= n; ++j )
			for ( k = 0; k <= j-1; ++k )
				if ( k == 0 || c[k] > c[j] )
					z[0][j] = max(z[0][j],z[0][k]+1);
		for ( i = 1; i <= n; ++i )
			for ( j = 1; j <= n; ++j ) {
				if ( i == j ) continue ; 
				if ( i > j )
				for ( k = 0; k <= i-1; ++k )
					if ( k == 0 || c[k] < c[i] && z[k][j] > -oo )
						z[i][j] = max(z[i][j],z[k][j]+1);
				if ( j > i )
				for ( k = 0; k <= j-1; ++k )
					if ( k == 0 || c[k] > c[j] && z[i][k] > -oo )
						z[i][j] = max(z[i][j],z[i][k]+1);
			}
		for ( ans = -oo, i = 1; i <= n; ++i )
			for ( j = 1; j <= n; ++j )
				ans = max(z[i][j],ans);
		printf("%d\n",ans);
	}
	return 0;
}

