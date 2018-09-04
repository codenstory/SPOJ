/*
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
#define tol (1e-13)
using namespace std;
typedef unsigned long long u64;
typedef long long i64;
#define MAXK (22)
#define N (0x400)
#define oo (1<<30)

int n,c[N];

int main() {
	int i,j,k,ts,cs= 0,ans,least;
	for ( cin >> ts; ts--; ) {
		for ( cin >> n, i= 0; i < n; cin >> c[i++] ) ;
		for ( k= c[0], i= 1; i < n; k^= c[i++] ) ;
		if ( k ) {
			printf("Case #%d: NO\n",++cs);
			continue ;
		}
		for ( least= +oo, ans= 0, i= 0; i < n; ans+= c[i++] ) 
			if ( c[i] < least )
				least= c[i];
		printf("Case #%d: %d\n",++cs,ans-least);
	}
	return 0;
}

