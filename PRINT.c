/*
 * PRINT
 * TOPIC: segmented sieve
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX (46656)
#define RNG (1000032)
#define LEN (4830)
#define LMT (216)
#define BIT(k) (1ULL<<(k))
typedef long long i64;

void getnum( int *res ) {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	*res = sign*n;
}

int n;
unsigned int base[MAX>>6], segment[RNG>>6], primes[LEN];
#define SET(a,k) (a[(k)>>6] |= BIT(((k)>>1)&31UL))
#define CLR(a,k) (a[(k)>>6] &= ~BIT(((k)>>1)&31UL))
#define TST(a,k) (a[(k)>>6] & BIT(((k)>>1)&31UL))

void sieve() {
	int i,j,k;
	for ( i = 3; i < LMT; i += 2 )
		if ( !TST(base,i) )
			for ( j = i*i, k = (i<<1); j < MAX; SET(base,j), j += k ) ;
	for ( n = 0, i = 3; i < MAX; i += 2 )
		if ( !TST(base,i) )
			primes[n++] = i;
}

void segmented_sieve( int L, int R ) {
	int i,j,k;
	if ( L <= 2 && 2 <= R )
		puts("2");
	if ( R < 3 ) return ;
	if ( L < 3 ) L = 3;
	if ( !(L&1) ) ++L;
	if ( !(R&1) ) --R;
	if ( L > R ) return ;
	assert( L <= R );
	memset(segment,0,sizeof segment);
	for ( i = 0; primes[i]*primes[i] <= R; ++i ) {
		if ( !(L%primes[i]) ) j = L;
		else j = primes[i]*(L/primes[i]+1);
		//j = primes[i]*((L+primes[i]-1)/primes[i]);
		if ( !(j&1) ) j += primes[i];
		for ( k = primes[i]<<1; j <= R; j += k )
			if ( j != primes[i] )
				SET(segment,j-L);
	}
	for ( i = 0; i <= R-L; i += 2 )
		if ( !TST(segment,i) )
			printf("%d\n",L+i);
}

int main() {
	int ts,L,R;
	for ( sieve(), getnum(&ts); ts--; getnum(&L), getnum(&R), segmented_sieve(L,R) ) ;
	return 0;
}

