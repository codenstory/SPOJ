/*
 * IITWPC4I
 * TOPIC: Prim's Algorithm
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
#define N (1<<17)
using namespace std;
typedef long long i64;
#define oo (1LL<<60)

int n,m,is[N],yes;
vector<pair<int,int> > adj[N];
set<pair<i64,int> > s;
i64 d[N],ans;
bool in_tree[N],ok;

int main() {
	int i,j,k,ts,x,y;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( i = 0; i < n; scanf("%d",is+i), in_tree[i] = false, d[i] = +oo, adj[i++].clear() ) ;
		for ( ;m-- && 3==scanf("%d %d %d",&i,&j,&k); --i,--j,adj[i].push_back(make_pair(j,k)),adj[j].push_back(make_pair(i,k)) ) ;
		for ( i = 0; i < n; ++i )
			if ( is[i] )
				s.insert(make_pair(d[i]=0,i));
		for ( ;!s.empty(); ) {
			set<pair<i64,int> > :: iterator it = s.begin(); 
			for ( s.erase(*it), in_tree[x = it->second] = true, i = 0; i < (int)adj[x].size(); ++i ) 
				if ( !in_tree[y = adj[x][i].first] && d[y] > adj[x][i].second )
					s.insert(make_pair(d[y]=adj[x][i].second,y));
		}
		for ( ans = 0, ok = true, i = 0; i < n && ok; ++i )
			if ( d[i] == +oo ) ok = false ;
			else ans += d[i];
		if ( !ok ) {
			puts("impossible");
			continue ;
		}
		printf("%lld\n",ans);
	}
	return 0;
}

