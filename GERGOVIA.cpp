/*
 * TOPIC: greedy
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

i64 c[N];
int n;

int main() {
	int i,j,k;
	for ( ;1 == scanf("%d",&n) && n; ) {
		i64 ans = 0, a;
		for ( i = 0; i < n; scanf("%lld",&c[i++]) ) ;
		for ( i = 1; i < n; ++i ) {
			if ( c[i-1] >= 0 ) 
				ans += c[i-1], c[i] += c[i-1];
			else 
				ans -= c[i-1], c[i] += c[i-1];
		}
		printf("%lld\n",ans);
	}
	return 0;
}

