/*
 * SKYLINE
 * TOPIC: dp, invariants
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x400
#define MOD (1000000LL)
#define oo (1LL<<30)
typedef long long i64;

int n;
i64 z[N][N],w[N],pref[N][N];

int main() {
	int i,j,k;
	for ( w[1] = pref[1][1] = z[1][1] = 1LL, n = 2; n < N; ++n ) {
		for ( k = 1; k <= n-1; ++k ) {
			for ( z[n][k] = w[n-1]-(k-2<0?0:pref[n-1][k-2]); z[n][k] < 0; z[n][k] += MOD ) ;
			z[n][k] %= MOD, pref[n][k] = (pref[n][k-1]+z[n][k])%MOD;
		}
		w[n]=pref[n][n]=(pref[n][n-1]+(z[n][n]=1))%MOD;
	}
	for (;1==scanf("%d",&n)&&n&&printf("%lld\n",w[n]); ) ;
	return 0;
}

