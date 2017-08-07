/*
 * MKMONEY
 * TOPIC: ad hoc, percentages, simulation
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol 1e-9

long double P,I;
int n;

long double f( long double x ) {
	return ((long long)(100*x+tol))/100.00;
}

int main() {
	int i,j,k,cs = 0;
	long double ans,e;
	for ( ;3 == scanf("%Lf %Lf %d",&P,&I,&n); ) {
		if ( fabs(P) < tol && fabs(I) < tol && n == 0 ) break ;
		for ( ans = P, e = I/n/100, i = 0; i < n; ++i, ans = ans+f(ans*e) ) ;
		printf("Case %d. $%.2Lf at %.2Lf%% APR compounded %d times yields $%.2Lf\n",++cs,P,I,n,ans);
	}
	return 0;
}

