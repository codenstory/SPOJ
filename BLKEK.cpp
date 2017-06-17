/*
 * BLKEK: Emoticons
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
#define N 0x800
typedef long long i64;
using namespace std;

int n;
i64 z[N][4];
char x[N];

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%s",x+1), n = strlen(x+1);
		memset(z,0,sizeof z);
		for ( z[0][0] = 1, i = 1; i <= n; ++i ) {
			for ( j = 0; j < 4; ++j )
				z[i][j] = z[i-1][j];
			if ( x[i] == 'K' )
				z[i][1] += z[i-1][0], z[i][3] += z[i-1][2];
			if ( x[i] == 'E' )
				z[i][2] += z[i-1][1];
		}
		printf("%lld\n",z[n][3]);
	}
	return 0;
}

