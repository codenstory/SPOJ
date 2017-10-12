/*
 * COT2
 * TOPIC: modified DFS order, Mo's Algorithm, LCA
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<16)
#define MAXE (N<<1)
#define MAXQ (1<<17)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define LEFT(u) ((u) & MASK(20))
#define RIGHT(u) ((u)>>20)
#define MAXK (20)
#define NONE (-1)
typedef long long i64;

int V,E,n,to[MAXE],last[N],next[MAXE],value[N],K,
	distinct[N],p[N],seen[N],yes,d[N],
	start[N],end[N],tick,sons[N],anc[N][MAXK],
	A[N<<1],BS,
	vc[N<<1],cc[N<<1];

i64 enc( i64 x, i64 y, i64 z ) {
	return x | (y<<20) | (z<<40);
}

int cmp( const void *a, const void *b ) {
	int *x = (int *)a,
		*y = (int *)b;
	if ( *x == *y )
		return 0;
	return *x<*y?-1:1;
}

int cmp_segments( const void *a, const void *b ) {
	i64 x = *(i64 *)a,
		y = *(i64 *)b;
	int lx = LEFT(x)/BS, ly = LEFT(y)/BS, rx = (RIGHT(x)&MASK(20)), ry = (RIGHT(y)&MASK(20));
	if ( lx == ly )
		return rx-ry;
	return lx<ly?-1:1;
}

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

void dfs( int x ) {
	int i,y,k;
	assert( seen[x] != yes );
	for ( sons[x] = 0, A[start[x] = ++tick] = x, seen[x] = yes, i = last[x]; i >= 0; i = next[i] )
		if ( seen[y = to[i]] != yes ) {
			p[y] = i, d[y] = d[x]+1, ++sons[x];
			for ( anc[y][0] = x, k = 1; anc[y][k-1] != NONE; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
			dfs(y);
		}
	A[end[x] = ++tick] = x;
}

int up( int x, unsigned int u ) {
	int k = 0;
	for ( ;u; u>>=1, ++k )
		if ( u&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( x == y )
		return x;
	for ( k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

typedef struct query {
	i64 s;
	int id,ans;
} query;

query q[MAXQ];

int cmp_by_segment( const void *a, const void *b ) {
	query *x = (query *)a,
		  *y = (query *)b;
	return cmp_segments(&(x->s),&(y->s));
}

int cmp_by_id( const void *a, const void *b ) {
	query *x = (query *)a,
		  *y = (query *)b;
	return cmp(&(x->id),&(y->id));
}

void check( int x, int *res ) {
	if ( vc[x] && !--cc[value[x]] ) --(*res);
	else if ( !vc[x] && ++cc[value[x]] == 1 ) ++(*res);
	vc[x] ^= 1;
}

int main() {
	int i,j,k,t,qr,low,high,mid,left,right,ll,rr,zz,
		lft,rgt,x,y;
	i64 segment;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( E = 0, i = 1; i <= n; scanf("%d",&value[i]), distinct[i] = value[i], last[i++] = -1 ) ;
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( i = 1; i <= n; ++i )
			for ( j = 0; j < K; anc[i][j++] = NONE ) ;
		qsort(distinct+1,n,sizeof *distinct,cmp);
		for ( i = 1; i <= n; ++i ) {
			if ( distinct[1] == value[i] ) {
				value[i] = 1;
				continue ;
			}
			assert( distinct[1] < value[i] );
			assert( distinct[n] >= value[i] );
			for ( low=1, high=n; low+1<high; distinct[mid=(low+high)>>1]<value[i]?(low=mid):(high=mid) ) ;
			assert( distinct[high] == value[i] );
			assert( distinct[low] < value[i] );
			value[i] = high;
		}
		for ( k = 0; k < n-1 && 2 == scanf("%d %d",&i,&j); add_arcs(i,j), ++k ) ;
		if ( !qr ) {
			continue ;
		}
		for ( ++yes, tick = 0, dfs(1), BS = 1; BS <= tick/BS; ++BS ) ;
		for ( k = 0; k < qr; ++k ) {
			scanf("%d %d",&i,&j);
			if ( start[i] > start[j] ) 
				t = i, i = j, j = t;
			t = lca(i,j);
			if ( t == i ) 
				segment = enc(start[i],start[j],0);
			else 
				segment = enc(end[i],start[j],start[t]);
			q[k].s = segment, q[k].id = k, q[k].ans = NONE;
		}
		qsort(q,qr,sizeof *q,cmp_by_segment);
		for ( i = 0; i <= tick; vc[i] = cc[i] = 0, ++i ) ;

		left = LEFT(q[0].s), right = (LEFT(q[0].s)&MASK(20))-1, k = 0;
		for ( i = 0; i < qr; ++i ) {
			ll = LEFT(q[i].s), rr = (RIGHT(q[i].s)&MASK(20)), zz = (q[i].s>>40);
			for ( ;left < ll; check(A[left++],&k) ) ;
			for ( ;left > ll; check(A[--left],&k) ) ;
			for ( ;right < rr; check(A[++right],&k) ) ;
			for ( ;right > rr; check(A[right--],&k) ) ;
			x = A[left], y = A[right];
			if ( zz ) check(A[zz],&k);
			q[i].ans = k;
			if ( zz ) check(A[zz],&k);
		}
		qsort(q,qr,sizeof *q,cmp_by_id);
		for ( i = 0; i < qr; printf("%d\n",q[i++].ans) ) ;
	}
	return 0;
}

