/*
 * MKBUDGET: Making a budget
 * TOPIC: dp
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N 0x20
#define MAXW 301
#define oo (1LL<<60)
typedef long long i64;
using namespace std;

i64 hiring, firing, salary,
	z[N][MAXW];
int n,m[N];

i64 calc_z( int i, int j ) {
	int k;
	if ( z[i][j] < +oo )
		return z[i][j];
	if ( i == 0 ) {
		if ( j == 0 ) return z[i][j] = 0;
		return z[i][j];
	}
	assert( i >= 1 );
	if ( j < m[i] )
		return z[i][j] = +oo;
	for ( k = 0; k <= j; ++k ) 
		if ( calc_z(i-1,k) < +oo ) 
			z[i][j] = min(z[i][j],z[i-1][k]+hiring*(j-k)+salary*j);
	for ( k = j+1; k < MAXW; ++k )
		if ( calc_z(i-1,k) < +oo )
			z[i][j] = min(z[i][j],z[i-1][k]+firing*(k-j)+salary*j);
	return z[i][j];
}

int main() {
	int i,j,k,cs = 0;
	i64 ans;
	for ( ;1 == scanf("%d",&n) && n; ) {
		scanf("%lld %lld %lld",&hiring,&salary,&firing);
		for ( i = 1; i <= n; scanf("%d",&m[i++]) ) ;
		printf("Case %d, cost = $",++cs);
		for ( i = 0; i <= n; ++i )
			for ( j = 0; j < MAXW; ++j )
				z[i][j] = +oo;
		for ( ans = +oo, j = 0; j < MAXW; ++j )
			ans = min(ans,calc_z(n,j));
		printf("%lld\n",ans);
	}	
	return 0;
}

