/*
 * BLCONE
 * TOPIC: binary search, math
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define pi (2*acos(0.00))
#define tol 1e-7

double R,H,a,t;

double volume( double h ) {
	double r = R*h/H;
	return pi*r*r*h/3;
}

double area( double h ) {
	double r = R*h/H;
	return pi*r*sqrt(h*h+r*r);
}

double painted_area( double h ) { 
	return area(h)+volume(h); 
}

int main() {
	int i,j,k,ts;
	double low,high,mid;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%lf %lf",&R,&H);
		t = sqrt((R/H)*(R/H)+1), a = 2*area(H);
		if ( painted_area(H) <= a ) {
			printf("%lf\n",H);
			continue ;
		}
		assert( painted_area(H) > a );
		assert( painted_area(0) <= a );
		for ( low = 0, high = H; fabs(high-low) >= tol; painted_area(mid=(low+high)/2) <= a ? (low = mid):(high = mid) ) ;
		printf("%lf\n",low);
	}
	return 0;
}

