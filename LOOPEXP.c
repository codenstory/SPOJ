/*
 * LOOPEXP
 * TOPIC: expectation, dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)

int n;
double z[N],p[N];

int main() {
	int i,j,k,cs = 0,ts;
	for ( n = 1; n < N; ++n ) 
		p[n]=p[n-1]+(z[n]=1+p[n-1]/n);
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); printf("%lf\n",z[n]) );
	return 0;
}

