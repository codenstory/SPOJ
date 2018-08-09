/*
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#define tol (1e-13)
using namespace std;
typedef long long i64;

i64 gcd( i64 x, i64 y ) { return !y?x:gcd(y,x%y); }
i64 P,Q;

long double f( i64 x ) {
	long double total= x;
	for ( int i= 1; i <= 40; ++i )
		total/= 2;
	return total;
}

int main() {
	int i,j,k,cs= 0,ts;
	for ( cin >> ts; ts--; ) {
		scanf("%lld/%lld",&P,&Q);
		i64 d= gcd(P,Q);
		P/= d, Q/= d;
		printf("Case #%d:",++cs);
		i64 low, high, mid;
	    long double frac= P/(Q+0.00);
		if ( f(high= (1LL<<40)) < frac ) {
			puts(" impossible");
			continue ;
		}
		for ( low= 0; low+1 < high; Q*f(mid= (low+high)/2)>=P?(high= mid):(low= mid) ) ;
		if ( fabs(f(high)*Q-P) > tol ) {
			puts(" impossible");
			continue ;
		}
		printf(" %lld\n",40-(i64)(log(high)/log(2)+tol));
	}
	return 0;
}

