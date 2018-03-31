/*
 */
#include <bits/stdc++.h>
#define oo (DBL_MAX-0x400)
using namespace std;
#define N 0x80

double d[N][N],v[N],e[N],g[N][N],D[N][N];
int n;

int main() {
	int i,j,k,ts,cs= 0,qr;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&qr);
		for ( i= 0; i < n; ++i )
			scanf("%lf %lf",e+i,v+i);
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				scanf("%lf",&g[i][j]);
				if ( g[i][j] == -1 )
					g[i][j]= +oo;
				d[i][j]= g[i][j];
			}
		for ( i= 0; i < n; d[i][i]= 0, ++i ) ;
		for ( k= 0; k < n; ++k )
			for ( i= 0; i < n; ++i )
				for ( j= 0; j < n; ++j )
					if ( d[i][k] < +oo && d[k][j] < +oo )
						if ( d[i][j] > d[i][k]+d[k][j] )
							d[i][j]= d[i][k]+d[k][j];
		for ( i= 0; i < n; ++i )
			for ( D[i][i]= 0, j= i+1; j < n; ++j )
				D[j][i]= D[i][j]= +oo;
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j )
				if ( i != j && d[i][j] <= e[i] )
					D[i][j]= d[i][j]/v[i];
		for ( k= 0; k < n; ++k )
			for ( i= 0; i < n; ++i )
				for ( j= 0; j < n; ++j )
					if ( D[i][k] < +oo && D[k][j] < +oo )
						if ( D[i][j] > D[i][k]+D[k][j] )
							D[i][j]= D[i][k]+D[k][j];
		for ( printf("Case #%d:",++cs); qr-- && 2 == scanf("%d %d",&i,&j); printf(" %lf",D[--i][--j]) ) ;
		putchar('\n');
	}
	return 0;
}

