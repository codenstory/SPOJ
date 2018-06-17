/*
 * TEMPLEQ
 * TOPIC: range tree with updates
 * status: Accepted
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
#define N (100100)
#define Q (N<<2)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
#define oo (1<<30)

int nextint() {
	int ch,n= 0;
	static int dig[256]= {0};
	if ( !dig['0'] )
		for ( ch= '0'; ch <= '9'; dig[ch++]= 1 ) ;
	for ( ;(ch= getchar_unlocked()) != EOF && !dig[ch]; ) ;
	if ( ch == EOF )
		return +oo;
	for ( n= ch-'0'; (ch= getchar_unlocked()) != EOF && dig[ch]; n= 10*n+ch-'0' ) ;
	return n;
}

int n,leftmost[Q],
	h2q[N],q2h[N],height[N],
	mn[Q],mx[Q],h[Q];
pair<int,int> a[N*5];

inline void push_up( int v, int i, int j ) {
	if ( i < j ) {
		mx[v]= max(mx[L(v)],mx[R(v)]);
		mn[v]= min(mn[L(v)],mn[R(v)]);
	}
}

void build( int v, int i, int j ) {
	h[v]= 0;
	if ( i == j ) {
		mn[v]= mx[v]= height[i];
		leftmost[v]= i;
		return ;
	}
	int k= (i+j)>>1;
	build(L(v),i,k), build(R(v),k+1,j);
	leftmost[v]= leftmost[L(v)];
	push_up(v,i,j);
}

inline void push_down( int v, int i, int j ) {
	if ( !h[v] ) return ;
	mx[v]+= h[v], mn[v]+= h[v];
	if ( i < j ) 
		h[L(v)]+= h[v], h[R(v)]+= h[v];
	h[v]= 0;
}

int point_query( int v, int i, int j, int pos ) {
	push_down(v,i,j);
	if ( pos < i || pos > j ) 
		return -oo;
	if ( i == j || mn[v] == mx[v] )
		return mx[v];
	int k= (i+j)>>1;
	int res= max(point_query(L(v),i,k,pos),point_query(R(v),k+1,j,pos));
	push_up(v,i,j);
	return res;
}

int q_successor( int v, int i, int j, int value ) {
	push_down(v,i,j);
	if ( mx[v] < value )
		return n;
	if ( mn[v] >= value )
		return leftmost[v];
	if ( i == j ) 
		return i;
	int k= (i+j)>>1, res= min(q_successor(L(v),i,k,value),q_successor(R(v),k+1,j,value));
	push_up(v,i,j);
	return res;
}

/**
 * return the leftmost position "pos" in the array
 * such that actual size of the queue at this position is at least "value"
 */
int successor( int value ) {
	return q_successor(1,0,n-1,value);
}

void point_update( int v, int i, int j, int pos, int dt ) {
	push_down(v,i,j);
	if ( pos < i || pos > j )
		return ;
	if ( i == j ) {
		h[v]+= dt, push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	point_update(L(v),i,k,pos,dt), point_update(R(v),k+1,j,pos,dt);
	push_up(v,i,j);
}

void increment( int j ) {
	int i= q2h[j],nj,
		current_size= point_query(1,0,n-1,i),
		ni= successor(current_size+1)-1;
	//assert( point_query(1,0,n-1,ni) == current_size );
	assert( ni >= i );
	if ( ni > i ) {
		nj= h2q[ni];
		swap(h2q[ni],h2q[i]);
		swap(q2h[nj],q2h[j]);
	}
	point_update(1,0,n-1,ni,1);
}

void update( int v, int i, int j, int qi, int qj, int dt ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return ;
	if ( qi <= i && j <= qj ) {
		h[v]+= dt, push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(L(v),i,k,qi,qj,dt), update(R(v),k+1,j,qi,qj,dt);
	push_up(v,i,j);
}

void decrement( int value ) {
	int i= successor(value);
	if ( i < n )
		update(1,0,n-1,i,n-1,-1);
}

int query( int value ) {
	return n-successor(value);
}

int main() {
	int i,j,k,qr,comm;
	for ( ;(n= nextint()) < +oo; ) {
		for ( qr= nextint(), i= 0; i < n; ++i )
			a[i].first= nextint(), a[i].second= i;
		sort(a,a+n);
		for ( i= 0; i < n; ++i ) {
			j= a[i].second;
			q2h[j]= i, h2q[i]= j;
			height[i]= a[i].first;
		}
		for ( build(1,0,n-1); qr--; ) {
			comm= nextint(), k= nextint();
			switch ( comm ) {
				case 1: increment(k-1); break ;
				case 2: printf("%d\n",query(k)); break ;
				case 3: decrement(k); break ;
			}
		}
	}
	return 0;
}

