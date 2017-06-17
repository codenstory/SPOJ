/*
 * KAOS: Kaos
 * TOPIC: bitmasks, data-structures, set, upper_bound
 * status:
 */
#include <algorithm>
#include <vector>
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#define N (1<<17)
using namespace std;
typedef unsigned long long u64;

u64 conv( char *s ) {
	int i,j,n = strlen(s);
	u64 u = 0;
	for ( j = 57, i = 0; j >= 0 && i < n; ++i, j -= 5 )
		u |= (((u64)(s[i]-'a'+1))<<j);
	return u;
}

int n;
pair<u64,u64> v[N];

u64 f( u64 w, set<u64> &cnt ) {
	return (u64)distance(cnt.upper_bound(w),cnt.end());
}

int main() {
	int i,j,k;
	char s[0x10],t[0x10];
	u64 ans;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i ) {
			for ( scanf("%s",s), j = strlen(s)-1, k = 0; j >= 0; t[k++] = s[j--] ) ;
			t[k] = '\0';
			v[i] = make_pair(conv(s),conv(t));
			//printf("%llu %llu\n",v[i].first,v[i].second);
		}
		sort(v,v+n);
		set<u64> cnt;
		cnt.insert(v[0].second);
		for ( ans = 0, i = 1; i < n; ++i ) {
			ans += f(v[i].second,cnt);
			assert( cnt.find(v[i].second) == cnt.end() );
			cnt.insert(v[i].second);
		}
		printf("%llu\n",ans);
	}
	return 0;
}

