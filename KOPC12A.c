/*
 * KOPC12A
 * TOPIC: ternary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol (1e-9)
#define N (1<<16)
#define oo (1LL<<30)

int n,h[N],c[N];

double cost( double H ) {
	double ans = 0;
	int i;
	for ( i = 0; i < n; ++i )
		ans += fabs(h[i]-H)*c[i];
	return ans;
}

int main() {
	int i,j,k,ts,ans;
	double a,b,na,nb,w;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i )
			scanf("%d",&h[i]);
		for ( i = 0; i < n; ++i )
			scanf("%d",&c[i]);
		for ( a = +oo, b = -oo, i = 0; i < n; ++i )
			if ( h[i] < a ) a = h[i];
			else if ( h[i] > b ) b = h[i];
		for ( ;fabs(b-a) >= tol; ) {
			na = (2*a+b)/3;
			nb = (2*b+a)/3;
			if ( cost(na) < cost(nb) )
				b = nb;
			else a = na;
		}
		i = (int)(a+tol), j = i-1, k = i+1;
		w = cost(ans = i);
		if ( j >= 0 && w > cost(j) ) w = cost(ans = j);
		if ( w > cost(k) ) w = cost(ans = k);
		printf("%.lf\n",w+tol);
	}
	return 0;
}

