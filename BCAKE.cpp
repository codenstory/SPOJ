/*
 * BCAKE: Birthday Cake
 * TOPIC: maxflow, dp
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
#define N 0x40
using namespace std;

int m,n,K,c[N][N],ans;
char g[N][N];

int main() {
	int i,j,k,t,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&m,&n,&K);
		for ( i = 1; i <= m; scanf("%s",g[i++]+1) ) ;
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j )
				c[i][j] = (g[i][j]=='C'?1:0)+c[i-1][j]+c[i][j-1]-c[i-1][j-1];
		for ( ans = m*n+1, i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j )
				for ( k = i; k <= m; ++k )
					for ( t = j; t <= n; ++t ) 
						if ( c[k][t]-c[i-1][t]-c[k][j-1]+c[i-1][j-1] == K )
							ans = min(ans,(k-i+1)*(t-j+1));
		printf("%d\n",ans<m*n+1?ans:-1);
	}
	return 0;
}

