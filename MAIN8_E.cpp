/*
 * MAIN8_E
 * TOPIC: DP, binary search
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
#include <unordered_map>
#include <set>
#define tol (1e-13)
#define N (20100)
#define M (0x80)
#define oo (1<<30)
#include <map>
using namespace std;

char x[N],y[N];
int m,n;
unordered_map<char,vector<int> > p;
map<int,int> b[M];

int main() {
	int i,j,k,ts,t,l,low,high,mid;
	bool ok;
	for ( sscanf(fgets(x,sizeof x,stdin),"%d",&ts); ts--; ) {
		fgets(y+1,sizeof y,stdin), fgets(x+1,sizeof x,stdin);
		for ( n = 0; y[n+1] && y[n+1] != '\n'; ++n ) ;
		for ( m = 0; x[m+1] && x[m+1] != '\n'; ++m ) ;
		for ( i = 0; i <= m; b[i++].clear() ) ;
		for ( p.clear(), i = 1; i <= n; ++i )
			if ( p.count(y[i]) )
				p[y[i]].push_back(i);
			else p[y[i]].clear(), p[y[i]].push_back(i);
		for ( j = 0; j <= n; b[0][j] = j+1, ++j ) ;
		for ( ok = true, i = 1; i <= m && ok; ++i ) {
			if ( !p.count(x[i]) ) {
				ok = false;
				break ;
			}
			vector<int> &pos = p[x[i]];
			k = pos.size();
			if ( pos[k-1] < i ) {
				ok = false;
				break ;
			}
			if ( pos[0] >= i ) l = 0;
			else {
				for ( low=0, high=k-1; low+1<high; pos[mid=(low+high)/2]>=i?(high=mid):(low=mid) ) ;
				l = high;
			}
			if ( !b[i-1].size() ) {
				ok = false;
				break ;
			}
			map<int,int> :: iterator it;
			vector<pair<int,int> > vec;
			for ( vec.clear(), it = b[i-1].begin(); it != b[i-1].end(); vec.push_back(make_pair(it->first,it->second)), ++it ) ;
			for ( k = vec.size(); l < (int)pos.size(); ++l ) {
				j = pos[l];
				assert( x[i] == y[j] );
				if ( vec[k-1].first < j ) t = k-1;
				else if ( vec[0].first >= j ) continue ;
				else {
					for ( low=0, high=k-1; low+1<high; vec[mid=(low+high)/2].first>=j?(high=mid):(low=mid) ) ;
					t = low;
				}
				assert( vec[t].first < j );
				assert( t+1 == k || vec[t+1].first >= j );
				if ( b[i].count(j) ) {
					if ( b[i][j] < vec[t].second )
						b[i][j] = vec[t].second;
				}
				else {
					b[i][j] = vec[t].second;
				}
			}
		}
		if ( !ok )
			puts("-1");
		else {
			int ans = n+n+n;
			for ( map<int,int> :: iterator jt = b[m].begin(); jt != b[m].end(); ++jt )
				ans = min(ans,jt->first-jt->second+1);
			printf("%d\n",ans<n+n+n?ans:-1);
		}
	}
	return 0;
}

