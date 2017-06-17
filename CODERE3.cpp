/*
 * CODERE3: coder Express 3
 * TOPIC: dp, lis, O(nlogn), binary search
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
#define N 0x400
using namespace std;

int n,c[N],a[N],b[N],len,best_inc[N],best_dec[N],ans;

int main() {
	int i,j,k,ts,low,high,mid;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d",&n), i = 1; i <= n; scanf("%d",&c[i++]) ) ;
		for ( len = 0, i = 1; i <= n; ++i ) {
			if ( len == 0 || c[a[len]] < c[i] ) {
				++len, a[len] = i, best_inc[i] = len;
				continue ;
			}
			assert( c[i] <= c[a[len]] ) ;
			if ( c[i] <= c[a[1]] ) {
				a[1] = i, best_inc[i] = 1;
				continue ;
			}
			assert( c[a[1]] < c[i] ) ;
			for ( low=1, high=len; low+1<high; c[a[mid=(low+high)/2]]<c[i]?(low=mid):(high=mid) ) ;
			assert( c[a[low]] < c[i] ) ;
			assert( c[i] <= c[a[high]] ) ;
			a[high]=i, best_inc[i]=high;
		}
		for ( len = 0, i = n; i >= 1; --i ) {
			if ( len == 0 || c[a[len]] < c[i] ) {
				++len, a[len] = i, best_dec[i] = len;
				continue ;
			}
			assert( c[i] <= c[a[len]] ) ;
			if ( c[i] <= c[a[1]] ) {
				a[1] = i, best_dec[i] = 1;
				continue ;
			}
			assert( c[a[1]] < c[i] ) ;
			for ( low=1, high=len; low+1<high; c[a[mid=(low+high)/2]]<c[i]?(low=mid):(high=mid) ) ;
			assert( c[a[low]] < c[i] ) ;
			assert( c[i] <= c[a[high]] ) ;
			a[high]=i, best_dec[i]=high;
		}
		for ( ans = 0, i = 1; i <= n; ++i )
			ans = max(ans,best_inc[i]+best_dec[i]-1);
		printf("%d\n",ans);
	}
	return 0;
}

