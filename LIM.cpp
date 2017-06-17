/*
 * LIM: Lost in Madrid
 * TOPIC: matrix, probability, gaussian elimination
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#define N 302
#define is_booth(x) (booth[x] == yes)
#define SRC 0
#define BOOTH 291
#define DEST 300
#define tol 1e-13
using namespace std;

int n,adj[N][N],deg[N],src,dest;
double g[N][N],A[N][N],B[N][N],t[N][N];
map<int,int> m;
set<int> s;
vector<pair<int,int> > vec;
int booth[N],yes;

bool find_pivot( int x, int y, int *row, int *col ) {
	double w = 0.00;
	for ( int i = x; i <= dest; ++i )
		for ( int j = y; j <= y; ++j )
			if ( fabs(A[i][j]) > w )
				return (w = fabs(A[*row = i][*col = j])) > 0;
	return w > 0;
}

void swap_rows( int i1, int i2 ) {
	if ( i1 == i2 ) return ;
	for ( int j = src; j <= dest; ++j )
		swap(A[i1][j],A[i2][j]), swap(B[i1][j],B[i2][j]);
}

void swap_cols( int j1, int j2 ) {
	if ( j1 == j2 ) return ;
	for ( int i = SRC; i <= DEST; ++i )
		swap(A[i][j1],A[i][j2]), swap(B[i][j1],B[i][j2]);
}

int main() {
	int i,j,k,row,col;
	double cf;
	for (;1==scanf("%d",&n)&&n;) {
		for ( i = SRC; i <= DEST; deg[i++] = 0 ) ;
		for ( vec.clear(), s.clear(), s.insert(SRC), s.insert(DEST), m.clear(); n-- && 2 == scanf("%d %d",&i,&j); ) 
			if ( i != j ) 
				s.insert(i), s.insert(j), vec.push_back(make_pair(i,j));
		for ( set<int> :: iterator it = s.begin(); it != s.end(); k = m.size(), m[*it++] = k ) ;
		for ( ++yes, i = 291; i <= 299; ++i )
			if ( s.find(i) != s.end() )
				booth[m[i]] = yes;
		src = m[SRC], dest = m[DEST];
		for ( i = src; i <= dest; ++i )
			for ( j = src; j <= dest; ++j )
				A[i][j] = 0;
		for ( i = 0; i < (int)vec.size(); ++i ) {
			j = m[vec[i].first], k = m[vec[i].second];
			adj[j][deg[j]++] = k, adj[k][deg[k]++] = j;
		}
		for ( i = src; i <= dest; ++i )
			for ( int l = 0; l < deg[i]; ++l )
				if ( (j = adj[i][l]) != dest && !is_booth(j) )
					A[i][j] = 1.00/deg[j];
		for ( i = src; i <= dest; ++i )
			for ( j = src; j <= dest; ++j )
				(g[i][j]=(A[i][j]=(i==j?1:0)-A[i][j])), B[i][j] = (i==j?1:0);
		for ( k = src; k <= dest; ++k ) {
			assert( find_pivot(k,k,&row,&col) );
			swap_rows(row,k);
			//swap_cols(col,k);
			for ( i = k+1; i <= dest; ++i ) 
				for ( cf = A[i][k]/A[k][k], j = src; j <= dest; ++j )
					A[i][j] -= cf*A[k][j], B[i][j] -= cf*B[k][j];
			for ( cf = A[k][k], j = src; j <= dest; ++j )
				A[k][j] /= cf, B[k][j] /= cf;
			assert( fabs(A[k][k]-1) < tol );
		}
		for ( k = dest; k >= src; --k ) 
			for ( i = k-1; i >= src; --i ) {
				cf = A[i][k];
				for ( j = src; j <= dest; ++j )
					B[i][j] -= cf*B[k][j], A[i][j] -= cf*A[k][j];
				assert( fabs(A[i][k]) < tol );
			}

		/*
		for ( i = SRC; i <= DEST; ++i )
			for ( j = SRC; j <= DEST; ++j )
				for ( t[i][j] = 0, k = SRC; k <= DEST; ++k )
					t[i][j] += g[i][k]*B[k][j];
		for ( i = SRC; i <= DEST; ++i )
			for ( j = SRC; j <= DEST; ++j ) {
				if ( i == j ) {
					//assert( fabs(t[i][j]-1) < tol );
					assert( fabs(A[i][j]-1) < tol );
				}
				else {
					if ( fabs(t[i][j]) >= tol )
						printf("t[%d][%d] = %lf\n",i,j,t[i][j]);
					assert( fabs(A[i][j]) < tol );
					//assert( fabs(t[i][j]) < tol );
				}
			}
			*/
		printf("%.3lf\n",B[dest][src]);
	}
	return 0;
}

