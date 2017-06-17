/*
 * BORW: Black or White
 * TOPIC: dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 201
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)<(y)?(y):(x))

int n,x[N],z[N][N],ans;

int main() {
	int i,j,k,t;
	for ( ;1 == scanf("%d",&n) && n > 0; printf("%d\n",ans) ) {
		for ( i=1; i<=n; scanf("%d",&x[i++]) );
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j <= n; ++j )
				z[i][j] = +oo;
		for ( z[0][0] = 0, i = 0; i <= n; ++i )
			for ( j = 0; j <= n; ++j )
				if ( z[i][j] < +oo ) {
					assert( i == 0 && j == 0 || i != j );
					if ( i < j ) {
						for ( k = 0, t = j+1; t <= n; ++t, ++k ) {
							if ( i == 0 || x[t] > x[i] ) 
								z[t][j] = min(z[t][j],z[i][j]+k);
							if ( j == 0 || x[t] < x[j] )
								z[i][t] = min(z[i][t],z[i][j]+k);
						}
					}
					else {
						assert( i == 0 && j == 0 || i > j ) ;
						for ( k = 0, t = i+1; t <= n; ++t, ++k ) {
							if ( i == 0 || x[t] > x[i] ) 
								z[t][j] = min(z[t][j],z[i][j]+k);
							if ( j == 0 || x[t] < x[j] )
								z[i][t] = min(z[i][t],z[i][j]+k);
						}
					}
				}
		for ( ans = +oo, i = 0; i <= n; ++i )
			for ( j = 0; j <= n; ++j )
				if ( z[i][j] < +oo )
					ans = min(ans,z[i][j]+n-max(i,j));
	}
	return 0;
}

