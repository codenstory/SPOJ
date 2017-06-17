/*
 * GS: Going to School
 * TOPIC: probability
 * status: Accepted
 */
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol 1e-13
#define N 0x20
using namespace std;

int n,st,en,id[N];
vector<pair<int,int> > vec[N];
vector<pair<int,double> > adj[N];
double A[N][N],B[N][N],M[N][N],tmp[N][N],v[N];

bool find_pivot( int x, int y, int *r, int *c ) {
	double w = 0;
	for ( int i = x; i < n; ++i )
		for ( int j = y; j <= y; ++j )
			if ( fabs(A[i][j]) > w )
				return (w=fabs(A[*r = i][*c = j])) > 0;
	return w > 0;
}

void swap_rows( int i1, int i2 ) {
	if ( i1 == i2 ) return ;
	for ( int j = 0; j < n; ++j )
		swap(A[i1][j],A[i2][j]), swap(B[i1][j],B[i2][j]);
}

int main() {
	int i,j,k,t,ts,col,row;
	double s,cf,ans;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d",&n,&st,&en), --st, --en;
		for ( i = 0; i < n; adj[i].clear(), vec[i++].clear() ) ;
		for ( int l = 0; l < n-1; ++l ) 
			scanf("%d %d %d",&i,&j,&k), --i, --j, vec[i].push_back(make_pair(j,k)), vec[j].push_back(make_pair(i,k));
		for ( i = 0; i < n; ++i ) {
			for ( s = 0, k = 0; k < (int)vec[i].size(); s += vec[i][k++].second ) ;
			for ( k = 0; k < (int)vec[i].size(); adj[i].push_back(make_pair(vec[i][k].first,vec[i][k].second/s)), ++k ) ;
		}
		for ( memset(A,0,sizeof A),memset(B,0,sizeof B), i = 0; i < n; ++i ) 
			for ( int l = 0; i != en && l < (int)adj[i].size(); ++l ) {
				j = adj[i][l].first;
				A[j][i] += adj[i][l].second;
			}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				B[i][j]=(i==j?1:0);
		/*
		for ( i = 0; i < n; ++i ) {
			for ( j = 0; j < n; ++j )
				printf("%.4lf ",A[i][j]);
			puts("");
		}
		*/


		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				M[i][j]=A[i][j],A[i][j]=(B[i][j]-A[i][j]);

		/*
		for ( i = 0; i < n; ++i ) {
			for ( j = 0; j < n; ++j )
				printf("%.4lf ",A[i][j]);
			puts("");
		}
		*/

		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( tmp[i][j] = 0, k = 0; k < n; ++k )
					tmp[i][j] += A[i][k]*A[k][j];
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				A[i][j] = tmp[i][j];

		/*
		for ( i = 0; i < n; ++i ) {
			for ( j = 0; j < n; ++j )
				printf("%.4lf ",A[i][j]);
			puts("");
		}
		*/

		for ( k = 0; k < n; ++k ) {
			assert( find_pivot(k,k,&row,&col) );
			swap_rows(row,k);
			//printf("A[%d][%d] = %.20lf\n",k,k,A[k][k]);
			assert( fabs(A[k][k]) >= tol );
			for ( cf = A[k][k], j = 0; j < n; ++j )
				A[k][j] /= cf, B[k][j] /= cf;
			assert( fabs(A[k][k]-1.00) < tol );
			for ( i = k+1; i < n; ++i ) {
				for ( cf = A[i][k], j = 0; j < n; ++j )
					A[i][j] -= cf*A[k][j], B[i][j] -= cf*B[k][j];
				assert( fabs(A[i][k]) < tol );
			}
		}
		for ( i = n-1; i >= 0; --i ) 
			for ( t = i-1; t >= 0; --t ) 
				for ( cf = A[t][i], j = 0; j < n; ++j )
					A[t][j] -= cf*A[i][j], B[t][j] -= cf*B[i][j];
		/*
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( tmp[i][j] = 0, k = 0; k < n; ++k )
					tmp[i][j] += A[i][k]*B[k][j];
		for ( i = 0; i < n; ++i ) 
			for ( j = 0; j < n; ++j )
				if ( i == j ) {
					printf("This i  = %d, %lf\n",i,A[i][j]);
					assert( fabs(A[i][j]-1) < tol );
					printf("This i  = %d, %lf\n",i,tmp[i][j]);
					assert( fabs(tmp[i][j]-1) < tol );
				}
				else {
					assert( fabs(A[i][j]) < tol );
					assert( fabs(tmp[i][j]) < tol );
				}
				*/
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( tmp[i][j] = 0, k = 0; k < n; ++k )
					tmp[i][j] += M[i][k]*B[k][j];
		for ( i = 0; i < n; ++i )
			v[i] = (i==st?1:0);
		for ( ans = 0, j = 0; j < n; ++j )
			ans += tmp[en][j]*v[j];
		printf("%.5lf\n",ans);
	}
	return 0;
}

