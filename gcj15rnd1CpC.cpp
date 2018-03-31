#include <bits/stdc++.h>
#define N (10100)
using namespace std;

int n,C,m,d[N];
long long e[N],V;

int main() {
	int i,j,k,cs= 0,ts,len;
	long long sum, needs_cover;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %lld",&C,&n,&V);
		for ( m= 0, i= 0; i < n; ++i )
			for ( scanf("%d",&k), j= 0; j < C; d[m++]= k, ++j ) ;
		assert( m == n*C );
		sort(d,d+m), len= 0;
		for ( sum= 0, needs_cover= 0, i= 0; needs_cover <= V; ) {
			if ( sum == needs_cover ) {
				++needs_cover;
				continue ;
			}
			if ( sum < needs_cover ) {
				if ( i < m && needs_cover == d[i] )
					++needs_cover, sum+= d[i++];
				else if ( i == m || needs_cover < d[i] )
					sum+= C*(e[len++]= needs_cover), ++needs_cover;
				else {
					assert( i < m && needs_cover > d[i] );
					sum+= d[i++];
				}
			}
			else {
				needs_cover= sum+1;
			}
		}
		printf("Case #%d: %d\n",++cs,len);
	}
	return 0;
}

