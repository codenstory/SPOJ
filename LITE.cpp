/*
 * LITE
 * TOPIC: lazy propagation segment tree
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (1<<21)
#define M (N<<1)
#define L(v) ((v)<<1)
#define R(V) (1|L(v))
typedef int i64;
using namespace std;
	
int sum[M],bucket[M];

class ST {
private:
	int n;
	void flip( int v, int i, int j, i64 value ) {
		bucket[v] += value;
		if ( (value/(j-i+1)) & 1 )
			sum[v] = (j-i+1)-sum[v];
	}

	void push_down( int v, int i, int j ) {
		if ( i < j && bucket[v] ) {
			assert( i < j );
			int k = (i+j)>>1;
			flip(L(v),i,k,bucket[v]/(j-i+1LL)*(k-i+1LL)), flip(R(v),k+1,j,bucket[v]/(j-i+1LL)*(j-k+0LL)), bucket[v] = 0;
			/*
			if ( sum[v] != sum[L(v)]+sum[R(v)] )
				printf("[%d,%d] This %lld %lld %lld\n",i,j,sum[v],sum[L(v)],sum[R(v)]);*/
			assert( sum[v] == sum[L(v)]+sum[R(v)] );
		}
	}

	void push_up( int v, int i, int j ) {
		if ( i < j ) 
			sum[v] = sum[L(v)]+sum[R(v)];
	}

	void range_flip( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return ;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) {
			flip(v,i,j,j-i+1);
			return ;
		}
		int k = (i+j)>>1;
		range_flip(L(v),i,k,qi,qj), range_flip(R(v),k+1,j,qi,qj);
		push_up(v,i,j);
	}
	int query( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return 0;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) 
			return sum[v];
		int k = ((i+j)>>1);
		i64 ans = query(L(v),i,k,qi,qj)+query(R(v),k+1,j,qi,qj);
		return ans;
	}
public:
	ST() {}
	void init( int n ) {
		int m = (n*4)+0x400;
		this->n = n;
		/*
		for ( int i = 0; i < m; ++i )
			bucket[i] = sum[i] = 0;*/
	}
	void update( int qi, int qj ) {
		range_flip(1,0,n-1,qi,qj);
	}
	int query( int qi, int qj ) { return query(1,0,n-1,qi,qj); }
} st;

int main() {
	int i,j,k,n,qr;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( st.init(n); qr-- && 3 == scanf("%d %d %d",&k,&i,&j); ) {
			--i, --j;
			if ( i > j ) swap(i,j);
			if ( k == 1 ) {
				//printf("Query %d %d\n",i+1,j+1);
				printf("%d\n",st.query(i,j));
			}
			else {
				//printf("Update %d %d\n",i+1,j+1);
				st.update(i,j);
			}
		}
	}
	return 0;
}

