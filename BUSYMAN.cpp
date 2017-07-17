/*
 * BUSYMAN
 * TOPIC: greedy, activity selection, binary search, interview
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
#define oo (1<<30)
using namespace std;

pair<int,int> a[N];
int n,z[N];

struct comparator {
	bool operator () ( const pair<int,int> &a, const pair<int,int> &b ) {
		if ( a.second == b.second )
			return a.first < b.first;
		return a.second < b.second;
	}
};

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%d %d",&a[i].first,&a[i].second);
		sort(a+1,a+1+n,comparator());
		for ( z[0] = 0, i = 1; i <= n; z[i++] = -oo ) ;
		for ( i = 1; i <= n; ++i ) {
			int good,bad,mid;
			z[i] = max(z[i-1],1);
			if ( a[good = 1].second > a[i].first ) 
				continue ;
			assert( a[good].second <= a[i].first );
			if ( a[bad = i-1].second <= a[i].first ) {
				z[i] = max(z[i],z[i-1]+1);
				continue ;
			}
			assert( a[bad].second > a[i].first );
			for ( ;good+1<bad; a[mid=(good+bad)/2].second > a[i].first ? (bad=mid):(good=mid) ) ;
			z[i] = max(z[i],z[good]+1);
		}
		printf("%d\n",z[n]);
	}
	return 0;
}

