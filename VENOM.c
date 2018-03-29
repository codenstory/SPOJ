/*
 * VENOM
 * TOPIC: binary search, quadratic equations, parabola configuration
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N (1<<16)
typedef long long i64;

int n;
i64 h,p,a;
double A,P;

i64 f( i64 t ) {
	return h-t*(t+1)/2*p+a*(t-1);
}

int main() {
	int i,j,k,ts;
	i64 good, bad, mid, t, apex;
	double dapex;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%lld %lld %lld",&h,&p,&a);
		apex = (2*a-p)/(2*p), A = a, P = p;
		dapex = (2*A-P)/(2*P);
		if ( fabs(dapex-apex) >= 1e-7 ) 
			++apex;
		if ( apex < 0 ) apex = 0;
		if ( apex < 1 ) apex = 1;
		if ( f(apex) <= 0 ) {
			puts("1");
			continue ;
		}
		for ( good = apex, bad = (1LL<<30); good+1 < bad; ) {
			mid = (good+bad)>>1;
			if ( f(mid) > 0 )
				good = mid;
			else bad = mid;
		}
		printf("%lld\n",2*good+1);
	}
	return 0;
}

