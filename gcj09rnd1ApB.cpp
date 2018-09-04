/*
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
#include <map>
#include <unordered_map>
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
#define Q (1 << 16)
#define DIM (0x20)
#define tol (1e-13)
#define vc(x,y) (0 <= (x) && (x) < m && 0 <= (y) && (y) < n)
#define oo (1<<30)
using namespace std;
typedef unsigned long long u64;
typedef long long i64;
#define W (10)
#define enc(t,x,y) ((t) | ((x)<<2) | ((y)<<7))

const int SE= 0, NE= 1, SW= 2, NW= 3;

struct traffic_light {
	int s,w,t0,i,j;
	traffic_light() {};
	inline int period() const {
		return s+w;
	}
	inline int next_available_ns_unit_segment_end( int t ) const {
		double tt= (t-t0+0.00)/(s+w);
		int k[3*W],i,j;
		for ( i= -W, j= 0; i <= W; ++i, ++j )
			k[j]= (int)(tt+i);
		for ( i= 0; i < j; ++i )
			if ( t0+k[i]*(s+w) <= t && t <= t0+k[i]*(s+w)+s-1 )
				return t+1;
		for ( i= 0; i < j; ++i )
			if ( t < t0+k[i]*(s+w) )
				return t0+k[i]*(s+w)+1;
		assert( 0 );
	}
	inline int next_available_ew_unit_segment_end( int t ) const {
		double tt= (t-t0+0.00)/(s+w);
		int k[3*W],i,j;
		for ( i= -W, j= 0; i <= W; ++i, ++j )
			k[j]= (int)(tt+i);
		for ( i= 0; i < j; ++i )
			if ( t0+k[i]*(s+w)+s <= t && t <= t0+(k[i]+1)*(s+w)-1 )
				return t+1;
		for ( i= 0; i < j; ++i )
			if ( t < t0+k[i]*(s+w)+s )
				return t0+k[i]*(s+w)+s+1;
		assert( 0 );
	}
	inline int next_instant( int cur_time, int direction ) const {
		if ( direction == 1 ) {
			// S--N direction
			return next_available_ns_unit_segment_end(cur_time);
		}
		else {
			// W--E direction
			return next_available_ew_unit_segment_end(cur_time);
		}
	}
};

traffic_light tl[DIM][DIM];
int d[Q],src,target,seen[Q],yes,m,n;
set<pair<int,int> > s;
vector<int> adj[4];
vector<pair<int,pair<int,int> > > lst[4];

int move( int i, int j, int k, int t, int nk, int &nt ) {
	nt= tl[i][j].next_instant(t,(nk&1)^(k&1));
	return enc(nk,i,j);
}

int main() {
	int i,j,k,ts,cs = 0,x,y,t,q,ans,ni,nj,nk,nt;

	adj[SE].push_back(SW);
	adj[SE].push_back(NE);
	adj[SW].push_back(SE);
	adj[SW].push_back(NW);
	adj[NE].push_back(SE);
	adj[NE].push_back(NW);
	adj[NW].push_back(SW);
	adj[NW].push_back(NE);

	lst[SE].push_back(make_pair(SW,make_pair(0,1)));
	lst[SE].push_back(make_pair(NE,make_pair(1,0)));
	lst[SW].push_back(make_pair(SE,make_pair(0,-1)));
	lst[SW].push_back(make_pair(NW,make_pair(1,0)));
	lst[NE].push_back(make_pair(SE,make_pair(-1,0)));
	lst[NE].push_back(make_pair(NW,make_pair(0,1)));
	lst[NW].push_back(make_pair(SW,make_pair(-1,0)));
	lst[NW].push_back(make_pair(NE,make_pair(0,-1)));

	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j ) {
				cin >> tl[i][j].s >> tl[i][j].w >> tl[i][j].t0;
				tl[i][j].i= i, tl[i][j].j= j;
			}
		for ( ans= +oo, s.clear(),seen[src= enc(SW,m-1,0)]= ++yes, target= enc(NE,0,n-1), s.insert(make_pair(d[src]=0,src)); !s.empty(); ) {
			set<pair<int,int>> :: iterator it= s.begin();
			s.erase(*it);
			t= d[x= it->second];
			if ( x == target ) 
				ans= min(ans,d[x]);
			if ( ans <= d[x] ) continue ;
			i= ((x>>2)&0x1f), j= ((x>>7)&0x1f), k= (x&3);
			assert( vc(i,j) );
			for ( int l= 0; l < (int)adj[k].size(); ++l ) {
				y= move(i,j,k,t,adj[k][l],nt);
				if ( seen[y] != yes || d[y] > nt )
					seen[y]= yes, s.insert(make_pair(d[y]= nt,y));
			}
			for ( int l= 0; l < (int)lst[k].size(); ++l ) {
				pair<int,pair<int,int> > pr= lst[k][l];
				ni= i+pr.second.first, nj= j+pr.second.second, nk= pr.first;
				if ( vc(ni,nj) && (seen[y= enc(nk,ni,nj)] != yes || d[y] > t+2) )
					seen[y]= yes, s.insert(make_pair(d[y]= t+2,y));
			}
		}
		printf("Case #%d: %d\n",++cs,ans);
	}
	return 0;
}

