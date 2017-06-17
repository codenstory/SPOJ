/*
 * LOSTNSURVIVED
 * TOPIC: dsu, multiset, STL
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol (1e-13)
#define N (1<<17)
using namespace std;

int n,qr,rank[N],p[N],card[N],largest,smallest;
multiset<int> s;

int find( int x ) { return p[x]=(x==p[x]?x:find(p[x])); }
void link( int x, int y ) {
	if ( rank[x] > rank[y] ) {
		p[y] = x;
		set<int> :: iterator it;
		it = s.find(card[x]); s.erase(it);
		it = s.find(card[y]); s.erase(it);
		s.insert(card[x] += card[y]);
	}
	else {
		p[x] = y;
		if ( rank[x] == rank[y] ) 
			++rank[y];
		set<int> :: iterator it;
		it = s.find(card[x]); s.erase(it);
		it = s.find(card[y]); s.erase(it);
		s.insert(card[y] += card[x]);
	}
}
void merge( int x, int y ) {
	link(find(x),find(y));
}
void make_set( int x ) { rank[p[x]=x] = 0, s.insert(card[x] = 1); }

int main() {
	int i,j,k,ans;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( ans = 0, s.clear(), i = 0; i < n; make_set(i++) ) ;
		for ( ;qr-- && 2 == scanf("%d %d",&i,&j); ) {
			if ( find(--i) == find(--j) ) {
				printf("%d\n",ans);
				continue ;
			}
			merge(i,j);
			ans = *(s.rbegin())-*(s.begin());
			printf("%d\n",ans);
		}
	}
	return 0;
}

