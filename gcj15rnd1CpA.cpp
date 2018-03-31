/*
 */
#include <bits/stdc++.h>
#define N 21
#define W N
#define oo (1<<29)
using namespace std;

int z[N][W],e[N][W];

int g( int n, int w ) {
	if ( n < w )
		return 0;
	if ( e[n][w] < +oo )
		return e[n][w];
	if ( n == w )
		return e[n][w]= 1;
	for ( int t= 1; t <= n; ++t ) {
		int left= t-1, right= n-t;

		if ( left < w && right < w ) 
			return e[n][w]= min(1,e[n][w]);

		e[n][w]= min(g(left,w)+g(right,w)+1,e[n][w]);
	}
	return e[n][w];
}

int f( int n, int w ) {
	if ( n < w )
		return +oo;
	if ( z[n][w] < +oo )
		return z[n][w];
	if ( n == w )
		return z[n][w]= w;
	for ( int t= 1; t <= n; ++t ) {
		int left= t-1, right= n-t;
		if ( left < w && right < w ) {
			z[n][w]= min(z[n][w],right+left+1);
			continue ;
		}
		int lz= left >= w ? max(0,right-(w-1))+f(left,w) : f(right,w),
			rz= right >= w ? max(0,left-(w-1))+f(right,w): f(left,w);
		assert( lz < +oo && rz < +oo );
		int zz= max(lz,rz)+1;
		z[n][w]= min(zz,z[n][w]);
	}
	return z[n][w];
}

int main() {
	int i,j,k,ts,cs= 0,m,n,w;
	for ( scanf("%d",&ts); ts-- && 3 == scanf("%d %d %d",&m,&n,&w); ) {
		for ( i = 0; i <= n; ++i )
			for ( j= 0; j <= w; e[i][j]= +oo, z[i][j++]= +oo ) ;
		int ans= f(n,w);
		printf("Case #%d: %d\n",++cs,(m-1)*g(n,w)+ans);
	}
	return 0;
}

