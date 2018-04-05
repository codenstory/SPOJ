/*
 */
#include <bits/stdc++.h>
#define N 0x100
#define oo (1<<29)
typedef unsigned long long u64;
using namespace std;
enum { S, T };

int L,n,mate[2][N],seen[N],yes;
u64 req[N],outlet[N];
unordered_map<u64,int> mp;
queue<int> q;
vector<int> adj[N];

int popcount( u64 u ) {
	int ans= 0;
	for ( ;u; u >>= 1 )
		ans+= (u&1);
	return ans;
}

int alt_path( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x] = yes, i= 0; i < adj[x].size(); ++i )
		if ( mate[T][y= adj[x][i]] == -1 || alt_path(mate[T][y]) ) {
			mate[S][x]= y;
			mate[T][y]= x;
			return 1;
		}
	return 0;
}

int bpm( u64 u ) {
	int x,y,i,j,k;
	bool flag;
	for (;!q.empty(); q.pop() ) ;
	for ( x= 0; x < n; q.push(x++) ) ;
	for ( x= 0; x < n; ++x )
		mate[S][x]= mate[T][x]= -1;
	for ( x= 0; x < n; ++x )
		for ( adj[x].clear(), y= 0; y < n; ++y )
			if ( (req[x]^outlet[y]) == u )
				adj[x].push_back(y);
	do {
		for ( k= q.size(), flag= false, ++yes; k--; ) {
			x= q.front(), q.pop();
			if ( alt_path(x) ) flag= true;
			else q.push(x);
		}
	} while ( flag );
	return n-q.size();
}

int main() {
	int i,j,k,ts,cs= 0;
	char tmp[0x400];
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&L);
		for ( i= 0; i < n; ++i ) 
			for ( scanf("%s",tmp), outlet[i]= 0, j= 0; j < L; ++j )
				if ( tmp[j] == '1' )
					outlet[i]|= (1ULL<<j);
		for ( i= 0; i < n; ++i ) 
			for ( scanf("%s",tmp), req[i]= 0, j= 0; j < L; ++j )
				if ( tmp[j] == '1' )
					req[i]|= (1ULL<<j);
		for ( mp.clear(), i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				u64 u= (req[i]^outlet[j]);
				if ( mp.count(u) )
					mp[u]= mp[u]+1;
				else mp[u]= 1;
			}
		int best= +oo;
		for ( auto pr: mp ) 
			if ( pr.second >= n && bpm(pr.first) == n )
				if ( popcount(pr.first) < best ) {
					best= popcount(pr.first);
				}
		printf("Case #%d: ",++cs);
		if ( best < +oo ) {
			printf("%d\n",best);
			continue ;
		}
		puts("NOT POSSIBLE");
	}
	return 0;
}

