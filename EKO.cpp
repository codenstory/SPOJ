/*
 * EKO
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
#define N (1<<20)
typedef long long i64;
using namespace std;

int n;
i64 h[N],m,prefix[N];

i64 sum( int i, int j, i64 l ) {
	return prefix[j]-prefix[i-1]-l*(j-i+1);
}

int main() {
	int i,j,k,low,high,mm,pos;
	i64 good,bad,mid;
	for ( ;2 == scanf("%d %lld",&n,&m); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%lld",&h[i]);
		for ( sort(h+1,h+n+1), i = 1; i <= n; ++i )
			prefix[i] = prefix[i-1]+h[i];
		for ( bad = h[n]+1, good = 0; good+1 < bad; ) {
			mid = (good+bad)/2;
			if ( mid <= h[1] ) {
				pos = 1;
			}
			else if ( mid > h[n] ) {
				pos = n+1;
			}
			else {
				assert( h[low = 1] < mid );
				assert( h[high=n] >= mid );
				for ( ;low+1 < high; h[mm=(low+high)/2] < mid ? (low=mm):(high=mm) ) ;
				pos = high;
				assert( h[low] < mid );
				assert( h[high] >= mid );
			}
			if ( pos == n+1 ) {
				bad = mid;
				continue ;
			}
			assert( h[pos] >= mid && (pos==0||h[pos-1]<mid) );
			if ( sum(pos,n,mid) >= m ) good = mid;
			else bad = mid;
		}
		/*
		if ( good <= h[1] ) pos = 1;
		else for ( low = 1, high = n; low+1 < high; h[mm=(low+high)/2] >= good ? (high = mm):(low = mm) ) ;
		printf("%lld\n",h[high]);*/
		printf("%lld\n",good);
	}
	return 0;
}

