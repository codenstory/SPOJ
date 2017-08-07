/*
 * HIGHWAYS
 * TOPIC: Dijkstra, trivial
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
using namespace std;
#define N (1<<17)
#define oo (1<<30)

int n,m,src,sink,d[N];
vector<pair<int,int> > adj[N];
set<pair<int,int> > s;

int main() {
	int i,j,k,ts,x,y;
	for ( scanf("%d",&ts); ts-- && 4 == scanf("%d %d %d %d",&n,&m,&src,&sink); ) {
		for ( --src, --sink, i = 0; i < n; adj[i].clear(), d[i++] = +oo ) ;
		for ( ;m-- && 3 == scanf("%d %d %d",&i,&j,&k); --i, --j, adj[i].push_back(make_pair(j,k)), adj[j].push_back(make_pair(i,k)) ) ;
		for ( s.insert(make_pair(d[src]=0,src)); !s.empty(); ) {
			set<pair<int,int> > :: iterator it = s.begin();
			for ( s.erase(*it), x = it->second, i = 0; i < (int)adj[x].size(); ++i ) {
				pair<int,int> r = adj[x][i];
				if ( d[y=r.first] > d[x]+r.second )
					s.insert(make_pair(d[y]=d[x]+r.second,y));
			}
		}
		if ( d[sink] < +oo )
			printf("%d\n",d[sink]);
		else puts("NONE");
	}
	return 0;
}

