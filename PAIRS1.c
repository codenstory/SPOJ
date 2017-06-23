/*
 * PAIRS1 Count the Pairs
 * TOPIC: binary search, invariants, sorting
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
typedef long long i64;

int n;
i64 x[N],m;

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( *x == *y )
		return 0;
	return *x<*y?-1:1;
}

int main() {
	int i,j,k,low,high,mid,
		left,right;
	i64 t,ans;
	for ( ;2 == scanf("%d %lld",&n,&m); printf("%lld\n",ans) ) {
		for ( i = 0; i < n; scanf("%lld",&x[i++]) ) ;
		qsort(x,n,sizeof *x,cmp);
		for ( i = 0; i < n-1; ++i )
			assert( x[i] <= x[i+1] ) ;
		for ( ans = 0, i = 0; i < n-1; ++i ) {
			t = m+x[i];
			if ( x[n-1] < t ) break ;
			if ( x[n-1] == t ) right = n-1;
			else {
				assert( x[high = n-1] > t );
				if ( x[low = i+1] > t ) continue ;
				assert( x[low] <= t );
				for ( ;low+1 < high; x[mid=(low+high)/2]<=t?(low=mid):(high=mid) ) ;
				assert( x[low] <= t );
				assert( x[high] > t );
				if ( x[low] == t ) right = low;
				else continue ;
			}
			if ( x[low = i+1] == t ) left = low;
			else if ( x[low] > t ) continue ;
			else {
				assert( x[low] < t ) ;
				if ( x[high = n-1] < t ) continue ;
				assert( x[high] >= t );
				for ( ;low+1<high; x[mid=(low+high)/2]>=t?(high=mid):(low=mid) ) ;
				assert( x[high] >= t );
				assert( x[low] < t  );
				if ( x[high] == t ) left = high;
				else continue ;
			}
			ans += right-left+1;
		}
	}
	return 0;
}

