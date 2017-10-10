/*
 * NGIRL
 * TOPIC: DP, binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (100100)
typedef long long i64;

i64 n,m,*p;
int len;
unsigned char isprime[N];

int bs( i64 t ) {
	int low, high, mid;
	if ( p[len-1] <= t ) 
		return len;
	if ( t < p[0] )
		return 0;
	for ( low = 0, high = len-1; low+1 < high; p[mid=(low+high)/2] > t ? (high=mid):(low=mid) ) ;
	return high;
}

int main() {
	i64 i,j,k,ts;
	for ( i = 3; i < N; isprime[i] = 1, i += 2 ) ;
	for ( isprime[2] = 1, i = 3; i < N; i += 2 )
		if ( isprime[i] )
			for ( j=i*i; j < N; isprime[j] = 0, j += i ) ;
	for ( i = 0; i < N; ++i )
		if ( isprime[i] ) ++len;
	p = (i64 *)malloc(len*sizeof *p);
	for ( k = 0, i = 0; i < N; ++i )
		if ( isprime[i] )
			p[k++] = i;
	for ( i = 0; i < len; ++i )
		p[i] *= p[i];
	for ( scanf("%lld",&ts); ts-- && 2 == scanf("%lld %lld",&n,&m); ) {
		k = bs(n), j = bs(m);
		printf("%lld %lld\n",k,k-j>=0?k-j:0);
	}
	return 0;
}

