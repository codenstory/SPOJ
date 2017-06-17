/*
 * DCEPC501: Save Thy Toys
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
#include <map>
#define tol (1e-13)
#define N (1<<18)
typedef long long i64;
#define oo (1LL<<60)
using namespace std;

int n;
i64 c[N],pref[N],ans,m[N][4];

i64 calc_z( int k, int len ) {
	if ( k > n ) return 0L;
	assert( k <= n );
	if ( m[k][len] > -oo )
		return m[k][len];
	if ( k+len-1 >= n ) 
		return m[k][len] = pref[n]-pref[k-1];
	i64 w = pref[k+len-1]-pref[k-1];
	for ( int t = 1; t <= 3; ++t ) 
		m[k][len] = max(m[k][len],w+calc_z(k+len+len,t));
	return m[k][len];
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j < 4; ++j )
				m[i][j] = -oo;
		for ( i = 1; i <= n; ++i )
			scanf("%lld",&c[i]), pref[i] = pref[i-1]+c[i];
		for ( ans = -oo, k = 1; k <= 3; ++k )
			ans = max(ans,calc_z(1,k));
		printf("%lld\n",ans);
	}
	return 0;
}

