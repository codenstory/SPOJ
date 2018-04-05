#include <bits/stdc++.h>
#define N 0x400
using namespace std;
enum { S, T };

map<string,int> mp[2];
vector<pair<int,int>> vec;
int n,mate[2][N],adj[N][N],deg[N],seen[N],yes,q[N],head,tail,cnt;

int alt_path( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x]= yes, i= 0; i < deg[x]; ++i ) {
		if ( mate[T][y= adj[x][i]] < 0 || alt_path(mate[T][y]) ) {
			mate[T][mate[S][x]= y]= x;
			return 1;
		}
	}
	return 0;
}

int bpm() {
	int i,j,k,x;
	memset(mate,-1,sizeof mate);
	for ( head= tail= 0, cnt= 0, x= 0; x < mp[S].size(); q[tail++]= x++, ++cnt ) ;
	bool flag;
	do {
		for ( flag= false, k= cnt, ++yes; k--; ) {
			x= q[head++], head &= (N-1), --cnt;
			assert( mate[S][x] < 0 );
			if ( alt_path(x) ) flag= true;
			else q[tail++]= x, ++cnt, tail &= (N-1);
		}
	} while ( flag );
	return mp[S].size()-cnt;
}

int main() {
	int i,j,k,t,ts,cs= 0;
	char ta[0x400],tb[0x400];
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n), mp[S].clear(), mp[T].clear(), vec.clear();
		for ( i= 0; i < n; ++i ) {
			scanf("%s %s",ta,tb);
			string sa= string(ta),
				   sb= string(tb);
			if ( !mp[S].count(sa) ) 
				k= mp[S].size(), mp[S][sa]= k;
			if ( !mp[T].count(sb) ) 
				k= mp[T].size(), mp[T][sb]= k;
			vec.push_back({mp[S][sa],mp[T][sb]});
		}
		memset(deg,0,sizeof deg);
		for ( i= 0; i < vec.size(); ++i ) 
			adj[vec[i].first][deg[vec[i].first]++]= vec[i].second;
		k= bpm();
		printf("Case #%d: %d\n",++cs,n-(mp[S].size()+mp[T].size()-k));
	}
	return 0;
}

