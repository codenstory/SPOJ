/*
 * KOICOST
 * TOPIC: dsu
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<21)
#define MOD (1000000000ULL)
using namespace std;

int m,n,card[N],_rank[N],p[N];

struct edge {
	int x,y,w;
	edge( int x, int y, int w ) : x(x), y(y), w(w) {};
	edge() {};
	bool operator < ( const edge &other ) const {
		return w > other.w;
	}
};

set<edge> s;

void make_set( int x ) { _rank[p[x]= x]= 0, card[x]= 1; }

void link( int x, int y ) {
	if ( _rank[x] > _rank[y] )
		p[y]= x, card[x]+= card[y];
	else {
		card[y]+= card[x], p[x]= y;
		if ( _rank[x] == _rank[y] )
			++_rank[y];
	}
}

int find( int x ) { return p[x]= (p[x]==x?x:find(p[x])); }

void merge( int x, int y ) { link(find(x),find(y)); }

unsigned long long G( unsigned long long k ) {
	return (k*(k-1)/2)%MOD;
}

int main() {
	int i,j,k;
	for (;2 == scanf("%d %d",&n,&m);) {
		for ( s.clear(), i= 0; i < m; ++i ) {
			edge e;
			cin >> e.x >> e.y >> e.w;
			--e.x, --e.y;
			s.insert(e);
		}
		for ( i= 0; i < n; make_set(i++) ) ;
		unsigned long long ans= 0ULL, num_pairs= 0;
		for ( set<edge>:: iterator it= s.begin(); it != s.end(); ++it ) {
			int x= it->x, y= it->y;
			unsigned long long w= it->w;
			if ( find(x) != find(y) ) {
				num_pairs+= MOD, num_pairs+= MOD;
				num_pairs-= G(card[find(x)]), num_pairs-= G(card[find(y)]);
				merge(x,y);
				num_pairs+= G(card[find(x)]), num_pairs%= MOD;
				ans+= (num_pairs*w)%MOD, ans%= MOD;
			}
			else {
				ans+= (num_pairs*w)%MOD, ans%= MOD;
			}
		}
		cout << (ans%MOD);
		break ;
	}
	return 0;
}

