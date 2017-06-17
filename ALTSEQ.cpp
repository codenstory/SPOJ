/*
 * ALTSEQ: Alternating Sequence
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
#define   tol  (1e-13)
#define   N    (5100)
typedef long long i64;
using namespace std;

int n,z[N],ans;
i64 a[N];

int main() {
	int i,j,k;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; scanf("%lld",a+i), ++i ) ;
		for ( z[0] = 0, z[1] = 1, i = 2; i <= n; ++i ) 
			for ( z[i] = 1, j = i-1; j >= 1; --j )
				if ( a[j]*a[i] < 0 && labs(a[j]) < labs(a[i]) ) 
					z[i] = max(z[i],1+z[j]);
		for ( ans = 0, i = 1; i <= n; ++i )
			ans = max(ans,z[i]);
		printf("%d\n",ans);
	}
	return 0;
}

