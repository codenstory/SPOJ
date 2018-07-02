/*
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <map>
#define N 0x80
#define M 0x400
typedef long long i64;
using namespace std;
#define oo (1LL<<45)
#define FG (ptr= fgets(buff,sizeof buff,stdin))

string s[N];
map<string,int> mp;
int n,b[M];
i64 z[N][M];
queue<pair<int,int>> q;
char buff[1<<21],*ptr;

int main() {
	int i,j,k,ts,cs= 0,qr;
	i64 ans;
	for ( sscanf(FG,"%d",&ts); ts--; ) {
		for ( sscanf(FG,"%d",&n), mp.clear(), i= 0; i < n; ++i ) 
			mp[string(FG)]= (i+1);
		for ( sscanf(FG,"%d",&qr), i= 0; i < qr; ++i ) {
			string tmp(FG);
			assert( mp.count(tmp) );
			b[i+1]= mp[tmp];
			assert( 1 <= b[i+1] && b[i+1] <= n );
		}
		for ( i= 0; i <= n; ++i )
			for ( j= 0; j <= qr; ++j )
				z[i][j]= +oo;
		for ( i= 1; i <= n; ++i )
			if ( i != b[1] )
				z[i][1]= 0, q.push(make_pair(i,1));
		if ( qr == 0 ) {
			printf("Case #%d: 0\n",++cs);
			continue ;
		}
		for ( ans= +oo; !q.empty(); ) {
			auto pr= q.front(); q.pop();
			i= pr.first, j= pr.second;
			if ( j == qr ) {
				ans= min(ans,z[i][j]);
				continue ;
			}
			assert( j <= qr-1 );
			if ( ans <= z[i][j] ) continue ;
			if ( i != b[j+1] && z[i][j+1] > z[i][j] )
				z[i][j+1]= z[i][j], q.push(make_pair(i,j+1));
			//if ( i == b[j+1] ) {
				for ( k= 1; k <= n; ++k )
					if ( k != b[j+1] && z[k][j+1] > z[i][j]+1 )
						z[k][j+1]= z[i][j]+1, q.push(make_pair(k,j+1));
			//}
		}
		printf("Case #%d: %lld\n",++cs,ans);
	}
	return 0;
}

