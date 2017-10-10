/*
 * BEHAPPY
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
#define M 0x20
#define N 0x80
using namespace std;
typedef long long i64;
#define oo (1LL<<60)

int m,n,a[M],b[M],q[M*N],*head,*tail,seen[M][N],yes;
i64 z[M][N];

i64 calc_z( int m, int n ) {
	if ( seen[m][n] == yes )
		return z[m][n];
	seen[m][n] = yes, z[m][n] = 0;
	if ( m == 0 ) 
		return z[m][n] = n==0?1:0;
	for ( int k = a[m]; k <= b[m] && k <= n; ++k )
		z[m][n] += calc_z(m-1,n-k);
	return z[m][n];
}

int main() {
	for ( ;2 == scanf("%d %d",&m,&n) && (m||n) && ++yes; ) {
		for ( int i = 1; i <= m; ++i )
			scanf("%d %d",&a[i],&b[i]);
		printf("%lld\n",calc_z(m,n));
	}
	return 0;
}

