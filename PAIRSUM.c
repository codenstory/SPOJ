/*
 * PAIRSUM
 * TOPIC: maths, simple, trivial, formula
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
typedef long long i64;

i64 getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int n,m;
i64 pref[N],q[N],c[N];

int main() {
	int i,j,k,qr;
	i64 s;
	for ( n = getnum(), i = 1; i <= n; c[i] = getnum(), pref[i] = pref[i-1]+c[i], q[i]=q[i-1]+c[i]*c[i], ++i ) ;
	for ( qr=getnum(); qr--; i=getnum()+1, j=getnum()+1, s=pref[j]-pref[i-1], printf("%lld\n",(s*s+(q[j]-q[i-1]))>>1) ) ;
	return 0;
}

