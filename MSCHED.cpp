/*
 * MSCHED
 * TOPIC: scheduling, priority queue
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
#include <queue>
#define N (1<<14)
typedef long long i64;
using namespace std;

int d[N],g[N],n,dig[256],id[N];

int num() {
	int ch, n = 0;
	for ( ;(ch = getchar_unlocked()) != EOF && !dig[ch]; ) ;
	for ( n = ch-'0'; (ch = getchar_unlocked()) != EOF && dig[ch]; n = 10*n+ch-'0' ) ;
	return n;
}

struct comparator {
	bool operator() ( const int &a, const int &b ) {
		if ( d[a] == d[b] )
			return g[a]>g[b];
		return d[a]>d[b];
	}
};

struct anticomparator {
	bool operator () ( const int &a, const int &b ) {
		return g[a] < g[b];
	}
};

priority_queue<int,vector<int>,anticomparator> pq;

int main() {
	int i,j,k,curtime;
	i64 ans;
	for ( i = '0'; i <= '9'; dig[i++] = 1 ) ;
	for ( ;(n = num()); ) {
		for ( curtime = -1, ans = 0, i = 0; i < n; ++i )
			g[i]=num(), d[id[i]=i] = num(), curtime=max(curtime,d[i]);
		sort(id,id+n,comparator());
		//for ( i = 0; i < n; ++i )
		//	printf("%d %d\n",d[id[i]],g[id[i]]);
		for ( ans = 0, i = 0; i < n && d[id[i]] == curtime; pq.push(id[i++]) ) ;
		for ( ;curtime; ) {
			if ( !pq.empty() ) {
				ans += g[j=pq.top()], pq.pop();
				//printf("[t = %d], cow #%d, milk = %d\n",curtime,j,g[j]);
			}
			for ( --curtime; i < n && d[id[i]] == curtime; pq.push(id[i++]) ) ;
		}
		printf("%lld\n",ans);
		return 0;
	}
	return 0;
}

