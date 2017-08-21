/*
 * BTCODE_H
 * TOPIC: math. expectation, formula derivation, very neat, DP, recurrence
 * NOTES: failed to find a closed form formula, special case when z[1] = n+1
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 0x200

int n,m;
double z[N];

double g( int t ) {
	return 1-pow(2.00,-t);
}

double f( int t, int m ) {
	return pow(g(t),m);
}

int main() {
	int i,j,k,ts,t;
	double ans,dw,bf;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) { 
			/*
		for ( ans = n+1, t = 1; t <= n-1; ++t )
			ans -= t*(g(t+1)*exp((t+1)*log(2)+log(1-f(t+1,m-1)))-g(t)*exp(t*log(2)+log(1-f(t,m-1))));
		ans += n*g(n)*exp(n*log(2)+log(1-f(n,m-1)));
		printf("%.2lf\n",ans);*/
		for ( bf = 0, z[1] = n+1, i = 2; i <= m; z[i] += n*f(n,i-1), ++i )
			for ( z[i] = z[i-1], t = 1; t <= n-1; ++t )
				z[i] -= t*(f(t+1,i-1)-f(t,i-1));
		printf("%.2lf\n",z[m]);
	}
	return 0;
}

