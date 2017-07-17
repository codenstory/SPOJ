/*
 * BRDGHRD
 * TOPIC: LIST in O(nlogn)
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
using namespace std;

int n,x[2][N],a[N],z[N];
pair<int,int> vec[N];

int main() {
	int i,j,k,ts,t,low,high,mid,len;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( t = 0; t <= 1; ++t )
			for ( i = 0; i < n; ++i )
				scanf("%d",&x[t][i]);
		for ( i = 0; i < n; ++i )
			vec[i] = make_pair(x[1][i],x[0][i]);
		sort(vec,vec+n);
		for ( i = 0; i < n; ++i )
			z[i] = vec[i].second;
		for ( len = 0, i = 0; i < n; ++i ) {
			if ( len == 0 || z[a[len-1]] <= z[i] ) {
				a[len++] = i;
				continue ;
			}
			if ( z[i] < z[a[0]] ) {
				a[0] = i;
				continue ;
			}
			assert( z[i] >= z[a[0]] );
			assert( z[i] < z[a[len-1]] );
			for ( low = 0, high = len-1; low+1 < high; z[a[mid=(low+high)/2]] > z[i]?(high=mid):(low=mid) );
			a[high] = i;
		}
		printf("%d\n",len);
	}
	return 0;
}

