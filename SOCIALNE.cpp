/*
 * SOCIALNE
 * TOPIC: bitmasks
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
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
using namespace std;
#define N 0x40
typedef unsigned long long u64;

int n,possible_friends[N];
char g[N][N],which[1<<21],bts[1<<21];
vector<int> adj[N];
u64 u[N],v[N];

int who( u64 u ) {
	assert( !(u&(u-1)) );
	if ( u >= BIT(60) ) return 60+which[u>>60];
	if ( u >= BIT(40) ) return 40+which[u>>40];
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

struct comparator {
	bool operator () ( const int &a, const int &b ) {
		if ( possible_friends[a] == possible_friends[b] )
			return a<b;
		return possible_friends[a] > possible_friends[b];
	}
};

int how_many( u64 u ) {
	return bts[u&MASK(20)]+bts[(u>>20)&MASK(20)]+bts[(u>>40)&MASK(20)]+bts[u>>60];
}

int main() {
	int i,j,k,ts;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( i = 0; i < (1<<21); ++i )
		bts[i] = bts[i>>1]+(i&1);
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%s",g[0]), n = strlen(g[0]);
		for ( i = 1; i < n; ++i ) scanf("%s",g[i]);
		memset(possible_friends,-1,sizeof possible_friends);
		for ( i = 0; i < n; ++i )
			for ( u[i]=v[i]=0, j = 0; j < n; ++j )
				if ( g[i][j] == 'Y' )
					u[i]|=BIT(j);
		set<int,comparator> s;
		for ( s.clear(), i = 0; i < n; ++i )
			for ( u64 w = (~u[i])&MASK(n); w && (j=who(L(w)))>=0; w &= ~L(w) )
				if ( u[i]&u[j] )
					v[i]|=BIT(j), v[j]|=BIT(i), v[i]&=~u[i], v[j]&=~u[j];
		for ( i = 0; i < n; ++i )
			if ( (v[i]&=~BIT(i)) >= 0 ) 
				possible_friends[i] = how_many(v[i]), s.insert(i);
		set<int> :: iterator it = s.begin();
		printf("%d %d\n",*it,possible_friends[*it]);
	}
	return 0;
}

