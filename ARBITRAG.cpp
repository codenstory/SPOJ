/*
 * ARBITRAG
 * TOPIC: Floyd-Warshall
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
using namespace std;
#define N (0x400)
#include <cfloat>
#define oo (DBL_MAX-0x400)

map<string,int> m;
int n;
double d[N][N];

int main() {
	int i,j,k,cs = 0;
	for ( ;1 == scanf("%d",&n) && n; ) {
		for ( m.clear(), i = 0; i < n; ++i ) {
			char tmp[0x400];
			scanf("%s",tmp);
			m[string(tmp)] = i;
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( i != j )
					d[i][j] = -oo;
				else d[i][j] = 0;
		for ( scanf("%d",&k); k--; ) {
			char tmp0[0x400], tmp1[0x400];
			double dbl;
			scanf("%s %lf %s",tmp0,&dbl,tmp1);
			i = m[string(tmp0)], j = m[string(tmp1)];
			d[i][j] = log(dbl);
		}
		for ( k = 0; k < n; ++k )
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					if ( d[i][k] > -oo && d[k][j] > -oo )
						if ( d[i][j] < d[i][k]+d[k][j] )
							d[i][j] = d[i][k]+d[k][j];
		bool yes = false;
		for ( i = 0; i < n && !yes; ++i )
			if ( d[i][i] > 0 )
				yes = true ;
		printf("Case %d: %s\n",++cs,yes?"Yes":"No");
	}
	return 0;
}

