/*
 * DIVSEQ
 * TOPIC: dp, trivial
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
#define MAXK 0x400
#define MOD 1000000009LL
using namespace std;
typedef long long i64;

int n,m;
vector<int> divisors[N];
i64 z[N][MAXK];

int main() {
	int i,j,k,t;
	i64 ans,w;
	for ( i = 1; i < MAXK; ++i )
		for ( j = 1; j <= i; ++j )
			if ( !(i%j) )
				divisors[i].push_back(j);
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		memset(z,0,sizeof z);
		for ( k = 1; k <= m; z[1][k++] = 1 ) ;
		for ( i = 2; i <= n; ++i ) 
			for ( k = 1; k <= m; ++k )
				if ( w = z[i-1][k] ) {
					for ( t = 2; t*k <= m; ++t )
						z[i][t*k] += w, z[i][t*k] %= MOD;
					for ( int l = 0; l < (int)divisors[k].size(); ++l ) {
						t = divisors[k][l], z[i][t] += w, z[i][t] %= MOD;
						assert( !(k%t) );
						assert( t <= m );
					}
				}
		for ( ans = 0, k = 1; k <= m; ans += z[n][k++], ans %= MOD ) ;
		printf("%lld\n",ans);
	}
	return 0;
}

