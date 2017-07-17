/*
 * LARSUBP
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
#define N (1<<14)
#define MOD (1000000000LL+7LL)
#define A 10
typedef long long i64;
using namespace std;

int n,c[N];
i64 z[N],dp[A][N],w;
char s[N];

int main() {
	int i,j,k,ts,cs = 0;
	for ( scanf("%d",&ts); ts-- && scanf("%s",s+1) == 1; ) {
		for ( n = 1; s[n]; c[n] = s[n]-'0', ++n ) ;
		--n, memset(dp,0,sizeof dp);
		for ( z[1] = 1, dp[c[1]][1] = 1, i = 2; i <= n; ++i ) {
			for ( z[i] = z[i-1], k = 0; k < c[i]; ++k )
				z[i] += dp[k][i-1], z[i] %= MOD;
			++z[i], z[i] %= MOD;
			for ( w = 1, k = 0; k < c[i]; ++k )
				w += dp[k][i-1], w %= MOD;
			for ( k = 0; k < A; ++k )
				if ( k != c[i] )
					dp[k][i] = dp[k][i-1];
				else dp[k][i] = (dp[k][i-1]+w)%MOD;
		}
		printf("Case %d: %lld\n",++cs,z[n]);
	}
	return 0;
}

