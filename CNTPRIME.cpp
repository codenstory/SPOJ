/*
 * CNTPRIME
 * TOPIC: segment tree, lazy propagation
 * status: some weird WA
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (100000)
#define Q (1<<21)
#define BIT(k) (1ULL<<(k))
#define SET(k) (a[(k)>>3] |= BIT((k)&7ULL))
#define TST(k) (a[(k)>>3] & BIT((k)&7ULL))
#define CLR(k) (a[(k)>>3] &= ~BIT((k)&7ULL))
using namespace std;

unsigned char a[(Q>>3)+8];
int x[N],n,sum[N*5],h[N*5];
bool change[N*5];

int build( int v, int i, int j ) {
	if ( i == j ) {
		x[i]= TST(abs(x[i]))?1:0, change[v]= false;
		return sum[v]= x[i];
	}
	int k= (i+j)>>1;
	return sum[v]= build(v<<1,i,k)+build((v<<1)+1,k+1,j);
}

void push_down( int v, int i, int j ) {
	if ( !change[v] ) return ;
	sum[v]= (j-i+1)*h[v];
	if ( i < j )
		change[v<<1]= true, h[v<<1]= h[v], h[(v<<1)+1]= h[v], change[1+(v<<1)]= true;
	change[v]= false;
}

void push_up( int v, int i, int j ) {
	if ( i == j ) return ;
	sum[v]= sum[v<<1]+sum[(v<<1)+1];
}

void update( int v, int i, int j, int qi, int qj, int newval ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return ;
	if ( qi <= i && j <= qj ) {
		change[v]= true, h[v]= newval, push_down(v,i,j);
		return ;
	}
	int k= (i+j)>>1;
	update(v<<1,i,k,qi,qj,newval), update(1+(v<<1),k+1,j,qi,qj,newval);
	push_up(v,i,j);
}

int query( int v, int i, int j, int qi, int qj ) {
	push_down(v,i,j);
	if ( qi > j || qj < i )
		return 0;
	if ( qi <= i && j <= qj )
		return sum[v];
	int k= (i+j)>>1;
	int res= query(v<<1,i,k,qi,qj)+query(1+(v<<1),k+1,j,qi,qj);
	//push_up(v,i,j);
	return res;
}

int main() {
	int i,j,k,ts,cs= 0,qr,comm;
	for ( SET(2), i= 3; i < Q; SET(i), i+= 2 ) ;
	for ( i= 3; i < Q; i+= 2 )
		for ( j= i+i; j < Q && TST(i); CLR(j), j+= i ) ;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&qr);
		for ( i= 0; i < n; scanf("%d",&x[i++]) ) ;
		build(1,0,n-1), printf("Case %d:\n",++cs);
		for ( ;qr-- && 3 == scanf("%d %d %d",&comm,&i,&j); ) {
			--i, --j;
			if ( comm == 0 ) {
				scanf("%d",&k);
				assert( 2 <= k );
				assert( k <= 1000000 );
				update(1,0,n-1,i,j,TST(k)?1:0);
				continue ;
			}
			printf("%d\n",query(1,0,n-1,i,j));
		}
	}
	return 0;
}

