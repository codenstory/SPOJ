/*
 * ACQUIRE: Land Acquisition
 * TOPIC: convex hull trick, dp
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N (1<<16)
#define h first
#define w second
#include <queue>
#include <stack>
#define oo (1LL<<60)
typedef long long i64;
typedef unsigned long long u64;
using namespace std;

int n,m;
u64 cost[N];
pair<u64,u64> v[N];
vector<pair<u64,u64> > vec;
stack<pair<u64,u64> > st;

typedef struct line {
	u64 m,b;
	line() {};
	line( u64 m, u64 b ) { this->m = m, this->b = b; };
} line;

double get_intersection( const line &p, const line &q ) {
	return (((i64)p.b)-((i64)q.b)+0.00)/(((i64)q.m)-((i64)p.m));
}

line e[N];
pair<double,double> t[N];

int main() {
	int i,j,k,idx,good,bad,mid;
	for ( ;1 == scanf("%d",&n); printf("%llu\n",cost[n]) ) {
		for ( i = 0; i < n; scanf("%llu %llu",&v[i].h,&v[i].w), ++i ) ;
		for ( sort(v,v+n), vec.clear(), i = 0; i < n; i = j, st.push(v[idx]) ) {
			for (  idx = i, j = i+1; j < n && v[j].h == v[i].h; ++j )
				if ( v[idx].w < v[j].w && (idx = j) >= 0 ) ;
			for (;!st.empty() && st.top().w <= v[idx].w; st.pop() ) ;
		}
		for ( ;!st.empty(); vec.push_back(st.top()), st.pop() );
		reverse(vec.begin(),vec.end());
		for ( i = 0; i+1 < (int)vec.size(); ++i ) {
			assert( vec[i].h < vec[i+1].h );
			assert( vec[i].w > vec[i+1].w );
		}
		for ( j = 1, i = 0; i < (int)vec.size(); v[j++] = vec[i++] ) ;
		for ( n = j-1, i = 1; i <= n; cost[i++] = +oo ) ;
		for ( m = 0, e[m] = line(v[1].w,cost[0]=0), t[m++] = make_pair(-oo,+oo), i = 1; i <= n; ++i ) {
			assert( t[good=0].first <= v[i].h );
			if ( t[bad = m-1].first <= v[i].h ) 
				good = m-1;
			else for (;good+1<bad;t[mid=(good+bad)/2].first<=v[i].h?(good=mid):(bad=mid));
			assert( good <= m-1 );
			assert( t[good].first <= v[i].h && v[i].h <= t[good].second );
			cost[i]=e[good].b+e[good].m*v[i].h;
			if ( i < n ) {
				line L(v[i+1].w,cost[i]);
				for (;m >= 2 && get_intersection(e[m-2],L) >= get_intersection(e[m-1],L); --m ) ;
				t[m-1] = make_pair(t[m-1].first,get_intersection(e[m-1],e[m]=L));
				t[m] = make_pair(t[m-1].second,+oo), ++m;
			}
		}
	}
	return 0;
}

