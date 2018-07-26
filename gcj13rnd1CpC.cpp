/*
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<20)
#define Q (N<<2)
#define L(k) ((k)<<1)
#define R(k) (1|L(k))
#define oo (1<<30)
#define tol (1e-9)
using namespace std;

typedef struct attack {
	int id,day;
	int strength,qi,qj;
	bool operator < ( const attack &a ) const {
		if ( day == a.day ) 
			return strength < a.strength;
		return day < a.day;
	}
	attack() {};
	attack( int d, int s, int i, int j, int id ) : day(d), strength(s), qi(i), qj(j), id(id) { assert( qi <= qj ); }
} attack;

vector<attack> vec;
set<double> s,ns;
map<double,int> mp;
int m,n,mn[Q],mx[Q],V,h[Q];
bool is[N];

/**
 * post-condition: the fields of "v" are up-to-date
 */
void push_down( int v, int i, int j ) {
	assert( mx[v] >= mn[v] );
	if ( !h[v] || h[v] < mn[v] ) { h[v]= 0; return ; }
	if ( i == j ) {
		assert( mn[v] == mx[v] );
		assert( h[v] > mx[v] );
		mn[v]= mx[v]= h[v], h[v]= 0;
		return ;
	}
	assert( i < j );
	if ( h[v] >= mx[v] ) {
		mn[v]= mx[v]= h[L(v)]= h[R(v)]= h[v], h[v]= 0;
		return ;
	}
	if ( !h[L(v)] && mn[L(v)] < h[v] || h[L(v)] && h[L(v)] < h[v] )
		h[L(v)]= h[v];
	if ( !h[R(v)] && mn[R(v)] < h[v] || h[R(v)] && h[R(v)] < h[v] )
		h[R(v)]= h[v];
	mn[v]= h[v], h[v]= 0;
	assert( mx[v] >= mn[v] );
}

/**
 * pre-condition: the fields of L(v) and R(v) are up-to-date
 * post-condition: the fields of "v" are up-to-date
 */
void push_up( int v, int i, int j ) {
	if ( i < j ) {
		int old_mx= mx[v], old_mn= mn[v];
		mx[v]= max(mx[L(v)],mx[R(v)]);
		assert( mx[v] >= old_mx );
		mn[v]= min(mn[L(v)],mn[R(v)]);
		assert( mn[v] >= old_mn );
	}
}

/**
 * post-condition: the fields of "v" are up-to-date
 */
void update( int v, int i, int j, int qi, int qj, int newval ) {
	push_down(v,i,j);
	assert( i <= j );
	if ( qi > j || qj < i || mn[v] >= newval ) return ;
	assert( mn[v] < newval );
	if ( qi <= i && j <= qj ) {
		assert( !h[v] );
		h[v]= newval, push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(L(v),i,k,qi,qj,newval), update(R(v),k+1,j,qi,qj,newval);
	push_up(v,i,j);
}

int min_query( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	assert( i <= j );
	if ( qi > j || qj < i )
		return +oo;
	assert( !h[v] );
	if ( qi <= i && j <= qj ) 
		return mn[v];
	int k= (i+j)>>1;
	int ans= min(min_query(L(v),i,k,qi,qj),min_query(R(v),k+1,j,qi,qj));
	push_up(v,i,j);
	return ans;
}

int main() {
	int i,j,k,cs= 0,ts,ans,t;
	for ( cin >> ts; ts-- && (cin >> n); ) {
		for ( m= 0, vec.clear(), i= 0; i < n; ++i ) {
			int di,ni,wi,ei,si,delta_di,delta_pi,delta_si;
			cin >> di >> ni >> wi >> ei >> si >> delta_di >> delta_pi >> delta_si; m+= ni;
			for ( k= 0; k < ni; ++k ) {
				vec.push_back(attack(di,si,wi,ei,i));
				di+= delta_di, si+= delta_si, wi+= delta_pi, ei+= delta_pi;
			}
		}

		/**
		 * coordinate compression
		 */
		for ( k= 0, mp.clear(), ns.clear(), s.clear(), i= 0; i < m; s.insert(vec[i].qi), s.insert(vec[i].qj), ++i ) ;
		for ( set<double> :: iterator pit, it= s.begin(); it != s.end(); pit= it, ++it ) 
			if ( it != s.begin() )
				ns.insert((*it+*pit)/2.00);
		for ( set<double> :: iterator it= ns.begin(); it != ns.end(); mp[*it++]= k++ ) ;
		for ( k= 0; k < m; ++k ) {
			// left end is mapped to the successor
			set<double> :: iterator it= ns.lower_bound(vec[k].qi);
			assert( it != ns.end() );
			assert( vec[k].qi <= *it );
			vec[k].qi= mp[*it];

			// right end is mapped to predecessor
			it= ns.lower_bound(vec[k].qj);
			assert( it == ns.end() || vec[k].qj <= *it );
			if ( it != ns.end() && fabs(vec[k].qj-*it) < tol )
				vec[k].qj= mp[*it];
			else {
				assert( it == ns.end() || vec[k].qj < *it );
				assert( it != ns.begin() );
				vec[k].qj= mp[*--it];
			}
		}
		/**
		 * coordinate compression ended
		 */

		V= mp.size(), sort(vec.begin(),vec.end());
		memset(mn,0,sizeof mn), memset(mx,0,sizeof mx);
		memset(h,0,sizeof h);
		memset(is,0,sizeof is);
		for ( ans= 0, i= 0; i < m; i= j ) {
			for ( j= i+1; j < m && vec[j].day == vec[i].day; ++j ) ;
			for ( k= i; k <= j-1; ++k ) {
				ans+= (t=min_query(1,0,V-1,vec[k].qi,vec[k].qj))<vec[k].strength?(is[k]=true,1):0;
				//if ( is[k] )
				//	printf("[%d:%d] Successful attack by tribe %d, on day %d, at height %d, and min query returned %d\n",vec[k].qi,vec[k].qj,vec[k].id+1,vec[k].day,vec[k].strength,t);
			}
			for ( k= i; k <= j-1; ++k )
				if ( is[k] ) {
					//if ( min_query(1,0,V-1,vec[k].qi,vec[k].qj) < vec[k].strength )
					update(1,0,V-1,vec[k].qi,vec[k].qj,vec[k].strength);
					//printf("On day %d, attack by %d succeeds at height %d\n",vec[k].day,vec[k].id+1,vec[k].strength);
					assert( min_query(1,0,V-1,vec[k].qi,vec[k].qj) >= vec[k].strength );
					//printf("min_query(%d,%d) = %d\n",vec[k].qi,vec[k].qj,min_query(1,0,V-1,vec[k].qi,vec[k].qj));
				}
			//printf("HERE %d and %d\n",min_query(1,0,V-1,4,4),min_query(1,0,V-1,0,4));
		}
		printf("Case #%d: %d\n",++cs,ans);
	}
	return 0;
}

