/*
 * DBALLZ: Help the Heroes
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
#include <map>
#define tol (1e-13)
#define N 0x400
#define MAXE (1000007)
#define oo (1<<30)
using namespace std;

int E,n,cost[N],value[N],ans,z[2][MAXE];

int main() {
	int i,j,k,t,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&E,&n);
		for ( i = 1; i <= n; ++i )
			scanf("%d",cost+i);
		for ( i = 1; i <= n; ++i )
			scanf("%d",value+i);
		for ( t = 0, i = 0; i <= E; ++i )
			z[0][i] = z[1][i] = -oo;
		for ( z[t][0] = 0, i = 1; i <= n; ++i ) {
			//for ( t^=1, k = 0; k <= E; ++k )
			//	z[t][k] = z[t^1][k];
			for ( k = 0; k <= E; ++k )
				//if ( z[t^1][k] > -oo )
				if ( z[t][k] > -oo )
					for ( j = 1; j <= 1; ++j )
						if ( z[t][k+j*cost[i]] < z[t][k]+j*value[i] )
							z[t][k+j*cost[i]] = z[t][k]+j*value[i];
		}
		for ( ans = 0, i = 0; i <= E; ++i )
			ans = max(ans,z[t][i]);
		printf("%d\n",ans);
		// z[i][cost] -- the maximum time to gain when using the first "i" items with budget "cost"
		/*
		for ( ans = 0, m[0].clear(), m[1].clear(), m[t=0][0] = 0, i = 1; i <= n; ++i ) {
			m[t^=1].clear();
			for ( map<int,int> :: iterator it = m[t^1].begin(); it != m[t^1].end(); ++it )
				for ( k = 0; k*cost[i]+it->first <= E; ++k )
					if ( m[t].find(k*cost[i]+it->first) == m[t].end() || m[t][k*cost[i]+it->first] < it->second+k*value[i] ) 
						m[t][k*cost[i]+it->first] = it->second+k*value[i];
		}
		for ( map<int,int> :: iterator it = m[t].begin(); it != m[t].end(); ++it )
			if ( it->second > ans )
				ans = it->second;
		printf("%d\n",ans);
		*/
	}
	return 0;
}

