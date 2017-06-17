/*
 * DCOWS: Dancing Cows
 * TOPIC: dp, edit distance-type
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
#define N (5100)
typedef long long i64;
#define oo (1LL<<62)
using namespace std;

int m,n;
i64 b[N],c[N],z[N][N];

int main() {
	int i,j,k;
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		for ( i = 1; i <= n; scanf("%lld",&b[i++]) ) ;
		for ( j = 1; j <= m; scanf("%lld",&c[j++]) ) ;
		//assert( n < m ) ;
		sort(b+1,b+1+n), sort(c+1,c+1+m);
		/*
		for ( i = 1; i <= n-1; ++i )
			assert( b[i] <= b[i+1] );
		for ( j = 1; j <= m-1; ++j )
			assert( c[j] <= c[j+1] );
			*/
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j <= m; ++j )
				z[i][j] = +oo;
		for ( j = 0; j <= m; ++j )
			z[0][j] = 0;
		for ( i = 1; i <= n; ++i )
			for ( j = i; j <= m; ++j ) {
				z[i][j] = z[i][j-1];
				i64 w = labs(b[i]-c[j]);
				if ( z[i-1][j-1] < +oo && z[i-1][j-1]+w < z[i][j] )
					z[i][j] = z[i-1][j-1]+w;
			}
		printf("%lld\n",z[n][m]);
	}
	return 0;
}

