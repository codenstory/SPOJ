/*
 * MAIN12A
 * TOPIC: precalc, simulation
 * status: Accepted
 */
#include <stdio.h>
#define N (1<<21)
int z[N];
int main() {
	int i,j,k,t,ts,cs = 0,n;
	for ( z[1] = 1, t = 2, j = 2, i = 1; j < N; ++i, ++t ) 
		for ( k = z[i]; k-- && j < N; z[j++] = t ) ;
	for ( scanf("%d",&ts); ts-- && 1==scanf("%d",&n); printf("Case #%d: %d\n",++cs,z[n]) ) ; 
	return 0;
}

