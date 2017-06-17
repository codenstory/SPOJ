/*
 * BALLSUM
 * TOPIC: probability
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
typedef long long i64;

i64 gcd( i64 x, i64 y ) { return !y?x:gcd(y,x%y); }

int main() {
	i64 n,k;
	while ( 2 == scanf("%lld %lld",&n,&k) && (n+1||k+1) ) {
		i64 m = (k-1)>>1, a = k*m-(1+m)*m, b = n*(n-1)/2, d = gcd(a,b);
		a /= d, b /= d;
		if ( a == 0 ) {
			puts("0");
			continue ;
		}
		if ( b == a ) {
			puts("1");
			continue ;
		}
		printf("%lld/%lld\n",a,b);
	}
	return 0;
}

