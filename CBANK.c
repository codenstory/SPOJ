/*
 * CBANK
 * TOPIC: maths, combinatorics, counting
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000000LL+7LL)
typedef long long u64;
#define THREE (333333336LL)

u64 C( u64 n ) {
	return ((n&1)?(((n-1)>>1)*n):((n/2)*(n-1)))%MOD;
}

u64 S( u64 n ) {
	u64 x = C(n+1), y = (((n*THREE)%MOD)*(((n*n)%MOD+MOD-1)%MOD))%MOD;
	return (x+y)%MOD;
	/*return ((n+1)*n/2+2*((n+1)*(n)*(n-1)/6))%MOD;*/
}

u64 f( u64 n ) {
	u64 m = (n/2), res = (((3+4*m)%MOD)*C(m+1))%MOD;
	res += (((m+1)*(m+1))%MOD), res %= MOD;
	res -= ((4*S(m))%MOD);
	for ( ;res < 0; res += MOD ) ;
	return res%MOD;
}

u64 h( u64 n ) {
	u64 m = (n-1)/2,
		x = ((m+1)*(m+1))%MOD,
		y = MOD-C(m+1),
		z = ((x+y)%MOD);
	return (z+z)%MOD;
}

int main() {
	int i,j,k,ts;
	u64 ans,n;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%lld",&n); ) {
		printf("%lld\n",(((f(n)+f(n-1))%MOD)+h(n))%MOD);
	}
	return 0;
}

