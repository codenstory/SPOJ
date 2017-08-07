/*
 * WACHOVIA
 * TOPIC: knapsack
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
#define MAXK 0x400
#define N 0x40
using namespace std;

int m,n,w[N],v[N],
	z[N][MAXK],ans;

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%d %d",w+i,v+i);
		memset(z,0,sizeof z);
		for ( ans = 0, i = 1; i <= n; ++i ) {
			for ( k = 0; k+w[i] <= m; ++k ) {
				ans = max(ans,z[i][k+w[i]] = max(z[i][k+w[i]],z[i-1][k]+v[i]));
				ans = max(ans,z[i][k]);
			}
			for ( k = 0; k <= m; ++k )
				ans = max(ans,z[i][k] = max(z[i][k],z[i-1][k]));
		}
		printf("Hey stupid robber, you can get %d.\n",ans);
	}
	return 0;
}

