/*
 * KOIREP
 * TOPIC: sliding window, invariants
 * status: Accepted
 */
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define oo (1LL<<60)
using namespace std;
#define N 0x400
#include <vector>
typedef long long i64;

i64 c[N][N],ans;
int m,n,last_seen[N];
vector<pair<i64,int> > vec;

int main() {
	int i,j,k,t;
	for ( ;2 == scanf("%d %d",&m,&n); ) {
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; scanf("%lld",&c[i][j++]) ) ;
		for ( vec.clear(), i = 1; i <= m; ++i )
			for ( last_seen[i] = -1, j = 1; j <= n; ++j )
				vec.push_back(make_pair(c[i][j],i));
		sort(vec.begin(),vec.end());
		for ( ans=+oo, i=0, last_seen[vec[i].second]=0, k=1, j=i+1; k<m && j<(int)vec.size(); last_seen[vec[j].second]=j, ++j ) 
			if ( last_seen[vec[j].second] == -1 && ++k ) ;
		if ( k < m ) {
			assert( 0 );
			continue ;
		}
		do {
			assert( i < j );
			assert( vec[j-1].first >= vec[i].first );
			assert( last_seen[vec[j-1].second] == j-1 );
			ans = min(ans,vec[j-1].first-vec[i].first);
			if ( last_seen[t = vec[i].second] == i ) {
				if ( ++i == (int)vec.size() ) break ;
				assert( vec[i].second != t );
				for ( last_seen[t]=-1, last_seen[vec[j].second]=j; (++j) < (int)vec.size() && last_seen[t] == -1; last_seen[vec[j].second] = j ) ;
				assert( i < j );
				if ( last_seen[t] != -1 ) {
					assert( j-1 < (int)vec.size() );
					assert( vec[j-1].first >= vec[i].first );
					ans = min(ans,vec[j-1].first-vec[i].first);
				}
				else break ;
			}
			else {
				assert( last_seen[t] > i );
				for ( ;++i < j && last_seen[vec[i].second] != i; ) ;
				ans = min(ans,vec[j-1].first-vec[i].first);
			}
		} while ( 1 );
		printf("%lld\n",ans);
	}
	return 0;
}

