/*
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <set>
#define N (0x80)
#define oo (1<<30)
enum { D, S };
using namespace std;

int n,m,P,T[N],
	norm[N][2];
pair<int,int> v[N];
set<int> preset;

int main() {
	int i,j,k,cs= 0,ts,l,r;
	for ( cin >> ts; ts--; ) {
		cin >> n >> m >> P;
		for ( i= 0; i < n; cin >> T[i++] ) ;
		for ( i= 0; i < n; ++i ) {
			int remainder= (T[i]%3);
			switch( remainder ) {
				case 0: norm[i][D]= T[i]>=3?T[i]/3:0;
						norm[i][S]= T[i]>=3?T[i]/3+1:-oo;
						break ;
				case 1: norm[i][D]= T[i]>=1?(T[i]-1)/3+1:0;
						norm[i][S]= T[i]>=4?(T[i]-1)/3+1:-oo;
						break ;
				case 2: norm[i][D]= T[i]>=2?(T[i]-2)/3+1:0;
						norm[i][S]= T[i]>=2?(T[i]-2)/3+2:-oo;
						break ;
			}
		}
		for ( k= 0, preset.clear(), i= 0; i < n; ++i )
			if ( norm[i][S] == -oo ) {
				preset.insert(i);
				if ( norm[i][D] >= P )
					++k;
			}
		for ( l= 0, i= 0; i < n; ++i ) {
			if ( preset.count(i) ) continue ;
			assert( norm[i][D] <= norm[i][S] );
			if ( norm[i][D] >= P ) 
				++k, ++l;
		}
		for ( j= min(m,n-l-(int)preset.size()), i= 0; i < n; ++i )
			if ( norm[i][D] < P && norm[i][S] >= P )
				if ( j-- > 0 )
					++k;
		printf("Case #%d: %d\n",++cs,k);
	}
	return 0;
}

