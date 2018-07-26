/*
 * TOPIC: greedy
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <queue>
#define N (1001)
#define W (2002)
using namespace std;
#define oo (0xffffffffffffffffull)
#define enc(i,t,k) ((i) | ((t)<<10) | ((k) << 11))
enum { ONE_STAR, TWO_STAR };
typedef unsigned long long u64;
#define Q (1 << 22)

int a[N],b[N],n,
	_left[N],_right[N],m,head[N],tail[N],sum[N];
vector<int> w[N];
bool one_completed[N], completed[N];

int main() {
	int i,j,k,t,cs= 0,ts,l,remainder;
	unsigned int u,v;
	u64 ans;
	for ( cin >> ts; ts--; ) {
		for ( cin>>n, i= 0; i < n; cin>>a[i]>>b[i], ++i ) ;
		/*
		vector<pair<int,int>> vec;
		for ( i= 0; i < n; ++i )
			vec.push_back({b[i+1],a[i+1]});
		sort(vec.begin(),vec.end());
		for ( int l= 0; l < (int)vec.size(); ++l )
			a[l+1]= vec[l].second, b[l+1]= vec[l].first;
		for ( m= 0, i= 1; i <= n; i= j ) {
			for ( k= b[i]+2, j= i+1; j <= n && k >= b[j]; k+= 2, ++j ) ;
			_left[m]= i, _right[m++]= j-1, sum[m-1]= k;
		}
		for ( i= 0; i < m; ++i ) {
			for ( w[i].clear(), j= _left[i]; j <= _right[i]; w[i].push_back(a[j++]) ) ;
			sort(w[i].begin(),w[i].end());
		}
		ans= 0, k= 0;
		if ( b[_left[0]] <= k ) {
			k= sum[0], ans= _right[0]-_left[0]+1;
		}
		else {
			for ( l= 0, j= _left[0]; j <= _right[0] && k < b[_left[0]] && k >= w[0][l]; ++ans, ++k, ++j, ++l ) ;
			if ( k < b[_left[0]] ) 
				goto NO;
			k= sum[0], ans+= (_right[0]-_left[0]+1);
		}
		for ( i= 1; i < m; ++i ) {
			if ( b[_left[i]] <= k ) 
				k= sum[i]+(k-b[_left[i]]);
			else {
				for ( l= 0, j= _left[i]; j <= _right[i] && k < b[_left[i]] && k >= w[i][l]; ++ans, ++k, ++j, ++l ) ;
				if ( k < b[_left[i]] ) 
					goto NO;
				k= sum[i];
			}
			ans+= (_right[i]-_left[i]+1);
		}
		printf("Case #%d: %llu\n",++cs,ans);
		continue ;
NO:		printf("Case #%d: Too Bad\n",++cs);
		*/
		memset(completed,0,sizeof completed);
		memset(one_completed,0,sizeof one_completed);
		for ( t= 0, k= 0, ans= 0; t < n; ) {
			for ( i= 0; i < n; ++i ) {
				if ( completed[i] ) continue ;
				if ( k >= b[i] ) {
					completed[i]= true, ++t, ++ans;
					if ( one_completed[i] )
						k+= 1;
					else k+= 2;
					goto next;
				}
			}
			for ( j= -1, i= 0; i < n; ++i )
				if ( !completed[i] && !one_completed[i] ) 
					if ( k >= a[i] )
						if ( j == -1 || b[j] < b[i] )
							j= i;
			if ( j != -1 ) {
				one_completed[j]= 1, ++ans, ++k;
				goto next ;
			}
			break ;
			next: continue ;
		}
		if ( t < n )
			printf("Case #%d: Too Bad\n",++cs);
		else printf("Case #%d: %llu\n",++cs,ans);
	}
	return 0;
}

