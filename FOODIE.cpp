/*
 * FOODIE
 * TOPIC: 0-1 knapsack
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
#define MAXK 0x400
using namespace std;

int n,m,t[N],z[N][MAXK];
vector<int> r[N];

int main() {
	int i,j,k,ts,ans;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( k = 1; k <= n; ++k ) {
			r[k].clear(), t[k] = 0;
			for ( scanf("%d",&j); j-- && 1 == scanf("%d",&i); r[k].push_back(i), t[k] += i ) ;
		}
		memset(z,0,sizeof z);
		for ( ans = 0, i = 1; i <= n; ++i ) 
			for ( k = 0; k <= m; ++k ) {
				if ( k >= t[i] )
					z[i][k] = max(z[i][k],z[i-1][k-t[i]]+t[i]);
				z[i][k] = max(z[i][k],z[i-1][k]);
			}
		for ( k = 0; k <= m; ans = max(ans,z[n][k++]) ) ;
		printf("%d\n",ans);
	}
	return 0;
}

