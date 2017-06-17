/*
 * ADATEAMS
 * TOPIC: combinatorics
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (1000000007LL)
#define N (1<<20)
typedef long long i64;

i64 pw( i64 x, i64 n ) {
	i64 ax = 1LL;
	for (;n; n>>=1,x*=x,x%=MOD)
		if(n&1) ax *= x,ax%=MOD;
	return ax;
}

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) {
	if ( !y ) *a = 1, *b = 0, *d = x;
	else 
		ext_euclid(y,x%y,b,a,d), *b -= *a*(x/y);
}

i64 inv( i64 x, i64 P ) {
	i64 a,b,d;
	for ( ext_euclid(x,P,&a,&b,&d); a < 0; a += P ) ;
	return a%P;
}

i64 f[N],q[N],n,a,b,d;

i64 C( int n, int k ) {
	return (((f[n]*q[k])%MOD)*q[n-k])%MOD;
}

int main() {
	int i,j,k;
	for ( f[0] = f[1] = 1, q[0]=q[1]=1, i = 2; i < N; ++i )
		f[i] = f[i-1]*i, f[i] %= MOD, q[i] = inv(f[i],MOD);
	for ( ;4 == scanf("%lld %lld %lld %lld",&n,&a,&b,&d); printf("%lld\n",(C(n,a)*pw(C(b,d),a))%MOD) ) ;
	return 0;
}

