/*
 * DCEPC11I
 * TOPIC: segment tree with updates
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (100100)
#define Q (N<<5)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
typedef unsigned long long u64;
using namespace std;

u64 tr[Q],x0[Q],n,qr,d[Q];

u64 F( u64 x, u64 d, u64 n ) {
	return x*n + d*((n-1)*n/2);
}

void push_down( int v, int i, int j ) {
	if ( !x0[v] && !d[v] ) return ;
	tr[v]+= F(x0[v],d[v],j-i+1);
	if ( i < j ) {
		int k= (i+j)>>1;
		x0[L(v)]+= x0[v], x0[R(v)]+= x0[v]+d[v]*(k-i+1), d[L(v)]+= d[v], d[R(v)]+= d[v];
	}
	x0[v]= d[v]= 0LL;
}

void push_up( int v, int i, int j ) {
	if ( i < j )
		tr[v]= tr[L(v)]+tr[R(v)];
}

void update( int v, int i, int j, int qi, int qj, u64 val ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return ;
	if ( qi <= i && j <= qj ) {
		x0[v]+= (i-qi)+val, ++d[v], push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(L(v),i,k,qi,qj,val), update(R(v),k+1,j,qi,qj,val);
	push_up(v,i,j);
}

u64 query( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return 0LL;
	if ( qi <= i && j <= qj ) 
		return tr[v];
	int k= (i+j)>>1;
	u64 res= query(L(v),i,k,qi,qj)+query(R(v),k+1,j,qi,qj);
	push_up(v,i,j);
	return res;
}

int main() {
	int i,j,k,comm;
	for ( ;2 == scanf("%llu %llu",&n,&qr); ) {
		memset(x0,0,sizeof x0), memset(tr,0,sizeof tr);
		memset(d,0,sizeof d);
		for ( ;qr-- && 3 == scanf("%d %d %d",&comm,&i,&j); ) 
			if ( comm == 0 ) update(1,0,n-1,i-1,j-1,1LL);
			else printf("%llu\n",query(1,0,n-1,i-1,j-1));
		break ;
	}
	return 0;
}

