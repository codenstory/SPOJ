/*
 */
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#define N 0x80
using namespace std;

int m,n,H,V,portion,
	r[N][N],c[N][N],
	rsum[N],csum[N],
	z[N][N];
char g[N][N];
vector<int> first,last,fc,lc;

int f( int i, int j, int k, int t ) {
	return z[k][t]-z[i-1][t]-z[k][j-1]+z[i-1][j-1];
}

int rows( int size ) {
	int i,j,k,sum,covered= 0;
	first.clear(), last.clear();
	for ( i= 1; i <= m && first.size() < H+1; i= j ) {
		first.push_back(i);
		for ( sum= 0, j= i; sum < size && j <= m; sum+= r[j++][n] ) ;
		if ( sum != size ) {
			return 0;
		}
		++covered;
		last.push_back(j-1);
	}
	//assert( covered == H+1 );
	//assert( first.size() == H+1 );
	return 1;
}

int cols( int size ) {
	int i,j,k,t,sum,covered= 0,prev= 0;
	fc.clear(), lc.clear();
	for ( i= 1; i <= n && fc.size() < V+1; i= j, prev= i-1 ) {
		fc.push_back(i);
		for ( sum= 0, j= i; sum < size && j <= n; sum+= c[j++][m] ) ;
		if ( sum != size )
			return 0;
		for ( t= 0; t <= H; ++t )
			if ( f(first[t],i,last[t],j-1) != portion ) {
				return 0;
			}
		lc.push_back(j-1);
		++covered;
	}
	/*
	for (int l =0 ;l <= V; ++l )
		printf("%d %d\n",fc[l],lc[l]);
		*/
	return 1;
}

int main() {
	int i,j,k,cs= 0,ts,chips;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d %d",&m,&n,&H,&V);
		memset(r,0,sizeof r);
		memset(c,0,sizeof c);
		memset(z,0,sizeof z);
		memset(rsum,0,sizeof rsum);
		memset(csum,0,sizeof csum);
		for ( i = 1; i <= m; ++i )
			scanf("%s",g[i]+1);
		for ( chips = 0, i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j )
				if ( g[i][j] == '@' )
					++chips;
		printf("Case #%d: ",++cs);
		if ( chips % ((H+1)*(V+1)) ) {
			puts("IMPOSSIBLE");
			continue ;
		}
		portion= chips / ((H+1)*(V+1));
		for ( i= 1; i <= m; ++i )
			for ( j= 1; j <= n; ++j )
				r[i][j]= r[i][j-1]+(g[i][j]=='@'?1:0);
		for ( j= 1; j <= n; ++j )
			for ( i= 1; i <= m; ++i )
				c[j][i]= c[j][i-1]+(g[i][j]=='@'?1:0);
		for ( i= 1; i <= m; ++i )
			for ( j= 1; j <= n; ++j )
				z[i][j] = z[i-1][j]+z[i][j-1]-z[i-1][j-1]+(g[i][j]=='@'?1:0);
		for ( i= 1; i <= m; ++i )
			rsum[i]= rsum[i-1]+r[i][n];
		for ( j= 1; j <= n; ++j )
			csum[j]= csum[j-1]+c[j][m];
		if ( portion == 0 ) {
			puts("POSSIBLE");
			continue ;
		}
		if ( !rows(portion*(V+1)) ) {
			puts("IMPOSSIBLE");
			continue ;
		}
		puts(cols(portion*(H+1))?"POSSIBLE":"IMPOSSIBLE");
	}
	return 0;
}

