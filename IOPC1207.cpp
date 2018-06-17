/*
 * IOPC1207: GM Plants
 * TOPIC: segment tree with lazy propagation, math formula
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
typedef long long i64;
using namespace std;
#define DIM 3
#define N (100100)
#define Q (N<<4)
#define white first
#define red second
#define L(v) ((v)<<1)
#define R(v) (1|L(v))

pair<i64,i64> cross( pair<i64,i64> a, pair<i64,i64> b ) {
	return {a.white*b.white+a.red*b.red,a.white*b.red+a.red*b.white};
}

int n[DIM];
i64 tr[DIM][Q];
unsigned char change[DIM][Q];

void push_down( const int t, int v, int i, int j ) {
	if ( !change[t][v] ) return ;
	tr[t][v]= (j-i+1)-tr[t][v], change[t][v]= 0;
	if ( i < j ) {
		//assert( !change[t][L(v)] ); <-- this is wrong
		//assert( !change[t][R(v)] );
		change[t][L(v)]^= 1, change[t][R(v)]^= 1;
	}
}

void push_up( const int t, int v, int i, int j ) {
	if ( i < j )
		tr[t][v]= tr[t][L(v)]+tr[t][R(v)];
}

void update( const int t, int v, int i, int j, int qi, int qj ) {
	push_down(t,v,i,j);
	if ( qi > j || qj < i )
		return ;
	if ( qi <= i && j <= qj ) {
		change[t][v]= 1, push_down(t,v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(t,L(v),i,k,qi,qj), update(t,R(v),k+1,j,qi,qj);
	push_up(t,v,i,j);
}

i64 query( const int t, int v, int i, int j, int qi, int qj ) {
	push_down(t,v,i,j);
	if ( qi > j || qj < i )
		return 0LL;
	if ( qi <= i && j <= qj )
		return tr[t][v];
	int k= (i+j)>>1;
	i64 res= query(t,L(v),i,k,qi,qj)+query(t,R(v),k+1,j,qi,qj);
	push_up(t,v,i,j);
	return res;
}

int main() {
	int i,j,k,ts,comm,qr,t;
	for ( scanf("%d",&ts); ts--; ) {
		for ( t= 0; t < DIM; ++t )
			scanf("%d",n+t);
		memset(change,0,sizeof change), memset(tr,0,sizeof tr);
		for ( scanf("%d",&qr); qr-- && 3 == scanf("%d %d %d",&t,&i,&j); ) {
			if ( t < 3 )
				query(t,1,0,n[t]-1,i,j), update(t,1,0,n[t]-1,i,j);
			else {
				int x1= i, y1= j, z1, x2,y2,z2;
				scanf("%d %d %d %d",&z1,&x2,&y2,&z2);
				i64 rx= query(0,1,0,n[0]-1,x1,x2), wx= x2-x1+1-rx;
				i64 ry= query(1,1,0,n[1]-1,y1,y2), wy= y2-y1+1-ry;
				i64 rz= query(2,1,0,n[2]-1,z1,z2), wz= z2-z1+1-rz;
				pair<i64,i64> xx= {wx,rx},
							  yy= {wy,ry},
							  zz= {wz,rz};
				pair<i64,i64> res= cross(cross(xx,yy),zz);
				printf("%lld\n",res.red);
			}
		}
	}
	return 0;
}

