/*
 * ULM09
 * TOPCI: mst, Prim's
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
#include <set>
#define tol (1e-13)
#define N 200100
using namespace std;
typedef long long i64;
#define oo (1LL<<60)

int m,n;
vector<pair<int,int> > adj[N];
i64 d[N],total,ans;
bool in_tree[N];

int main() {
	int i,j,k,x,y;
	for ( ;2 == scanf("%d %d",&n,&m) && (m||n); printf("%lld\n",total-ans) ) {
		for ( i = 0; i < n; in_tree[i] = false, d[i] = +oo, adj[i++].clear() ) ;
		for ( total = 0; m-- && 3 == scanf("%d %d %d",&i,&j,&k); adj[i].push_back(make_pair(j,k)), adj[j].push_back(make_pair(i,k)), total += k ) ;
		set<pair<i64,int> > s;
		for ( s.insert(make_pair(d[0]=0,0)); !s.empty(); ) {
			set<pair<i64,int> > :: iterator it = s.begin();
			for ( s.erase(*it), in_tree[x = it->second] = true, i = 0; i < (int)adj[x].size(); ++i ) 
				if ( !in_tree[y=adj[x][i].first] && d[y] > adj[x][i].second )
					s.insert(make_pair(d[y]=adj[x][i].second,y));
		}
		for ( ans = 0, i = 0; i < n; ans += d[i++] ) ;
	}
	return 0;
}

