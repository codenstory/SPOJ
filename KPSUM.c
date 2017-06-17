/*
 * KPSUM: The Sum
 * TOPIC: dp, decimal range decomposition, combinatorics, sums
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define D 10
#define L 17
#define LIM (10)
typedef long long i64;

i64 S[D],A[D],deg[L],
	dp[LIM];

i64 f( i64 *d, int len ) {
	i64 i,j=0,sum = 0;
	for ( i = len-1; i >= 0; --i,++j )
		if ( !(j&1) )
			sum += d[i];
		else sum -= d[i];
	/*
	for ( i = len-1; i >= 0; --i )
		printf("%lld",d[i]);
	printf(", sum = %lld\n",sum);*/
	return sum;
}

int main() {
	int i,j,k,n,m,is_minus;
	i64 d[L],nn,ans,sum,tmp;
	/* dp to prevent problems with boundary cases -- very small "n"
	 */
	for ( dp[1]=1,is_minus=1,nn=2; nn<LIM; dp[nn]=dp[nn-1]+(is_minus?-f(d,m):f(d,m)),is_minus^=(m&1),++nn ) 
		for ( n = nn, m = 0; n; d[m++] = (n%10), n /= 10 ) ;
	for ( i = 1; i < D; ++i ) S[i] = S[i-1]+i;
	for ( i = 1; i < D; ++i ) A[i] = A[i-1]+((i&1)?i:-i);
	for ( deg[0] = 1LL, i = 1; i < L; ++i ) deg[i] = deg[i-1]*10LL;
	for ( ;1 == scanf("%lld",&nn) && nn; ) {
		if ( nn < LIM ) {
			printf("%lld\n",dp[nn]);
			continue ;
		}
		for ( n = 0; nn; d[n++] = (nn%10), nn /= 10 ) ;
		for ( ans = A[9], i = 2; i < n; ++i ) {
			k = (i>>1);
			if ( i&1 ) {
				assert( 2*k+1 == i );
				ans += 9*A[9]*deg[2*k-1];
			}
			else ans -= S[9]*deg[2*k-2];
		}
		if ( n&1 ) {
			for ( sum = 0, i = 1; i <= n-2; ++i )
				sum += d[i]*deg[i-1];
			sum += (d[n-1]-1)*deg[n-2];
			sum *= A[9];
			for ( j = 0, i = 0; i <= d[0]; ++i, ++j ) 
				k = d[0], d[0] = i, sum += !(j&1)?-f(d,n):f(d,n), d[0] = k;
			ans += sum;
		}
		else {
			for ( sum = 0, i = 0; i <= n-2; ++i )
				sum += deg[i]*d[i]*f(d+i+1,n-i-1);
			ans -= sum;
			for ( i = 0; i <= n-1; ++i ) {
				if ( (n-i)&1 ) {
					if ( i<n-1 ) {
						assert( i );
						ans += S[9]*d[i]*deg[i-1];
					}
					else ans += S[9]*(d[n-1]-1)*deg[n-2];
				}
				if ( d[i] == 0 ) continue ;
				ans += ((n-i)&1)?(-S[d[i]-1]*deg[i]):(S[d[i]-1]*deg[i]);
			}
			ans -= f(d,n);
		}
		printf("%lld\n",ans);
	}
	return 0;
}

