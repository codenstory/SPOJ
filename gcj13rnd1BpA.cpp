/*
 * status: Accepted
 */
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#define N (0x80)
#define oo (1LL<<61)
#define B(k) (1<<(k))
#define M(k) (B(k)-1)
typedef long long i64;
using namespace std;

i64 z[N][N],c[N],A,prefix[N];
int n;
queue<pair<int,int>> q;

i64 range_sum( int i, int j ) { return prefix[j]-prefix[i-1]; }
int f( i64 x ) {
	int good, bad, mid;
	if ( c[n] < x )
		return n;
	if ( c[1] >= x )
		return 0;
	for ( good= 1, bad= n; good+1 < bad; c[mid=(good+bad)/2]<x?(good= mid):(bad= mid) ) ;
	return good;
}

int steps_needed( i64 x, i64 y ) {
	int k;
	assert( x >= 2 );
	for ( k= 0; x <= y; ) 
		x+= (x-1), ++k;
	return k;
}

int main() {
	int i,j,k,cs= 0,ts,t,ans;
	for ( cin >> ts; ts--; ) {
		for ( cin >> A >> n, i= 1; i <= n; cin >> c[i++] ) ;
		if ( A == 1 ) {
			printf("Case #%d: %d\n",++cs,n);
			continue ;
		}
		for ( sort(c+1,c+n+1), i= 0; i <= n; ++i )
			for ( j= 0; j <= n; z[i][j++]= -oo ) ;
		for ( i= 1; i <= n; prefix[i]= prefix[i-1]+c[i], ++i ) ;
		for ( ans= n, z[0][0]= A, q.push({0,0}); !q.empty(); ) {
			pair<int,int> p= q.front(); q.pop();
			i= p.first, k= p.second;
			if ( i == n ) {
				ans= min(ans,k);
				continue ;
			}
			if ( k >= ans ) continue ;
			assert( i <= n-1 );
			j= f(z[i][k]);
			if ( j >= i+1 ) {
				if ( z[j][k] < z[i][k]+range_sum(i+1,j) )
					z[j][k]= z[i][k]+range_sum(i+1,j), q.push({j,k});
			}
			else {
				t= steps_needed(z[i][k],c[i+1]);
				if ( k+t <= n && z[i+1][k+t] < z[i][k]*B(t)-M(t)+c[i+1] )
					z[i+1][k+t]= z[i][k]*B(t)-M(t)+c[i+1], q.push({i+1,k+t});
			}
			for ( j= i+1; j <= n; ++j ) {
				t= j-i;
				if ( t+k <= n && z[j][k+t] < z[i][k] )
					z[j][k+t]= z[i][k], q.push({j,k+t});
				if ( z[i][k] > c[j] )
					if ( z[j][k] < z[i][k]+range_sum(i+1,j) )
						z[j][k]= z[i][k]+range_sum(i+1,j), q.push({j,k});
			}
		}
		printf("Case #%d: %d\n",++cs,ans);
	}
	return 0;
}

