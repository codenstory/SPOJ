/*
 * BRIDGE: Building Bridges
 * TOPIC: dp
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

int n,x[N],y[N],z[N];
pair<int,int> p[N];

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%d",x+i);
		for ( i = 1; i <= n; ++i )
			scanf("%d",y+i);
		for ( i = 1; i <= n; ++i )
			p[i] = make_pair(x[i],y[i]);
		sort(p+1,p+n+1);
		for ( z[0] = 0, i = 1; i <= n; ++i ) 
			for ( z[i] = 1, j = 1; j <= i-1; ++j )
				if ( p[j].second <= p[i].second && z[j]+1 > z[i] )
					z[i] = 1+z[j];
		for ( k = 0, i = 1; i <= n; ++i )
			k = max(k,z[i]);
		printf("%d\n",k);
	}
	return 0;
}

