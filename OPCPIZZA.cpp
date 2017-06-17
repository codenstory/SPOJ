/*
 * TOPIC: binary search, left-right range
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
#define N (1<<21)
typedef long long i64;
using namespace std;

int n;
i64 c[N],m;

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %lld",&n,&m);
		for ( i = 0; i < n; ++i )
			scanf("%lld",c+i);
		sort(c,c+n), k = 0;
		for ( i = 0; i < n-1; ++i ) {
			int low,high,mid,left,right;
			if ( c[low = i+1]+c[i] > m ) break ;
			if ( c[high = n-1]+c[i] < m ) continue ;
			if ( c[low]+c[i] == m ) {
				if ( c[high]+c[i] == m ) {
					k += high-low+1;
					continue ;
				}
				assert( c[high]+c[i] > m );
				left = low;
			}
			else {
				assert( c[low]+c[i] < m );
				assert( c[high]+c[i] >= m );
				for ( ;low+1<high; c[i]+c[mid=(low+high)/2]<m?(low=mid):(high=mid) );
				assert( c[low]+c[i] < m );
				assert( c[high]+c[i] >= m );
				if ( c[high]+c[i] == m ) 
					left = high;
				else continue ;
			}
			assert( c[i]+c[low = left] == m );
			if ( c[i]+c[high=n-1] == m ) 
				right = n-1;
			else {
				assert( c[i]+c[high=n-1] > m);
				for ( ;low+1<high; c[mid=(low+high)/2]+c[i]>m?(high=mid):(low=mid) );
				right = low;
			}
			k += right-left+1;
		}
		printf("%d\n",k);
	}
	return 0;
}

