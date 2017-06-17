/*
 * CRAN04: Audition
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
typedef long long i64;
#define N (1<<20)
using namespace std;

int c[N],n,K,pref[N];
char tmp[N];

int sum( int i, int j ) { return pref[j]-pref[i-1]; }

int main() {
	int i,j,k,ts,left,right,low,high,mid;
	i64 ans;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&K); ) {
		for ( scanf("%s",tmp+1), i = 1; i <= n; c[i] = tmp[i]-'0', ++i ) ;
		for ( i = 1; i <= n; pref[i] = pref[i-1]+c[i], ++i ) ;
		for ( ans = 0, i = 1; i <= n; ++i ) {
			if ( sum(i,n) < K ) break;
			assert( sum(i,n) >= K );
			if ( sum(i,i) > K ) continue ;
			if ( sum(i,n) == K ) 
				right = n;
			else {
				assert( sum(i,high = n) > K );
				assert( sum(i,low = i) <= K );
				for ( ;low+1<high; sum(i,mid=(low+high)/2)<=K?(low=mid):(high=mid) ) ;
				assert( sum(i,high) > K );
				assert( sum(i,low) <= K );
				if ( sum(i,low) < K ) continue ;
				assert( sum(i,low) == K );
				right = low;
			}
			if ( sum(i,i) == K )
				left = i;
			else {
				assert( sum(i,low = i) < K );
				assert( sum(i,high = n) >= K );
				for ( ;low+1<high; sum(i,mid=(low+high)/2)>=K?(high=mid):(low=mid) ) ;
				assert( sum(i,low) < K );
				assert( sum(i,high) >= K );
				if ( sum(i,high) > K ) continue ;
				left = high;
			}
			ans += (right-left+1);
		}
		printf("%lld\n",ans);
	}
	return 0;
}

