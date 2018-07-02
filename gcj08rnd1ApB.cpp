/*
 * Milkshakes
 * TOPIC: greedy
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (0x800)
#include <vector>
using namespace std;

int n,m,types[N],which[N];
vector<pair<int,bool>> vec[N];
bool malted[N],satisfied[N],has_favourite_malted[N];

bool condition( int i ) {
	if ( has_favourite_malted[i] )
		return false ;
	for ( int k= 0; k < (int)vec[i].size(); ++k )
		if ( !malted[vec[i][k].first] )
			return false ;
	return true ;
}

int main() {
	int i,j,k,cs= 0,ts,t;
	for ( scanf("%d",&ts); ts-- && scanf("%d %d",&n,&m); ) {
		memset(has_favourite_malted,0,sizeof has_favourite_malted);
		for ( i= 0; i < m; ++i ) {
			scanf("%d",&types[i]), vec[i].clear();
			for ( j= 0; j < types[i]; ++j ) {
				scanf("%d %d",&k,&t), vec[i].push_back({k-1,t?true:false});
				if ( t ) has_favourite_malted[i]= true, which[i]= k-1;
			}
		}
		printf("Case #%d:",++cs);
		memset(malted,0,sizeof malted);
		memset(satisfied,0,sizeof satisfied);
		bool flag;
		do {
			for ( flag= false, i= 0; i < m; ++i ) {
				if ( satisfied[i] ) continue ;
				if ( condition(i) ) 
					goto IMPOSSIBLE;
				bool all_malted= true ;
				for ( k= 0; k < (int)vec[i].size() && all_malted; ++k )
					if ( !vec[i][k].second && !malted[vec[i][k].first] )
						all_malted= false;
				if ( all_malted && has_favourite_malted[i] )
					malted[which[i]]= true, satisfied[i]= true, flag= true ;
			}
		} while ( flag );
		for ( i= 0; i < n; ++i )
			printf(" %d",malted[i]?1:0);
		putchar('\n');
		continue ;
IMPOSSIBLE:
		puts(" IMPOSSIBLE");
	}
	return 0;
}

