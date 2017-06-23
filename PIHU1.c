/*
 * PIHU1
 * TOPIC: binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x400
typedef long long i64;

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( *x == *y ) return 0;
	return *x<*y?-1:1;
}

int n;
i64 c[N],m,*x;

int main() {
	int i,j,k,ts,ok,low,high,mid;
	i64 ans,t;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n);
		for ( i = 0; i < n; ++i ) scanf("%lld",&c[i]);
		qsort(c,n,sizeof *c,cmp);
		for ( i = 0; i < n-1; ++i )
			assert( c[i] <= c[i+1] ) ;
		for ( x = c, ans = 0, ok = 0, scanf("%lld",&m), i = 0; i < n && !ok; ++i ) {
			for ( j = i+1; j+1 < n && !ok; ++j ) {
				t = m-x[i]-x[j];
				if ( x[n-1] < t ) continue ;
				if ( x[n-1] == t ) {
					ok = 1;
					break ;
				}
				if ( x[j+1] > t ) continue ;
				assert( x[j+1] <= t ) ;
				assert( x[n-1] > t );
				for ( low = j+1, high = n-1; low+1<high; c[mid=(low+high)/2]>t?(high=mid):(low=mid) ) ;
				ok = (c[low] == t);
			}
		}
		puts(ok?"YES":"NO");
	}	
	return 0;
}

