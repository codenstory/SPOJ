/*
 * TOPIC: exponential searc, then binary search
 * status: Accepted
 */
#include <bits/stdc++.h>
#define tol (1e-13)
#define pi (2*acos(0.00))
using namespace std;

bool check( long long r, long long t, long long k ) {
	return 2*k*k + (2*r-1)*k <= t;
}

int main() {
	int i,j,k,cs= 0,ts;
	long long r,t;
	for ( cin >> ts; ts--; ) {
		cin >> r >> t;
		long long left, right;
		for ( left= 0, right= 1; check(r,t,right); left= right, right <<= 1 ) ;
		while ( left+1 < right ) {
			long long mid= (left+right)>>1;
			if ( check(r,t,mid) )
				left= mid;
			else
				right= mid;
		}
		printf("Case #%d: %lld\n",++cs,left);
	}
	return 0;
}

