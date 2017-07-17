/*
 * HACKRNDM
 * TOPIC: binary search
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol (1e-13)
#define N (1<<18)
typedef long long i64;
using namespace std;

int n;
i64 c[N],m;

int main() {
	int i,j,k;
	i64 ans;
	for ( ;2 == scanf("%d %lld",&n,&m); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%lld",c+i);
		sort(c+1,c+n+1);
		for ( ans = 0, i = 1; i <= n-1; ++i ) {
			int low,high,mid,left,right;
			bool ok = true ;
			i64 target = c[i]+m;
			if ( target < c[i+1] ) continue ;
			if ( target > c[n] ) break ;
			if ( target == c[i+1] )
				left = i+1;
			else {
				assert( target > c[i+1] );
				assert( target <= c[n] );
				for ( low = i+1, high = n; low+1 < high; target>c[mid=(low+high)/2]?(low=mid):(high=mid) ) ;
				if ( target == c[high] ) left = high;
				else ok = false ;
			}
			if ( !ok ) continue ;
			if ( target == c[n] ) right = n;
			else {
				assert( target < c[n] );
				assert( target >= c[i+1] );
				for ( low = i+1, high = n; low+1<high; target < c[mid=(low+high)/2]?(high=mid):(low=mid) ) ;
				assert( target == c[low] );
				right = low;
			}
			ans += (right-left+1);
		}
		printf("%lld\n",ans);
	}
	return 0;
}

