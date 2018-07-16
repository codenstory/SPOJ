/*
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (0x400)
using namespace std;

double fac[N],c[N][N],d[N],e[N],
	   path_probability[N],z[N];
int n,m;

bool is_pribability( double p ) {
	return 0 <= p && p <= 1;
}

double probability_via( int j, int k ) {
	if ( j < k ) return 0.00;
	if ( j == 1 || k == 1 ) return 0.00;
	assert( j >= 2 );
	double t;
	if ( k == 0 ) {
		return path_probability[j]/exp(log(1-exp(d[j]-fac[j]))+fac[j]);
	}
	assert( k >= 2 );
	assert( is_pribability(t= exp(c[j][j-k]+d[k]-fac[j])) );
	double res= path_probability[j] / (1-exp(d[j]-fac[j])) * exp(c[j][j-k]+d[k]-fac[j]);
	return res;
}

int main() {
	int i,j,k,cs= 0,ts;
	for ( fac[0]= 0.00, i= 1; i < N; ++i )
		fac[i]= fac[i-1]+log(i);
	for ( i= 1; i < N; ++i )
		for ( j= 1; j <= i; ++j )
			c[i][j]= fac[i]-fac[i-j]-fac[j];
	double F;
	for ( F= 1.00, e[0]= 1, i= 1; i < 10; ++i ) 
		F/= i, e[i]= e[i-1]+((i&1)?-1:1)*F;
	for ( ;i < N; ++i ) e[i]= exp(-1);
	for ( i= 2; i < N; ++i ) 
		d[i]= log(e[i])+fac[i];
	for ( scanf("%d",&ts); ts--; ) {
		for ( m= 0, scanf("%d",&n), i= 0; i < n; ++i )
			scanf("%d",&k), m+= (--k==i?0:1);
		if ( !m ) {
			printf("Case #%d: 0.000000\n",++cs);
			continue ;
		}
		for ( path_probability[m]= 1.00, k= m-1; k >= 0; --k )
			if ( k != 1 )
				for ( path_probability[k]= 0.00, j= m; j >= k+1; --j ) 
					if( j != 1 )
						path_probability[k]+= probability_via(j,k);
		for ( z[m]= 0.00, k= m-1; k >= 0; --k ) 
			if ( k != 1 )
			for ( z[k]= 0.00, j= m; j >= k+1; --j ) {
				if ( j == 1 ) continue ;
				double cycle= exp(d[j]-fac[j]);
				assert( is_pribability(cycle) );
				z[k]+= (z[j]+cycle/(1-cycle)+1) * probability_via(j,k) / path_probability[k];
			}
		printf("Case #%d: %lf\n",++cs,z[0]);
	}
	return 0;
}

