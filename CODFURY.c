/*
 * CODFURY
 * TOPIC: sliding window
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (50100)
typedef long long i64;

int n;
i64 m,c[N],pref[N];

int main() {
	int i,j,k,ts,maxn;
	i64 s,bestsum;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %lld",&n,&m); ) {
		for ( i = 1; i <= n; scanf("%lld",&c[i++]), pref[i-1] = pref[i-2]+c[i-1] ) ;
		for ( maxn = 0, i = 1; i <= n && c[i] > m; ++i ) ;
		if ( i == n+1 ) {
			puts("0 0");
			continue ;
		}
		for ( j = i+1, s = c[i]; i <= n; ) {
			assert( s == pref[j-1]-pref[i-1] );
			assert( s <= m );
			if ( maxn < (j-i) || maxn == (j-i) && bestsum > s )
				maxn = j-i, bestsum = s;
			for ( ;j <= n && s+c[j] <= m; s += c[j++] ) ;
			if ( j == n+1 ) {
				if ( maxn < (j-i) || maxn == (j-i) && bestsum > s )
					maxn = j-i, bestsum = s;
				break ;
			}
			if ( maxn < (j-i) || maxn == (j-i) && bestsum > s )
				maxn = j-i, bestsum = s;
			assert( s <= m && s+c[j] > m );
			for ( s += c[j]; i <= j && s > m; s -= c[i++] ) ;
			if ( i > j ) {
				assert( s == 0 );
				for ( ;i <= n && c[i] > m; ++i ) ;
				if ( i == n+1 ) break ;
				s = c[i], j = i+1;
			}
			else ++j;
			assert( s == pref[j-1]-pref[i-1] );
		}
		printf("%lld %d\n",bestsum,maxn);
	}
	return 0;
}

