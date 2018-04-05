/*
 */
#include <bits/stdc++.h>
#define oo (1<<29)
#define N 0x400
using namespace std;

int n,z[N],seen[N],yes,p[N];
vector<pair<int,int>> vec;
vector<int> adj[N];

void dfs( int x ) {
	assert( seen[x] != yes );
	seen[x]= yes;
	for ( auto y: adj[x] )
		if ( seen[y] != yes )
			p[y]= x, dfs(y);
		else {
			assert( p[x] == y );
		}
}

int f( int x ) {
	if ( seen[x] == yes )
		return z[x];
	seen[x]= yes;
	int a= -oo, b= -oo;
	for ( auto y: adj[x] )
		if ( p[y] == x ) {
			auto res= f(y);
			if ( res >= a )
				b= a, a= res;
			else if ( res > b )
				b= res;
			else {
				assert( res <= b );
			}
		}
	return z[x]= (a>-oo&&b>-oo)?1+a+b:1;
}

int dp( const int root ) { return ++yes, f(root); }

int main() {
	int i,j,k,cs= 0,ts;
	for ( scanf("%d",&ts); ts--; ) {
		for ( vec.clear(), scanf("%d",&n), k= 0; k < n-1; ++k )
			scanf("%d %d",&i,&j), --i, --j, vec.push_back({i,j});
		for ( i= 0; i < n; adj[i++].clear() ) ;
		for ( k= 0; k < n-1; ++k ) {
			i= vec[k].first, j= vec[k].second;
			adj[i].push_back(j);
			adj[j].push_back(i);
		}
		int ans= -oo;
		for ( i= 0; i < n; ++i ) {
			++yes, p[i]= -1, dfs(i);
			ans= max(ans,dp(i));
		}
		printf("Case #%d: %d\n",++cs,n-ans);
	}
	return 0;
}

