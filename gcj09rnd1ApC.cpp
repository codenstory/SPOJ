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
#include <cfloat>
#define N (0x40)
#define oo (DBL_MAX-0x400)

int m,n;
double S[N],z[N],fac[N],c[N][N],p[N][N];

double transition_probability( int i, int j ) {
	if ( j < i ) return 0.00;
	if ( p[i][j] < +oo )
		return p[i][j];
	return p[i][j]= exp(c[n-i][j-i]+c[i][i-(j-m)]-c[n][m]);
}

int main() {
	int i,j,k,ts,cs = 0,u,v;
	for ( fac[0]= fac[1]= 0, i= 2; i < N; ++i )
		fac[i]= fac[i-1]+log(i);
	for ( i= 1; i < N; ++i )
		for ( j= 1; j <= i; ++j )
			c[i][j]= fac[i]-fac[j]-fac[i-j];
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( i= 0; i <= n; ++i )
			for ( j= 0; j <= n; ++j )
				p[i][j]= +oo;
		for ( S[m]= 1, v= m+1; v <= n; ++v )
			for ( S[v]= 0, i= 1; i <= m; ++i ) 
				if ( (u= v-i) >= m ) 
					S[v]+= S[u]*transition_probability(u,v)/(1-transition_probability(u,u));
		for ( z[m]= 1, v= m+1; v <= n; ++v )
			for ( z[v]= 0, i= 1; i <= m; ++i )
				if ( (u= v-i) >= m ) {
					double pp= transition_probability(u,v),
						   qq= transition_probability(u,u);
					//z[v]+= pp*S[u]/(1-qq)/S[v]*(pp/(1-qq)*z[u]+pp/pow(1-qq,2)*S[u]);
					z[v]+= (pp/(1-qq)*z[u]+pp/pow(1-qq,2)*S[u]);
					//z[v]+= pp*S[u]/(1-qq)/S[v]*(z[u]+S[u]/(1-qq));
				}
		printf("Case #%d: %lf\n",++cs,z[n]);
	}
	return 0;
}

