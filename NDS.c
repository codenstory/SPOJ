/*
 * NDS
 * TOPIC: LIS, binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)

int n,x[N],len,a[N];

int main() {
	int i,j,k,ts,low,high,mid;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d",&n), i = 0; i < n; scanf("%d",&x[i++]) ) ;
		scanf("%d",&k);
		for ( len = 0, i = 0; i < n; ++i ) {
			if ( len == 0 || x[a[len]] < x[i] ) {
				a[++len] = i;
				continue ;
			}
			assert( x[a[len]] >= x[i] );
			if ( x[i] <= x[a[1]] ) {
				a[1] = i;
				continue ;
			}
			assert( x[a[1]] < x[i] );
			for ( low = 1, high = len; low+1 < high; x[a[mid=(low+high)/2]]<x[i]?(low=mid):(high=mid) ) ;
			a[high] = i;
		}
		if ( len < k ) 
			puts("-1");
		else printf("%d\n",x[a[k]]);
	}
	return 0;
}

