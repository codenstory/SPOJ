/*
 * BATMAN1
 * TOPIC: dp, knapsack
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
#define N 0x20
#define MAXK 0x400
#define oo (1<<30)
using namespace std;

int m,n;
int K,cost[N],seen[2][21][21][1001],yes,
	c[N][N],w[N][N],
	z[2][21][21][1001];

int calc_z( int t, int i, int j, int k ) {
	//if ( z[t][i][j][k] > -oo )
	if ( seen[t][i][j][k] == yes )
		return z[t][i][j][k];
	seen[t][i][j][k] = yes, z[t][i][j][k] = -oo;
	if ( i == 0 ) {
		if ( t == 1 ) return -oo;
		return z[t][i][j][k] = 0;
	}
	assert( i >= 1 );
	if ( t == 0 ) 
		return z[t][i][j][k] = max(calc_z(0,i-1,n,k),calc_z(1,i-1,n,k));
	if ( k >= cost[i] ) 
		z[t][i][j][k] = max(calc_z(0,i-1,n,k-cost[i]),calc_z(1,i-1,n,k-cost[i]));
	if ( j == 0 ) {
		assert( t == 1 && j == 0 && i >= 1 );
		return z[t][i][j][k];
	}
	assert( j >= 1 );
	z[t][i][j][k] = max(z[t][i][j][k],calc_z(t,i,j-1,k));
	for ( int l = 1; l <= 1; ++l ) {
		if ( k-cost[i]-l*c[i][j] >= 0 && calc_z(0,i,j-1,k-cost[i]-l*c[i][j]) > -oo )
			z[t][i][j][k] = max(z[t][i][j][k],calc_z(0,i,j-1,k-cost[i]-l*c[i][j])+l*w[i][j]);
		if ( k-l*c[i][j] >= 0 && calc_z(1,i,j-1,k-l*c[i][j]) > -oo )
			z[t][i][j][k] = max(z[t][i][j][k],calc_z(1,i,j-1,k        -l*c[i][j])+l*w[i][j]);
		if ( k-l*c[i][j] >= 0 && calc_z(1,i,j,k-l*c[i][j]) > -oo )
			z[t][i][j][k] = max(z[t][i][j][k],calc_z(1,i,j,k-l*c[i][j])+l*w[i][j]);
	}
	return z[t][i][j][k];
}

int main() {
	int i,j,k,t,ts;
	int ans;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		scanf("%d %d %d",&m,&n,&K);
		for ( i = 1; i <= m; ++i )
			scanf("%d",cost+i);
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j )
				scanf("%d",&c[i][j]);
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j )
				scanf("%d",&w[i][j]);
		/*
		for ( t = 0; t <= 1; ++t )
			for ( i = 0; i <= m; ++i )
				for ( j = 0; j <= n; ++j )
					for ( k = 0; k <= K; ++k )
						z[t][i][j][k] = -oo;
						*/
		/*for ( ans = -oo, i = m; i >= 0; --i )
			for ( k = 0; k <= K; ans = max(ans,calc_z(1,i,n,k++)) ) ;
			*/
		for ( ans = -oo, k = K; k >= 0; --k )
			ans = max(ans,max(calc_z(0,m,n,k),calc_z(1,m,n,k)));
		printf("%d\n",ans);
	}
	return 0;
}

