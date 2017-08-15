/*
 * SUBSEQ
 * TOPIC: dp, prefix sums
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <set>
#include <unordered_map>
#define T (47LL)
#define N (1<<20)
typedef long long i64;
using namespace std;

i64 c[N],pref[N];
unordered_map<i64,i64> z;
int n;

int main() {
	int i,j,k,ts,t;
	i64 ans;
	for ( scanf("%d",&ts); ts-- && 1==scanf("%d",&n); ) {
		for ( z.clear(), i = 1; i <= n && 1 == scanf("%lld",&c[i++]); pref[i-1]=pref[i-2]+c[i-1] ) ;
		for ( z[0] = 1LL, ans = 0, i = 1; i <= n; ++i ) {
			i64 val = pref[i];
			if ( z.count(val-T) )
				ans += z[val-T];
			if ( !z.count(val) )
				z[val] = 1;
			else ++z[val];
		}
		printf("%lld\n",ans);
	}
	return 0;
}

