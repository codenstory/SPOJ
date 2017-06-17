/*
 * GOODB: Good Predictions
 * TOPIC: combinatorics
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
#define MOD (1000000007LL)
typedef long long i64;

i64 c[N][N];

int main() {
	int i,j,k,n;
	for ( i = 0; i < N; ++i )
		c[i][0] = 1;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			c[i][j] = c[i-1][j]+c[i-1][j-1], c[i][j] %= MOD;
	while ( 4 == scanf("%d %d %d %d",&n,&i,&j,&k) )
		printf("%lld\n",(c[i+j+k][i]*c[j+k][k])%MOD);
	return 0;
}

