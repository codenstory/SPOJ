/*
 * IMMERSED
 * TOPIC: binary search, logarithms
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol 1e-13
typedef unsigned long long u64;

int n;
u64 P;

int main() {
	int i,j,k,ts,c;
	long double low, high, mid, dp;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%llu %d",&P,&c), dp = log(P);
		for ( low = 1, high = (1LL<<30); fabs(high-low) >= tol; ) {
			mid = (low+high)/2;
			if ( log(mid)*(c*mid+1) < dp )
				low = mid;
			else high = mid;
		}
		printf("%Lf\n",(low+high)/2);
	}
	return 0;
}

