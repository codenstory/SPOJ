/*
 * ISLHOP
 * TOPIC: MST, bfs
 * status: Accepted
 */
#include <bits/stdc++.h>
#define L(k) ((k)&((~(k))+1ULL))
#define oo (DBL_MAX-0x400)
#define N 0x40
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
typedef unsigned long long u64;
using namespace std;

int n,m[N],which[1<<20],parent[N];
pair<double,double> place[N];
u64 covered;
double d[N],g[N][N];
vector<int> adj[N];

double S( double x ) { return x*x; }

int who( u64 u ) {
	if ( u >= (1ULL<<40) )
		return 40+which[u>>40];
	if ( u >= (1ULL<<20) )
		return 20+which[u>>20];
	return which[u];
}

int main() {
	int i,j,k,x,y,cs= 0;
	u64 u,v;
	for ( i= 0; i < 20; which[1<<i]= i, ++i ) ;
	for ( ;1 == scanf("%d",&n) && n > 0; ) {
		for ( i= 0; i < n; scanf("%lf %lf %d",&place[i].first, &place[i].second, m+i), ++i ) ;
		for ( i= 0; i < n; ++i )
			for ( j= i+1; j < n; ++j ) 
				g[i][j]= g[j][i]= sqrt(S(place[i].first-place[j].first)+S(place[i].second-place[j].second));
		for ( i= 0; i < n; adj[i++].clear() ) ;
		set<pair<double,int>> s;
		for ( x= 0; x < n; d[x++]= +oo ) ;
		double total= 0, up= 0;
		for ( covered= 0, parent[0]= -1, s.insert({d[0]=0,0}); covered != MASK(n); ) {
			set<pair<double,int>>:: iterator it= s.begin();
			for ( x= it->second, covered|= BIT(x), s.erase(*it), u= ((~covered) & MASK(n)); u; u&= ~BIT(y)) {
				y= who(L(u));
				if ( d[y] > g[x][y] )
					s.insert({d[y]=g[x][y],y}), parent[y]= x;
			}
			if ( parent[x] != -1 )
				y= parent[x], adj[x].push_back(y), adj[y].push_back(x);
		}
		for ( i= 0; i < n; total+= m[i++] ) ;
		queue<int> q;
		bool seen[N];
		memset(seen,0,sizeof seen);
		for ( seen[0]= true, d[0]= 0, q.push(0); !q.empty(); ) {
			x= q.front(), q.pop();
			for ( i= 0; i < (int)adj[x].size(); ++i ) {
				y= adj[x][i];
				if ( !seen[y] )
					seen[y]= true, q.push(y), d[y]= max(d[x],g[x][y]);
			}
		}
		for ( up= 0, y= 0; y < n; ++y )
			up+= d[y]*m[y];
		printf("Island Group: %d Average %.2lf\n\n",++cs,up/total);
	}
	return 0;
}

