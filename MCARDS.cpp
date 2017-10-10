/*
 * MCARDS
 * TOPIC: DP, LIS
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
#define tol (1e-13)
#define N (1<<21)
#define oo (1<<30)
using namespace std;

int m,n,id[N],z[N];
vector<int> colors;
vector<pair<int,int> > vec;

struct comparator {
	bool operator () ( const pair<int,int> &a, const pair<int,int> &b ) {
		if ( id[a.first] == id[b.first] )
			return a.second < b.second;
		return id[a.first] < id[b.first];
	}
};

int LIS( const vector<pair<int,int> > &v ) {
	int i,j,k,n = v.size(),ans = +oo;
	comparator c;
	if ( n == 0 ) return 0;
	for ( z[0] = 1, i = 1; i < n; ++i ) 
		for ( z[i] = 1, j = 0; j < i; ++j )
			if ( c(v[j],v[i]) )
				z[i] = max(z[i],1+z[j]);
	for ( ans = -oo, i = 0; i < n; ++i )
		ans = max(ans,z[i]);
	return ans;
}

int main() {
	int i,j,k,ans;
	for ( ;2 == scanf("%d %d",&m,&n); ) {
		for ( colors.clear(), i = 1; i <= m; colors.push_back(i++) ) ;
		for ( ans = +oo, vec.clear(), i = 0; i < m*n; ++i )
			scanf("%d %d",&j,&k), vec.push_back(make_pair(j,k));
		do {
			for ( i = 0; i < m; ++i )
				id[colors[i]] = i;
			vector<pair<int,int> > v;
			for ( v.clear(), i = 0; i < m*n; v.push_back(vec[i++]) ) ;
			ans = min(ans,m*n-LIS(v));
		} while ( next_permutation(colors.begin(),colors.end()) ) ;
		printf("%d\n",ans);
	}
	return 0;
}

