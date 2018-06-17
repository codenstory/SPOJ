/*
 * BYTESE2
 * TOPIC: priority queue, greedy
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#define N (1<<21)
using namespace std;

typedef struct event {
	struct event *p;
	bool is_opening;
	int timestamp;
	bool operator < ( const event &other ) const { 
		if ( timestamp == other.timestamp ) {
			if ( is_opening == other.is_opening ) {
				if ( p == other.p )
					return false ;
				return p < other.p;
			}
			return is_opening;
		}
		return timestamp < other.timestamp;
	}
	bool operator == ( const event &other ) const {
		return timestamp == other.timestamp && is_opening == other.is_opening && p == other.p;
	}
	event() {};
	event( bool flag, int t ) : is_opening(flag), timestamp(t) { p= NULL; }
} event;

struct comp {
	bool operator() ( const event &a, const event &b ) {
		return b < a;
	}
};

event e[N];
priority_queue<event,vector<event>,comp> pq;
set<event> s;
int n,m;

int main() {
	int i,j,k,ts,ans;
	for ( scanf("%d",&ts); ts--; ) {
		for ( m= 0, s.clear(), scanf("%d",&n), i= 0; i < n; ++i ) {
			scanf("%d %d",&j,&k);
			e[m++]= event(true,j), e[m++]= event(false,k);
			e[m-2].p= &e[m-1], e[m-1].p= &e[m-2];
			pq.push(e[m-2]), pq.push(e[m-1]);
		}
		for ( ans= 0; !pq.empty(); ) {
			event entry= pq.top(); pq.pop();
			if ( entry.is_opening ) {
				s.insert(*(entry.p));
				continue ;
			}
			if ( s.size() > ans )
				ans= s.size();
			assert( s.find(entry) != s.end() ) ;
			s.erase(entry);
		}
		printf("%d\n",ans);
	}
	return 0;
}

