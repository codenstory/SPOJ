/*
 * RATING
 * TOPIC: sorting, BIT
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <map>
#define N (1<<21)
#define L(k) ((k)&((~(k))+1ULL))
using namespace std;
typedef long long i64;

int n,K,tr[N],ans[N];
map<pair<int,int>,int> cnt;

void update( unsigned int i, int dt ) {
	for ( ;i < (1<<K); tr[i] += dt, i += L(i) ) ;
}

int prefix( unsigned int i ) {
	int ans = 0;
	for ( ;i; ans += tr[i], i &= ~L(i) ) ;
	return ans;
}

struct cell {
	pair<int,int> p;
	int idx;
	cell() {};
	cell( pair<int,int> p, int idx ) { this->p = p, this->idx = idx; };
};

struct comparator {
	bool operator () ( const cell &a, const cell &b ) {
		return a.p < b.p;
	}
};

cell c[N];

int main() {
	int i,j,k;
	for ( ;1 == scanf("%d",&n); ) {
		cnt.clear();
		pair<int,int> p;
		for ( i = 0; i < n; ++i ) {
			scanf("%d %d",&p.first,&p.second);
			c[i] = cell(p,i);
		}
		sort(c,c+n,comparator()), K = 19;
		for ( i = 0; i < (1<<K); tr[i++] = 0 ) ;
		for ( i = 0; i < n; ++i ) {
			update(c[i].p.second,1);
			p = c[i].p;
			if ( cnt.find(p) != cnt.end() )
				++cnt[p];
			else cnt[p] = 1;
			ans[c[i].idx] = max(0,prefix(c[i].p.second)-cnt[c[i].p]);
		}
		for ( i = 0; i < n; ++i )
			printf("%d\n",ans[i]);
	}
	return 0;
}

