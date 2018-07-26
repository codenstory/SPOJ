/*
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#define N 0x80
#define MAXVAL (0x80)
using namespace std;
typedef long long i64;
enum { S, T };

int m[2],A[2][N],cur[2][MAXVAL],ptr[2][MAXVAL];
i64 a[2][N],z[N][N];
vector<int> pos[2][MAXVAL];

i64 f( const int ch, int *ii, int t, i64 remainder ) {
	assert( A[S][ii[S]] == ch );
	assert( A[T][ii[T]] == ch );
	assert( remainder >= 1 );
	i64 w= min(remainder,a[t^1][ii[t^1]]),
		candidate1= w+z[ii[S]-1][ii[T]-1],
		new_remainder= (max(remainder,a[t^1][ii[t^1]])-w);
	if ( !new_remainder ) 
		return candidate1;
	assert( remainder != a[t^1][ii[t^1]] );
	int l= remainder>=a[t^1][ii[t^1]]?t:t^1;
	if ( --ptr[l^1][ch] >= 0 ) {
		ii[l^1]= pos[l^1][ch][ptr[l^1][ch]];
		return max(candidate1,w+f(ch,ii,l,new_remainder));
	}
	return candidate1;
}

int main() {
	int i,j,k,cs= 0,ts,t,l;
	i64 remainder;
	for ( cin >> ts; ts--; ) {
		for ( t= S; t <= T; cin >> m[t++] ) ;
		for ( t= S; t <= T; ++t )
			for ( i= 1; i <= m[t]; cin >> a[t][i] >> A[t][i], ++i ) ;
		for ( t= S; t <= T; ++t )
			for ( k= 0; k < MAXVAL; ++k )
				cur[t][k]= 0, pos[t][k].clear();
		for ( t= S; t <= T; ++t ) 
			for ( i= 1; i <= m[t]; ++i )
				pos[t][A[t][i]].push_back(i);
		memset(z,0,sizeof z);
		int ii[2];
		for ( ii[S]= 1; ii[S] <= m[S]; ++ii[S] )
			for ( ii[T]= 1; ii[T] <= m[T]; ++ii[T] ) {
				z[ii[S]][ii[T]]= max(z[ii[S]-1][ii[T]],z[ii[S]][ii[T]-1]);
				if ( A[S][ii[S]] != A[T][ii[T]] ) 
					continue ;
				int ch= A[S][ii[S]];
				for ( t= S; t <= T; ++t ) {
					if( pos[t][ch].size() ) {
						for ( cur[t][ch]= 0; pos[t][ch][cur[t][ch]] < ii[t]; ++cur[t][ch] ) ;
						assert( pos[t][ch][cur[t][ch]] == ii[t] ) ;
						ptr[t][ch]= cur[t][ch];
					}
				}
				int ni[2]= {ii[S],ii[T]};
				z[ii[S]][ii[T]]= max(z[ii[S]][ii[T]],f(ch,ni,T,a[T][ii[T]]));
			}
		printf("Case #%d: %lld\n",++cs,z[m[S]][m[T]]);
	}
	return 0;
}

