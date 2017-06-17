/*
 * BADXOR: Bad XOR
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
#define N 0x456
#define MAXV 0x456
#define MOD 100000007LL
#define MAXC 0x10
#include <queue>
typedef long long i64;
using namespace std;

int m,n,a[N],b[N],seen[N][MAXV],yes,in_b[MAXV];
i64 z[N][MAXV],deg[N];

queue<pair<int,unsigned int> > q;

int main() {
	int i,j,k,ts,cs = 0;
	unsigned int u,v;
	i64 ans;
	for ( deg[0] = 1, i = 1; i < N; ++i )
		deg[i] = (deg[i-1]+deg[i-1])%MOD;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		scanf("%d %d",&n,&m);
		for ( i = 1; i <= n; ++i )
			scanf("%d",a+i);
		for ( j = 1; j <= m; ++j )
			scanf("%d",b+j), in_b[b[j]] = yes;
		printf("Case %d: ",++cs);
		memset(z,0,sizeof z);
		for ( ans = 0, q.push(make_pair(0,0)), z[0][0] = 1, seen[0][0] = yes; !q.empty(); ) {
			pair<int,unsigned int> r = q.front();
			q.pop();
			i = r.first, u = r.second;
			if ( i+1 <= n ) {
				z[i+1][u] += z[i][u], z[i+1][u] %= MOD;
				if ( seen[i+1][u] != yes )
					seen[i+1][u] = yes, q.push(make_pair(i+1,u));
			}
			if ( i < n ) {
				z[i+1][v = u^a[i+1]] += z[i][u], z[i+1][v] %= MOD;
				if ( seen[i+1][v] != yes )
					seen[i+1][v] = yes, q.push(make_pair(i+1,v));
			}
			if ( i == n && in_b[u] == yes )
				ans += z[i][u], ans %= MOD;
		}
		printf("%lld\n",(deg[n]-ans+MOD)%MOD);
	}
	return 0;
}

