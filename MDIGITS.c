/*
 * MDIGITS
 * TOPIC: DP, digit DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol (1e-11)
#define N 0x10
typedef long long i64;

i64 deg[N];

i64 h( const int dig, int len ) {
	if ( len < 0 ) return 0;
	return deg[len-1]*len;
}

i64 g( const int dig, int len ) {
	i64 ans = 0;
	if ( len < 0 ) return 0;
	if ( dig == 0 ) 
		return 9*h(dig,len-1);
	return deg[len-1]+9*h(dig,len-1);
}

i64 max( i64 x, i64 y ) { return x<y?y:x; }

i64 e( int *d, int n ) {
	i64 ans = 0,i;
	if ( n <= 0 ) return 0;
	for ( i = 0; i < n; ++i )
		ans = 10*ans+d[i];
	return ans;
}

i64 f( const int dig, int limit ) {
	int num_digs,k,d[N],i,j;
	i64 ans = 0, cnt = 0;

	if ( limit == 0 )
		return 0;
	assert( limit >= 1 );
	num_digs = (int)(log(limit)/log(10.00)+tol)+1;
	for ( k = 1; k <= num_digs-1; ++k ) 
		ans += g(dig,k);
	for ( k = limit, i = num_digs-1; i >= 0 && k; d[i--] = (k%10), k /= 10 ) ;
	assert( i == -1 && k == 0 );
	for ( i = 0, k = num_digs-1; i < num_digs; ++i, --k ) {
		ans += max(0,(i==0?d[i]-1:d[i]))*h(dig,k);
		if ( !(i == 0 && dig == 0) && d[i] > dig )
			ans += deg[k];
		if ( d[i] == dig && ++cnt )
			ans += e(d+i+1,k);
	}
	return ans+cnt;
}

int main() {
	int i,qa,qb;
	for ( deg[0] = 1LL, i = 1; i < N; deg[i] = 10LL*deg[i-1], ++i ) ;
	for ( ;2 == scanf("%d %d",&qa,&qb) && (qa||qb); printf("%lld \n",f(i,qb)-f(i,qa-1)) ) {
		if ( qa > qb ) i = qa, qa = qb, qb = i;
		for ( i = 0; i <= 8; printf("%lld ",f(i,qb)-f(i,qa-1)), ++i ) ;
	}
	return 0;
}

