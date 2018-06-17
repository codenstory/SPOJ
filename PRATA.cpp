/*
 * PRATA
 * TOPIC: binary search
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;
typedef long long i64;
#define oo (1LL<<30)
#define N 0x40

i64 prata,p[N];
int n;

i64 f( i64 r, i64 time ) {
	i64 low, high, mid;
	for ( low= 0, high= time+2; low+1 < high; ) {
		mid= (low+high)>>1;
		if ( mid*(mid-1)/2*r <= time )
			low= mid;
		else
			high= mid;
	}
	return low-1;
}

i64 cook( i64 time ) {
	i64 ans= 0;
	for ( int i= 0; i < n; ans+= f(p[i++],time) ) ;
	return ans;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%lld",&prata);
		for ( scanf("%d",&n), i= 0; i < n; ++i )
			scanf("%lld",&p[i]);
		i64 good, bad, mid;
		if ( cook(bad= 0) >= prata )
			puts("0");
		else {
			for ( good= +oo; bad+1 < good; ) {
				mid= (good+bad)>>1;
				if ( cook(mid) >= prata )
					good= mid;
				else 
					bad= mid;
			}
			printf("%lld\n",good);
		}
	}
	return 0;
}

