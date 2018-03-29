/*
 * BALLOT
 * TOPIC: binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)
typedef long long i64;

int n,m;
i64 a[N];

i64 boxes_required( i64 q ) {
	int i;
	i64 ans = 0;
	for ( i = 0; i < n; ++i )
		ans += (a[i]/q)+((a[i]%q)?1:0);
	return ans;
}

int main() {
	int i,j,k,good,bad,mid;
	for ( ;2 == scanf("%d %d",&n,&m) && (n+1||m+1); ) {
		for ( i = 0; i < n; scanf("%lld",&a[i++]) ) ;
		if ( boxes_required(1) <= m ) {
			puts("1");
			continue ;
		}
		for ( good = N, bad = 1; good > bad+1; boxes_required(mid = (good+bad)>>1) <= m ? (good = mid) : (bad = mid) ) ;
		printf("%d\n",good);
	}
	return 0;
}

