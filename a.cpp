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
#define tol (1e-13)
using namespace std;

int good( int u ) {
	int i;
	for ( i = 31; i >= 1; --i )
		if ( (u>>i)&1 )
			if ( (u>>(i-1))&1 )
				return 0;
	return 1;
}

int main() {
	int i,j,k,n,ans = 0;
	for ( ;1 == scanf("%d",&n); ) {
		for ( ans = 0, i = 0; i <= n; ++i )
			if ( good(i) ) ++ans;
		printf("%d\n",ans);
	}
	return 0;
}

