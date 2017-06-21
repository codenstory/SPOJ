/*
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <map>
using namespace std;
#define N 1000100
typedef long long i64;

int n;
i64 c[N],pref[N];
map<i64,i64> m;

i64 f( i64 x ) {
	return (x&1)?((x-1)>>1)*x:(x>>1)*(x-1);
}

int main() {
	int i,j,k,ts;
	i64 ans;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d",&n), i = 1; i <= n; scanf("%lld",&c[i++]) ) ;
		for ( ans = 0, m.clear(), m[0] = 0, i = 1; i <= n; ++i )
			if ( m.count(pref[i]=pref[i-1]+c[i]) )
				++m[pref[i]];
			else m[pref[i]] = 1;
		for ( map<i64,i64> :: iterator it = m.begin(); it != m.end(); ++it )
			ans += f(it->second);
		printf("%lld\n",ans+m[0]);
	}
	return 0;
}

