/*
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define MOD (100003)
#define N (0x200)
#define oo (1LL<<61)
typedef long long i64;
using namespace std;

int n;
i64 z[N][N],c[N][N];

i64 calc_z( int m, int k ) {
	if ( z[m][k] < +oo )
		return z[m][k];
	if ( k >= m || m == 1 )
		return z[m][k]= 0;
	assert( m >= 2 );
	if ( k == 1 )
		return z[m][k]= 1;
	assert( m >= 2 && k >= 2 );
	z[m][k]= 0;

	for ( int i= 1; i <= k-1; ++i )
		z[m][k]+= (c[m-k-1][k-i-1]*calc_z(k,i))%MOD, z[m][k]%= MOD;
	return z[m][k];
}

int main() {
	int i,j,k,cs= 0,ts;
	i64 ans;
	for ( i= 0; i < N; c[i++][0]= 1LL ) ;
	for ( i= 1; i < N; ++i )
		for ( j= 1; j <= i; ++j )
			c[i][j]= (c[i-1][j]+c[i-1][j-1])%MOD;
	for ( i= 0; i < N; ++i )
		for ( j= 0; j < N; ++j )
			z[i][j]= +oo;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n);
		for ( ans= 0, k= 1; k <= n; ++k )
			ans+= calc_z(n,k), ans%= MOD;
		printf("Case #%d: %lld\n",++cs,ans);
	}
	return 0;
}

