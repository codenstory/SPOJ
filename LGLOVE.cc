/*
 * LGLOVE
 * TOPIC: segment tree with updates, number theory, sieve
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define MOD (1000000007LL)
#define N (100100)
#define Q (N<<2)
#define MAXVAL (300008LL)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
#define oo (1<<30)
typedef long long i64;
using namespace std;

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) {
	!y?(*a= 1,*d= x,*b= 0):(ext_euclid(y,x%y,b,a,d),*b-=(*a)*(x/y));
}

i64 inv( i64 x, i64 P ) {
	i64 a,b,d;
	for ( ext_euclid(x,P,&a,&b,&d); a < 0; a+= P ) ;
	return a%P;
}

i64 GCD( i64 x, i64 y ) { return !y?x:GCD(y,x%y); }
i64 LCM( i64 x, i64 y ) { return (x*y*inv(GCD(x,y),MOD)) % MOD; }

int n,mx[Q],mn[Q],h[Q];
bool has_zero[Q];
i64 A[N],lcm[MAXVAL];

void push_down( int v, int i, int j ) {
	if ( !h[v] ) return ;
	mx[v]+= h[v], mn[v]+= h[v];
	assert( mx[v] >= 0 && mn[v] >= 0 );
	assert( mx[v] < MAXVAL );
	if ( !mn[v] ) has_zero[v]= true ;
	if ( i < j )
		h[L(v)]+= h[v], h[R(v)]+= h[v];
	h[v]= 0;
}

void push_up( int v, int i, int j ) {
	if ( i < j ) {
		mx[v]= max(mx[L(v)],mx[R(v)]);
		mn[v]= min(mn[L(v)],mn[R(v)]);
		has_zero[v]= (has_zero[L(v)] || has_zero[R(v)]);
	}
}

void build( int v, int i, int j ) {
	h[v]= 0, has_zero[v]= false ;
	if ( i == j ) {
		mx[v]= mn[v]= A[i], has_zero[v]= !A[i];
		return ;
	}
	int k= (i+j)>>1;
	build(L(v),i,k), build(R(v),k+1,j), push_up(v,i,j);
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
	update(L(v),i,k,qi,qj,dt), update(R(v),k+1,j,qi,qj,dt), push_up(v,i,j);
}

int q_lcm( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return -oo;
	if ( qi <= i && j <= qj ) 
		return mx[v];
	int k= (i+j)>>1,
		left= q_lcm(L(v),i,k,qi,qj),
		right= q_lcm(R(v),k+1,j,qi,qj);
	return max(left,right);
}

int q_gcd( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return +oo;
	if ( qi <= i && j <= qj ) 
		return mn[v];
	int k= (i+j)>>1,
		left= q_gcd(L(v),i,k,qi,qj),
		right= q_gcd(R(v),k+1,j,qi,qj);
	return min(left,right);
}

bool zero( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return false ;
	if ( qi <= i && j <= qj )
		return has_zero[v];
	int k= (i+j)>>1;
	bool res= (zero(L(v),i,k,qi,qj)|zero(R(v),k+1,j,qi,qj));
	push_up(v,i,j);
	return res;
}

bool isprime[MAXVAL];

int main() {
	int i,j,k,qr,comm;
	i64 dt;
	for ( isprime[2]= true, i= 3; i < MAXVAL; isprime[i]= true, i+= 2 ) ;
	for ( i= 3; i < MAXVAL; i+= 2 )
		if ( isprime[i] )
			for ( j= i+i; j < MAXVAL; isprime[j]= false, j+= i ) ;
	for ( lcm[0]= 0, lcm[1]= 1, i= 2; i < MAXVAL; lcm[i++]= 1 ) ;
	for ( i= 2; i < MAXVAL; ++i )
		if ( isprime[i] ) 
			for ( i64 t= i; t < MAXVAL; t*= i )
				lcm[t]*= i, lcm[t]%= MOD;
	i64 c= 1ll;
	for ( i= 2; i < MAXVAL; ++i ) 
		c*= lcm[i], c%= MOD, lcm[i]= c;
	// for ( lcm[0]= 0, lcm[dt= 1]= 1; ++dt < MAXVAL; lcm[dt]= ((lcm[dt-1]*dt)%MOD)*(lcm[dt-1]==0?0:inv(GCD(lcm[dt-1],dt),MOD)), lcm[dt]%= MOD ) ;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( i= 0; i < n; scanf("%lld",&A[i++]) ) ;
		for ( build(1,0,n-1); qr-- && 3 == scanf("%d %d %d",&comm,&i,&j); ) {
			if ( comm == 0 ) {
				scanf("%d",&k);
				update(1,0,n-1,i,j,k);
				continue ;
			}
			/*
			if ( zero(1,0,n-1,i,j) ) {
				puts("0");
				continue ;
			}
			*/
			printf("%lld\n",comm==1?lcm[q_lcm(1,0,n-1,i,j)]:lcm[q_gcd(1,0,n-1,i,j)]);
		}
	}
	return 0;
}

