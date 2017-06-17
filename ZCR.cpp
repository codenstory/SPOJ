/*
 * ZCR: Zen and His Crush
 * TOPIC: DP, probability
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define N 0x400
using namespace std;

int n,m;
double p[N],z[N][N];

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( i = 1; i <= n; scanf("%lf",&p[i++]) ) ;
		memset(z,0,sizeof z);
		for ( z[0][0] = 1, i = 1; i <= n; ++i )
			for ( z[i][0] = z[i-1][0]*(1-p[i]), j = 1; j <= i && j <= m; ++j )
				z[i][j] = p[i]*z[i-1][j-1]+(1-p[i])*z[i-1][j];
		printf("%.10lf\n",z[n][m]);
	}
	return 0;
}

