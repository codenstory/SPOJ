/*
 * B. Arcade!
 * TOPIC: math expectation, Gaussian Elimination, inverse of a matrix
 * NOTES: NAQ 2016
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 32
#define tol 1e-13
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<=(x))

int payoff[N*N],m,n,sink,id[N][N],
	di[] = {-1,-1,1,1},
	dj[] = {-1,0,0,1};
double A[N*N][N*N],M[N*N][N*N],X[N*N][N*N],t[N*N][N*N],originalM[N*N][N*N];

int find_pivot( int k, int *r ) {
	int i;
	double w = 0;
	for ( i = k; i < n; ++i )
		if ( fabs(M[i][k]) > w )
			w = fabs(M[*r = i][k]);
	return w >= tol;
}

void swap_rows( int i1, int i2 ) {
	int j;
	double tmp;
	if ( i1 == i2 ) return ;
	for ( j = 0; j < n; ++j ) {
		tmp = M[i1][j], M[i1][j] = M[i2][j], M[i2][j] = tmp;
		tmp = X[i1][j], X[i1][j] = X[i2][j], X[i2][j] = tmp;
	}
}

int main() {
	int i,j,k,l,ni,nj,x,y;
	double prob,tot,cf,ans;
	for ( ;1 == scanf("%d",&m); ) {
		for ( n = 0, i = 0; i < m; ++i ) 
			for ( j = 0; j <= i; scanf("%d",&payoff[id[i][j++]=n++]) ) ;
		for ( sink = n++, i = 0; i < n; ++i )
			for ( j = 0; j < n; A[i][j++] = 0 ) ;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j <= i; ++j ) {
				for ( x = id[i][j], tot = 0, l = 0; l < 4; ++l ) {
					scanf("%lf",&prob), tot += prob;
					if ( !vc(ni=i+di[l],nj=j+dj[l]) ) {
						assert( fabs(prob) < tol );
						continue ;
					}
					y = id[ni][nj], A[x][y] = prob;
				}
				scanf("%lf",&prob), tot += (A[x][sink] = prob);
				assert( fabs(tot-1) < tol );
			}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; originalM[i][j] = M[i][j] = (X[i][j]=(i==j?1:0))-A[i][j], ++j ) ;
		for ( k = 0; k < n; ++k ) {
			assert( find_pivot(k,&l) );
			for ( swap_rows(k,l), i = k+1; i < n; ++i ) {
				for ( cf = M[i][k]/M[k][k], j = 0; j < n; ++j )
					M[i][j] -= cf*M[k][j], X[i][j] -= cf*X[k][j];
				for ( cf = M[k][k], j = 0; j < n; ++j )
					M[k][j] /= cf, X[k][j] /= cf;
			}
		}
		for ( k = n-1; k >= 0; --k ) 
			for ( i = k-1; i >= 0; --i ) 
				for ( cf = M[i][k], j = 0; j < n; ++j )
					M[i][j] -= cf*M[k][j], X[i][j] -= cf*X[k][j];
		for ( ans = 0, x = 0; x < n-1; ++x )
			ans += X[0][x]*A[x][sink]*payoff[x];
		printf("%.7lf\n",ans);
	}
	return 0;
}

