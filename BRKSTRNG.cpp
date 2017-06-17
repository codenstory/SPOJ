/*
 * BRKSTRNG: Breaking String
 * TOPIC: greedy, priority queue
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
#define oo (1LL<<60)
using namespace std;
#define N 0x400
typedef long long i64;

int n,L,c[N];
i64 a[N],best,ans;

int main() {
	int i,j,k,t,idx;
	for ( ;2 == scanf("%d %d",&L,&n); ) {
		for ( i = 1; i <= n; scanf("%d",&c[i++]) ) ;
		c[0] = 0, c[n+1] = L;
		for ( i = 1; i <= n+1; ++i )
			a[i] = c[i]-c[i-1];
		for ( ans = 0, k = n+1; k >= 2; --k ) {
			for ( best = +oo, i = 1; i <= k-1; ++i )
				if ( a[i]+a[i+1] < best ) 
					best = a[i]+a[i+1], idx = i;
			for ( ans += best, a[idx] = a[idx]+a[idx+1], i = idx+1; i <= k-1; ++i )
				a[i] = a[i+1];
		}
		printf("%lld\n",ans);
	}
	return 0;
}

