/*
 * DQUERY
 * TOPIC: Mo's Algorithm
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
#include <set>
#define MAXQ (1<<18)
#define N (1<<15)
using namespace std;

int which_block[N],a[N],answer[MAXQ],m,n,s[N],BS,c[N];

typedef struct query {
	int l, r, ans, id;
	bool operator < ( const query &rhs ) const {
		if ( which_block[l] == which_block[rhs.l] )
			return r < rhs.r;
		return which_block[l] < which_block[rhs.l];
	}
} query;

query q[MAXQ];

void add_left( int x, int &res ) {
	if ( ++c[a[x]] == 1 ) ++res;
}

void remove_left( int x, int &res ) {
	if ( !--c[a[x]] ) --res;
}

void add_right( int x, int &res ) {
	if ( ++c[a[x]] == 1 ) ++res;
}

void remove_right( int x, int &res ) {
	if ( !--c[a[x]] ) --res;
}

int main() {
	int i,j,k,left,right;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%d",&a[i]), s[i] = a[i], ++i ) ;
		sort(s,s+n), m = unique(s,s+n)-s;
		for ( i = 0; i < n; ++i )
			a[i] = lower_bound(s,s+m,a[i])-s;
		for ( BS = 1; BS < n/BS; ++BS ) ;
		for ( i = 0; i < n; ++i )
			which_block[i] = i/BS;
		for ( scanf("%d",&m), i = 0; i < m; ++i )
			scanf("%d %d",&q[i].l,&q[i].r), --q[i].l, --q[i].r, q[i].id = i;
		sort(q,q+m), memset(c,0,sizeof c);
		for ( left = q[0].l, right = q[0].l-1, k = 0, i = 0; i < m; answer[q[i++].id] = k ) {
			for ( ;left < q[i].l; remove_left(left++,k) ) ;
			for ( ;left > q[i].l; add_left(--left,k) ) ;
			for ( ;right > q[i].r; remove_right(right--,k) ) ;
			for ( ;right < q[i].r; add_right(++right,k) ) ;
		}
		for ( i = 0; i < m; printf("%d\n",answer[i++]) ) ;
	}
	return 0;
}

