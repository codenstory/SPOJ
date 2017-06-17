/*
 * AMBLE: Alicia's Afternoon Amble
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
#include <cfloat>
#define oo (DBL_MAX-0x400)
#define tol (1e-13)
#define N 0x400
#define S(x) ((x)*(x))
typedef long long i64;
using namespace std;

pair<i64,i64> v[N];
int n;
double z[N][N],d[N][N],ans;

int main() {
	int i,j,k;
	for (;1==scanf("%d",&n);) {
		for ( i = 0; i < n; ++i ) 
			scanf("%lld %lld",&v[i].first,&v[i].second);
		for ( sort(v,v+n), i = 0; i < n; ++i )
			for ( j = i+1; j < n; ++j )
				d[i][j]=d[j][i]=sqrt(S(v[i].first-v[j].first)+S(v[i].second-v[j].second));
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j <= n; z[i][j++] = +oo ) ; 
		for ( z[0][0] = 0, i = 0; i < n-1; ++i )
			for ( j = 0; j < n-1; ++j )
				if ( z[i][j] < +oo ) {
					k = max(i,j)+1;
					if ( z[i][k] > z[i][j]+d[j][k] )
						 z[i][k] = z[i][j]+d[j][k];
					if ( z[k][j] > z[i][j]+d[i][k] )
						 z[k][j] = z[i][j]+d[i][k];
				}
		for ( ans = +oo, i = 0; i <= n-2; ++i ) {
			if ( z[i][n-1] < +oo )
				ans = min(ans,z[i][n-1]+d[i][n-1]);
			if ( z[n-1][i] < +oo )
				ans = min(ans,z[n-1][i]+d[n-1][i]);
		}
		printf("%.2lf\n",ans);
	}
	return 0;
}

