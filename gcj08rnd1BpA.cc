/*
 * TOPIC: basic combinatorics
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
typedef long long i64;
#define N (1<<17)
using namespace std;

i64 A,B,C,D,MOD,x0,Y0,x[N],y[N],c[3][3];
int n;

int main() {
	int i,j,k,ts,cs= 0,t,l,r;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %lld %lld %lld %lld %lld %lld %lld",&n,&A,&B,&C,&D,&x0,&Y0,&MOD);
		for ( x[0]= x0, y[0]= Y0, i= 1; i < n; ++i ) {
			x[i]= (A*x[i-1]+B)%MOD, y[i]= (C*y[i-1]+D)%MOD;
			assert( x[i] >= 0 && y[i] >= 0 );
		}
		memset(c,0,sizeof c);
		for ( i= 0; i < n; ++i )
			++c[x[i]%= 3][y[i]%= 3];
		i64 ans= 0;
		for ( i= 0; i < 3; ++i )
			for ( j= 0; j < 3; ++j )
				for ( k= 0; k < 3; ++k )
					for ( t= 0; t < 3; ++t )
						if ( i != k || j != t )
							for ( l= 0; l < 3; ++l )
								for ( r= 0; r < 3; ++r )
									if ( (l != i || r != j) && (l != k || r != t) )
										if ( !((i+k+l)%3) && !((j+t+r)%3) ) 
											ans+= c[i][j]*c[k][t]*c[l][r];
		assert( !(ans % 6) );
		for ( ans/= 6, i= 0; i < 3; ++i )
			for ( j= 0; j < 3; ++j )
				for ( k= 0; k < 3; ++k )
					for ( t= 0; t < 3; ++t )
						if ( k != i || t != j )
							if ( !((2*i+k)%3) && !((2*j+t)%3) )
								ans+= c[i][j]*(c[i][j]-1)/2*c[k][t];
		for ( i= 0; i < 3; ++i )
			for ( j= 0; j < 3; ++j )
				if ( c[i][j] >= 3 )
					ans+= c[i][j]*(c[i][j]-1)*(c[i][j]-2)/6;
		printf("Case #%d: %lld\n",++cs,ans);
	}
	return 0;
}

