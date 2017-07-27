/*
 * BTCK
 * TOPIC: lex. order, permutations, permutations in lex. order, interview level
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 10
typedef long long i64;

int c[N],n = N;
i64 a[N],K;

i64 scalar_product() {
	i64 s = 0;
	int i;
	for ( i = 0; i < n; ++i )
		s += c[i]*a[i];
	return s;
}

int next_permutation() {
	int i,j,k,t;
	for ( i = n-1; i >= 1 && c[i] < c[i-1]; --i ) ;
	if ( i == 0 ) return 0;
	for ( j = n-1; c[j] < c[i-1]; --j ) ;
	k = c[i-1], c[i-1] = c[j], c[j] = k;
	for ( j = i, k = n-1; j < k; ++j, --k )
		t = c[j], c[j] = c[k], c[k] = t;
	for ( j = i; j < n-1; ++j )
		assert( c[j] < c[j+1] );
	/*
	for ( i = 0; i < n; ++i )
		printf("%d ",c[i]);
	puts("");*/
	return 1;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		for ( i = 0; i < n; scanf("%lld",a+i), c[i] = i, ++i ) ;
		for ( scanf("%lld",&K); scalar_product() > K && (i = next_permutation()); ) ;
		if ( !i ) {
			puts("-1");
			continue ;
		}
		for ( i = 0; i < n-1; ++i )
			printf("%d ",c[i]);
		printf("%d\n",c[i]);
	}
	return 0;
}

