/*
 * TRIPINV
 * TOPIC: BIT
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define L(k) ((k)&((~(k))+1ULL))
#define N (1<<18)
typedef long long i64;

int n,c[N],K;
i64 q[N],tr[N];

void update( i64 *t, int i, i64 dt ) {
	for ( ;i <= (1<<K); t[i] += dt, i += L(i) ) ;
}

i64 prefix( i64 *t, int i ) {
	i64 s = 0;
	for ( ;i; s += t[i], i &= ~L(i) ) ;
	return s;
}

i64 sum( i64 *t, int i, int j ) {
	if ( i > j ) return 0;
	return prefix(t,j)-prefix(t,i-1);
}

int main() {
	int i,j,k;
	i64 ans;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; scanf("%d",&c[i++]) ) ;
		for ( K = 0; (1<<K) < n; ++K ) ;
		memset(q,0,sizeof q), memset(tr,0,sizeof tr);
		for ( ans = 0, i = 1; i <= n; ++i ) {
			ans += sum(q,c[i]+1,n);
			update(q,c[i],sum(tr,c[i]+1,n));
			update(tr,c[i],1);
		}
		printf("%lld\n",ans);
	}
	return 0;
}

