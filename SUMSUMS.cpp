/*
 * SUMSUMS
 * TOPIC: matrix power, DP, recurrences, maths, very neat, binomial theorem
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
#define tol (1e-13)
#define N (1<<17)
#define MOD (98765431LL)
typedef long long i64;
using namespace std;

int n;
i64 m,c[N],inverse_n,a,b,total,z[N],w;

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) { !y?(*a=1,*b=0,*d=x):(ext_euclid(y,x%y,b,a,d),*b-=(*a)*(x/y)); }
i64 inv( i64 x, i64 P ) {
	i64 a,b,d;
	for ( ext_euclid(x,P,&a,&b,&d); a < 0; a += P ) ;
	return a%P;
}

i64 f( i64 x ) {
	if ( x >= 0 ) return (x%MOD);
	i64 n = -x/MOD;
	for ( x = n*MOD+x; x < 0; x += MOD ) ;
	return x%MOD;
}

i64 mypow( i64 x, i64 n ) {
	i64 ax = 1LL;
	for ( ;n; n >>= 1, x=f(x*x) )
		if ( n&1 ) 
			ax=f(ax*x);
	return f(ax);
}


int main() {
	int i,j,k;
	for ( ;2 == scanf("%d %lld",&n,&m); ) {
		for ( total = 0, i = 0; i < n; scanf("%lld",&c[i++]), total += c[i-1], total %= MOD	) ;
		if ( m == 0 ) {
			for ( i = 0; i < n; printf("%lld\n",c[i++]) );
			continue ;
		}
		if ( n == 1 ) {
			puts("0");
			continue ;
		}
		inverse_n = inv(n,MOD), a = mypow(n-1LL,m), b = mypow(-1LL,m);
		assert( ((inverse_n*n)%MOD) == 1LL );
		w = f((inverse_n*(((a-b+MOD)*total)%MOD)));
		for ( i = 0; i < n; ++i ) 
			z[i]=f(w+b*c[i]);
		for ( i = 0; i < n; printf("%lld\n",z[i++]) ) ;
	}
	return 0;
}

