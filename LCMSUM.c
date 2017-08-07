/*
 * LCMSUM
 * TOPIC: number theory, DP, very neat, order of computation
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1000100)
typedef long long i64;

i64 z[N],f[N];
int n;

int main() {
	i64 i,j,k,ts;
	for ( i = 1; i < N; ++i )
		f[i] = (i+1)*i/2;
	for ( i = 1; i < N; ++i )
		for ( j = 2; i*j < N; ++j )
			f[i*j] -= j*f[i];
	for ( i = 1; i < N; ++i )
		for ( j = 1; i*j < N; z[i*j] += f[j], ++j ) ;
	for ( scanf("%lld",&ts); ts-- && 1 == scanf("%d",&n); printf("%lld\n",n*z[n]) ) ;
	return 0;
}

