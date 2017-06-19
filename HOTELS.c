/*
 * HOTELS
 * TOPIC: two-pointer, sliding windows
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
typedef long long i64;
#define N (300000+9)

int n;
i64 c[N],m,ans,s,pref[N];

int max( int x, int y ) { return x < y ? y:x; }

int main() {
	int i,j,k;
	i64 low,high,mid;
	for ( ;2 == scanf("%d %lld",&n,&m); ) {
		for ( i = 1; i <= n; scanf("%lld",&c[i++]), pref[i-1] = pref[i-2]+c[i-1] ) ;
		for ( i = 1; i <= n && c[i] > m; ++i ) ;
		assert( i <= n );
		for ( ans = 0, j = i+1, s = c[i]; i <= n; ) {
			if ( ans < s ) ans = s;
			for ( ;j <= n && s+c[j] <= m; s += c[j++] ) ;
			if ( s > ans ) ans = s;
			if ( ans == m ) {
				/*printf("s = %lld, i = %d, j = %d, pref = %lld\n",s,i,j-1,pref[j-1]-pref[i-1]);
				break ;*/
			}
			if ( j == n+1 ) break ;
			assert( s <= m && s+c[j] > m );
			s += c[j];
			assert( s == pref[j]-pref[i-1] );
			assert( s > m );
			for ( ; i <= j && s > m; ) {
				s -= c[i++];
				assert( s == pref[j]-pref[i-1] );
			}
			if ( i > j ) { 
				assert( i == j+1 );
				assert( s == 0 );
				for ( ;i <= n && c[i] > m; ++i ) ;
				if ( i == n+1 ) break ;
				s = c[i], j = i+1;
			}
			else ++j;
		}
		printf("%lld\n",ans);
		/*
		for ( low = 0, high = m+1; low+1 < high; ) {
			mid = (low+high)/2;
		}*/
	}
	return 0;
}

