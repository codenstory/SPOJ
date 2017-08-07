/*
 * TOPIC: Binary Search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol 1e-7
#define N (1<<20)

double p[N+1];

int main() {
	int i,j,k,low,high,mid;
	double c;
	for ( p[1] = 0.5, i = 2; i < N; ++i )
		p[i] = p[i-1]+1.00/(i+1);
	for (;1==scanf("%lf",&c) && fabs(c)>=tol; ) {
		if ( p[1] >= c ) { puts("1 card(s)"); continue; }
		else {
			for ( low = 1, high = N-1; low+1 < high; ) {
				mid = (low+high)/2;
				if ( p[mid] >= c )
					high = mid;
				else low = mid;
			}
		}
		printf("%d card(s)\n",high);
	}
	return 0;
}

