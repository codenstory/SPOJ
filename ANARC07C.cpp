/*
 * ANARC07C: Rotating Rings
 * TOPIC: simulation
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;
#include <vector>
#define N 0x400

int g[N][N],m[N][N];
vector<int> ring[N],vec[N];

bool equivalent( const vector<int> &a, const vector<int> &b ) {
	int i,j,k,n= a.size();
	if ( a.size() != b.size() )
		return false ;
	for ( i= 0; i < n && b[i] != a[0]; ++i ) ;
	if ( i == n )
		return false ;
	for ( k= 0; k < n; ++k ) {
		if ( b[i] != a[k] )
			return false ;
		++i, i%= n;
	}
	if ( k == n ) return true ;
	/*
next:
	for ( i= 0; i < n && b[i] != a[0]; ++i ) ;
	for ( k= 0; k < n; ++k ) {
		if ( b[i] != a[k] )
			return false ;
		--i, i+= n, i%= n;
	}
	*/
	return k == n;
}

int main() {
	int i,j,k,cs= 0,n;
	for ( ;1 == scanf("%d",&n) && n; ) {
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; scanf("%d",&g[i][j++]), --g[i][j-1] ) ;
		for ( k= 0, i= 0; i < n; ++i )
			for ( j= 0; j < n; m[i][j++]= k++ ) ;
		assert( k == n*n );
		bool ok= true;
		for ( k= 0; k < n/2+(n&1) && ok; ++k ) {
			vec[k].clear(), ring[k].clear();
			for ( i= k, j= k; j < n-k; ++j ) {
				vec[k].push_back(m[i][j]);
				ring[k].push_back(g[i][j]);
			}
			for ( i= k+1, j= n-k-1; i < n-k; ++i ) {
				vec[k].push_back(m[i][j]);
				ring[k].push_back(g[i][j]);
			}
			for ( i= n-k-1, j= n-k-2; j >= k; --j ) {
				vec[k].push_back(m[i][j]);
				ring[k].push_back(g[i][j]);
			}
			for ( i= n-k-2, j= k; i > k; --i ) {
				vec[k].push_back(m[i][j]);
				ring[k].push_back(g[i][j]);
			}
			ok&= equivalent(ring[k],vec[k]);
		}
		printf("%d. %s\n",++cs,ok?"YES":"NO");
	}
	return 0;
}

