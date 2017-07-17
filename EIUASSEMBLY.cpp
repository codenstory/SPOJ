/*
 * EIUASSEMBLY
 * TOPIC: binary search
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

int n;
i64 budget,m[N],p[N];

bool f( i64 T ) {
	i64 b = budget,deficit,i;
	for ( i = 1; i <= n && b >= 0; ++i ) 
		if ( p[i] < T ) {
			deficit = T-p[i];
			b -= deficit*m[i];
		}
	return i >= n+1 && b >= 0;
}

int main() {
	int i,j,k,ts;
	i64 bad,good,mid,maxp,minm,minp;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %lld",&n,&budget);
		for ( maxp = -(1LL<<31), minp = (1LL<<31), i = 1; i <= n; ++i ) {
			scanf("%lld %lld",p+i,m+i);
			if ( maxp < p[i] || maxp == p[i] && minm > m[i] )
				maxp = p[i], minm = m[i];
			if ( minp > p[i] ) minp = p[i];
		}
		if ( f(bad = budget/minm+minm+maxp) ) {
			printf("%lld\n",bad);
			continue ;
		}
		for ( good = minp; good+1 < bad; ) 
			if ( f(mid = (good+bad)/2) )
				good = mid;
			else bad = mid;
		printf("%lld\n",good);
	}
	return 0;
}

