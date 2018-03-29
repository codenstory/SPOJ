/*
 * MAIN8_C
 * TOPIC: binary search
 * status: 
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (1<<21)
#define oo (1ULL<<62)
typedef unsigned long long i64;
using namespace std;

int n;
i64 c[N],m;

i64 f( i64 k ) {
	i64 ans = 0;
	for ( int i = 0; i < n; ++i )
		ans += c[i]/k;
	return ans;
}

int main() {
	int i,j,k,ts;
	i64 low,high,mid,total;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %llu",&n,&m), total = 0;
		for ( i = 0; i < n; ++i ) scanf("%llu",&c[i]), total += c[i];
		if ( total < m ) {
			puts("0");
			continue ;
		}
		for ( low = 1, high = +oo; low+1 < high; f(mid = (low+high)>>1) >= m ? (low=mid):(high=mid) ) ;
		printf("%llu\n",low);
	}
	return 0;
}

