/*
 * LCPC12F
 * TOPIC: binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
#define MAXW (0x800)
typedef long long i64;

i64 cnt[MAXW],n,c[N],T;

i64 f( i64 n ) { return (n*(n-1))>>1; }

int main() {
	i64 i,j,k,ts,cs = 0,left,right,low,high,mid;
	i64 ans,bf;
	for ( scanf("%lld",&ts); ts-- && 1 == scanf("%lld",&T); ) {
		for ( memset(cnt,0,sizeof cnt), scanf("%lld",&n), i = 0; i < n; scanf("%lld",&c[i++]), ++cnt[c[i-1]] ) ;
		for ( k = 0, i = 0; i < MAXW; ++i )
			for ( ;cnt[i]--; c[k++] = i ) ;
		assert( k == n );
		if ( T == 0 ) {
			for ( left = 0; left < n && c[left] == 0; ++left ) ;
			ans = f(left);
			printf("%lld. %lld\n",++cs,ans);
			continue ;
		}
		for ( ans = 0, right = n-1, i = 0; i < n; i = j ) {
			for ( j = i+1; j < n && c[j] == c[i]; ++j ) ;
			if ( c[n-1] < T-c[i] || c[0] > T-c[i] ) continue ;
			if ( c[n-1] == T-c[i] ) right = n-1;
			else {
				assert( c[n-1] > T-c[i] ) ;
				for ( low = 0, high = n-1; low+1<high; c[mid=(low+high)/2]<=T-c[i]?(low=mid):(high=mid) );
				if ( c[low] == T-c[i] ) right = low;
				else right = -1;
			}
			if ( right == -1 ) continue ;
			if ( c[0] == T-c[i] ) left = 0;
			else {
				assert( c[0] < T-c[i] ) ;
				for ( low = 0, high = n-1; low+1 < high; c[mid=(low+high)/2]<T-c[i]?(low=mid):(high=mid) ) ;
				left = high;
			}
			if ( left == i ) {
				assert( right == j-1 );
				ans += f(j-i);
				continue ;
			}
			if ( j-1 < left ) 
				ans += (j-i)*(right-left+1);
		}
		printf("%lld. %lld\n",++cs,ans);
	}
	return 0;
}

