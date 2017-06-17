/*
 * UCI2009D: Octaves
 * TOPIC: backtracking
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#define  N 8
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)& ((~(k))+1))
typedef unsigned long long u64;
using namespace std;
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)

int n;
char g[N][N+1],which[BIT(21)],bts[BIT(21)];
set<u64> octaves;
u64 adj[N*N];

inline int bitcount( u64 u ) {
	return bts[u>>60]+bts[(u>>40)&MASK(20)]+bts[(u>>20)&MASK(20)]+bts[u&MASK(20)];
}

int who( u64 u ) {
	if ( u < BIT(20) )
		return which[u];
	if ( u < BIT(40) )
		return 20+which[u>>20];
	if ( u < BIT(60) )
		return 40+which[u>>40];
	return 60+which[u>>60];
}

void f( int cur, u64 oct ) {
	int i;
	if ( bitcount(oct) == N ) 
		octaves.insert(oct);
	else for ( u64 u = ((~oct)&adj[cur]); u && (i = who(L(u))) >= 0; f(i,oct|L(u)), u &= ~L(u) );
}

int main() {
	int i,j,k,ts,ni,nj,
		di[] = {-1,0,1,0},
		dj[] = {0,1,0,-1};
	for ( i = 0; i < BIT(21); bts[i] = bts[i>>1]+(i&1), ++i ) ;
	for ( i = 0; i < 21; ++i )
		which[BIT(i)] = i;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%s",g[i++]) ) ;
		for ( memset(adj,0,sizeof adj), i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == 'X' )
					for ( k = 0; k < 4; ++k )
						if ( vc(ni=i+di[k],nj=j+dj[k]) && g[ni][nj]=='X' ) 
							adj[i*n+j] |= BIT(ni*n+nj);
		for ( octaves.clear(), i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				f(i*n+j,0);
		printf("%u\n",octaves.size());
	}
	return 0;
}

