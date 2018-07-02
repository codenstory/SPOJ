/*
 * FREQUENT
 * TOPIC: segment tree, rmq, binary search
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (1<<17)
#define Q (N<<2)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
using namespace std;

int freq[Q],n,m,a[N],t[N];
pair<int,int> interval[N];

void build( int v, int i, int j ) {
	if ( i == j ) {
		freq[v]= interval[i].second-interval[i].first+1;
		return ;
	}
	int k= (i+j)>>1;
	build(L(v),i,k), build(R(v),k+1,j);
	freq[v]= max(freq[L(v)],freq[R(v)]);
}

int query( int v, int i, int j, int qi, int qj ) {
	if ( qi > j || qj < i || qi > qj )
		return 0;
	if ( qi <= i && j <= qj )
		return freq[v];
	int k= (i+j)>>1;
	return max(query(L(v),i,k,qi,qj),query(R(v),k+1,j,qi,qj));
}

int main() {
	int i,j,k,qr,left,right;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( i= 0; i < n; ++i )
			scanf("%d",&a[i]);
		for ( m= 0, i= 0; i < n; i= j ) {
			for ( j= i+1; j < n && a[j] == a[i]; ++j ) ;
			interval[m++]= {i,j-1}, t[m-1]= i;
		}
		interval[m++]= {i,i-1}, t[m-1]= n;
		assert( t[0] == 0 && t[m-1] == n );
		for ( build(1,0,m-1); qr-- && 2 == scanf("%d %d",&i,&j); ) {
			--i, --j;
			int low, high, mid;
			assert( i >= t[0] );
			assert( i < t[m-1] );
			for ( low= 0, high= m-1; low+1 < high; )
				if ( t[mid= (low+high)>>1] <= i )
					low= mid;
				else high= mid;
			left= high;
			assert( j >= t[0] );
			assert( j < t[m-1] );
			for ( low= 0, high= m-1; low+1 < high; )
				if ( t[mid= (low+high)>>1] > j )
					high= mid;
				else low= mid;
			right= low;
			assert( t[left] > i );
			assert( t[right] <= j );
			int res= query(1,0,m-1,left,right-1);
			res= max(res,min(j+1,t[left])-i);
			res= max(res,j-max(i,t[right])+1);
			printf("%d\n",res);
		}
	}
	return 0;
}

