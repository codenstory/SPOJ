/*
 * GCDEX
 * TOPIC: Euler totient, GCD, number theory
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef long long i64;
#define N 1000008

char isprime[N];
int totient[N],n;
i64 z[N];

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return -1;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int main() {
	int i,j,k,ts;
	i64 ans,d;
	for ( i = 2; i < N; totient[i] = i, ++i ) ;
	for ( isprime[2] = 1, totient[2] = 1, i = 3; i < N; isprime[i] = 1, i += 2 ) ;
	for ( i = 2; i < N; ++i )
		if ( isprime[i] )
			for ( totient[i] = i-1, j = i+i; j < N; totient[j] /= i, totient[j] *= (i-1), isprime[j] = 0, j += i ) ;
	for ( i = 1; i < N; ++i )
		for ( j = 2; i*j < N; ++j )
			z[i*j] += i*totient[j];
	for ( i = 1; i < N; z[i] += z[i-1], ++i ) ;
	for ( ;(n = getnum()) >= 1; ) {
		printf("%lld\n",z[n]);
	}
	return 0;
}

