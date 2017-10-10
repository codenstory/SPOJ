/*
 * RPLB
 * TOPIC: DP
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
using namespace std;
#define N (0x400)
#define K (0x400)
#define oo (1<<30)

int n,m,c[N],yes,seen[N][2][K],
	z[N][2][K];

int calc_z( int i, int t, int k ) {
	if ( seen[i][t][k] == yes )
		return z[i][t][k];
	seen[i][t][k] = yes;
	if ( i == 1 ) {
		if ( t == 0 )
			return z[i][t][k] = (k==0);
		return z[i][t][k] = (c[i]==k);
	}
	assert( i >= 2 );
	if ( t == 0 ) 
		return z[i][t][k] = calc_z(i-1,0,k)||calc_z(i-1,1,k);
	return z[i][t][k] = (c[i] <= k && calc_z(i-1,t^1,k-c[i]));
}

int main() {
	int i,j,k,ts,cs = 0,t,ans;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		printf("Scenario #%d: ",++cs), scanf("%d %d",&n,&m);
		for ( i = 1; i <= n; scanf("%d",&c[i++]) ) ;
		for ( ans = 0, j = m; j >= 0 && !ans; --j )
			for ( t = 0; t <= 1 && !ans; ++t )
				if ( calc_z(n,t,j) ) 
					ans = j;
		printf("%d\n",ans);
	}
	return 0;
}

