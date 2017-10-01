/*
 * RRSCHED
 * TOPIC: BIT
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define L(k) ((k)&((~(k))+1ULL))
enum { DIGL = 4, BASE = 10000, LEN = 0x10 };
typedef long long i64;
using namespace std;

int n,T[N],e[N],K,m;
i64 tr[N<<1],a[N],b[N],suf[N],total;
set<int> s;
unordered_map<int,int> mp,rt;

void update( unsigned int i, i64 dt ) {
	for ( ;i < (1<<K); tr[i] += dt, i += L(i) ) ;
}

i64 prefix( unsigned int i ) {
	i64 ans = 0;
	for ( ;i; ans += tr[i], i &= ~L(i) ) ;
	return ans;
}

i64 sum( unsigned int i, unsigned int j ) {
	return prefix(j)-prefix(i-1);
}

i64 *load( i64 n ) {
	i64 *x = (i64 *)malloc(LEN*sizeof *x);
	int i = 0;
	do {
		x[i++] = (n%BASE);
	} while ( n /= BASE ) ;
	return x;
}

i64 *add( i64 *x, i64 *y ) {
	i64 *z = (i64 *)malloc(LEN*sizeof *z);
	int i;
	for ( i = 0; i < LEN; ++i )
		z[i] = x[i]+y[i];
	for ( i = 0; i < LEN-1; ++i )
		if ( z[i] >= BASE )
			z[i+1] += z[i]/BASE, z[i] %= BASE;
	return z;
}

int is_negative( i64 *x ) { return x[LEN-1] >= BASE/2; }

i64 *sub( i64 *x, i64 *y ) {
	i64 *z;
	return z;
}

int main() {
	int i,j,k;
	for ( ;1 == scanf("%d",&n); ) {
		mp.clear(), rt.clear(), s.clear(), total = 0;
		for ( i = 0; i < n; ++i )
			scanf("%d",&T[i]), s.insert(e[i]=T[i]), total += e[i];
		m = 0;
		for ( set<int> :: iterator it = s.begin(); it != s.end(); ++it )
			rt[mp[*it] = ++m] = *it;
		for ( K = 0; (1<<K) <= m; ++K ) ;
		for ( i = 0; i <= (1<<K); tr[i++] = 0 ) ;
		for ( i = n-1; i >= 0; --i ) {
			a[i] = sum(mp[T[i]],(1<<K)-1);
			update(mp[T[i]],1);
		}
		for ( sort(e,e+n), suf[n] = 0, i = n-1; i >= 0; --i )
			suf[i] = suf[i+1] + e[i];
		for ( i = 0; i < n; ++i ) {
			i64 low, high, mid;
			if ( e[n-1] == T[i] ) {
				b[i] = 0;
				continue ;
			}
			assert( e[n-1] > T[i] );
			for ( low = 0, high = n-1; low+1 < high; e[mid=(low+high)>>1] > T[i] ? (high=mid) : (low=mid) ) ;
			assert( e[low] == T[i] );
			assert( e[high] > T[i] );
			assert( suf[high] >= T[i]*(n-high) );
			b[i] = suf[high]-T[i]*(n-high);
		}
		for ( i = 0; i < n; ++i )
			printf("%lld\n",total-a[i]-b[i]);
		break ;
	}
	return 0;
}

