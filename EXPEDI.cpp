/*
 * EXPEDI
 * TOPIC: priority queue, greedy
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
#define tol (1e-13)
#define N (1<<14)
typedef long long i64;
using namespace std;

int n,id[N];
i64 fuel[N],dist[N],L,P;

struct comparator {
	bool operator () ( const int &a, const int &b ) {
		if ( fuel[a] == fuel[b] )
			return false;
		return fuel[a]<fuel[b];
	}
};

struct comp {
	bool operator () ( const int &a, const int &b ) {
		if ( dist[a] == dist[b] )
			return false;
		return dist[a] < dist[b];
	}
};

priority_queue<int,vector<int>,comparator> pq;
int seen[N],yes,stopped_by[N];

int main() {
	int i,j,k,ts,ans;
	i64 cur;
	bool ok;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n) && ++yes; ) {
		for ( i = 0; i < n; ++i )
			scanf("%lld %lld",dist+i,fuel+i), id[i] = i;
		scanf("%lld %lld",&L,&P);
		for ( i = 0; i < n; dist[i] = L-dist[i], ++i ) ;
		if ( P >= L ) {
			puts("0");
			continue ;
		}
		for ( sort(id,id+n,comp()); !pq.empty(); pq.pop() ) ;
		for ( i = 0; i < n-1; ++i )
			assert( dist[id[i]] <= dist[id[i+1]] );
		for ( ok=true, ans=0, cur=0, i = 0; i < n; ++i ) {
			assert( cur <= L );
			if ( L-cur <= P ) {
				//printf("Reachable from %lld with %lld litres\n",cur,P);
				break ;
			}
			assert( dist[id[i]] >= cur );
			if ( dist[id[i]]-cur <= P ) {
				P -= (dist[id[i]]-cur);
				cur = dist[id[i]];
				assert( seen[id[i]] != yes );
				pq.push(id[i]);
				seen[id[i]] = yes;
				continue ;
			}
			for ( ;!pq.empty() && P < dist[id[i]]-cur && !(P >= L-cur); ) {
				//printf("Added %lld of fuel to %lld\n",fuel[j=pq.top()],P);
				++ans, P += fuel[j=pq.top()], pq.pop();
				assert( stopped_by[j] != yes );
				stopped_by[j] = yes;
			}
			if ( P >= L-cur ) {
				//printf("Reachable from %lld with %lld litres\n",cur,P);
				break ;
			}
			if ( P < dist[id[i]]-cur ) {
				ok = false;
				break ;
			}
			P -= (dist[id[i]]-cur), cur = dist[id[i]], pq.push(id[i]);
			assert( seen[id[i]] != yes );
			seen[id[i]] = yes;
		}
		if ( !ok ) {
			puts("-1");
			continue ;
		}
		if ( cur < L ) {
			for ( ;!pq.empty() && P < L-cur; ) {
				//printf("Added %lld of fuel to %lld\n",fuel[j=pq.top()],P);
				++ans, P += fuel[j=pq.top()], pq.pop();
				assert( stopped_by[j] != yes );
				stopped_by[j] = yes;
			}
			if ( P < L-cur ) ok = false;
			else cur = L;
		}
		if ( cur < L ) ok = false;
		if ( !ok ) {
			puts("-1");
			continue ;
		}
		printf("%d\n",ans);
	}
	return 0;
}

