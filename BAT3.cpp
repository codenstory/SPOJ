/*
 * BAT3: BATMAN3
 * TOPIC: dp
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
#define N 0x400
typedef long long i64;
using namespace std;

int n,c[N],a[N],ans;
i64 h[N];

int main() {
	int i,j,k,ts,idx;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&idx), ++idx;
		for ( i = 1; i <= n; scanf("%lld",&h[i++]) ) ;
		for ( c[n] = 1, i = n-1; i >= 1; --i )
			for ( c[i] = 1, j = i+1; j <= n; ++j )
				if ( h[i] > h[j] ) 
					c[i] = max(c[i],1+c[j]);
		for ( a[1] = 1, i = 2; i <= n; ++i )
			for ( a[i] = 1, j = i-1; j >= 1; --j )
				if ( h[j] > h[i] )
					a[i] = max(a[i],1+a[j]);
		for ( ans = 0, i = 1; i <= n; ++i )
			ans = max(ans,max(c[i],a[i]));
		for ( j = idx+1; j <= n; ans = max(ans,a[idx]+c[j++]) ) ;
		printf("%d\n",ans);
	}
	return 0;
}

