/*
 * MULTQ3
 * TOPIC: segment tree + lazy propagation
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N (1<<21)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
typedef int i64;
using namespace std;

i64 bucket[N], sum1[N], sum2[N];

class ST {
private:
	int n;
	void add_value( int v, int i, int j, i64 dt ) {
		bucket[v] += dt;
		i64 tmp, rounds = (dt/(j-i+1))%3;
		if ( !rounds ) return ;
		if ( rounds == 1 ) 
			tmp = sum2[v], sum2[v] = sum1[v], sum1[v] = (j-i+1)-tmp-sum1[v];
		else 
			tmp = sum1[v], sum1[v] = sum2[v], sum2[v] = (j-i+1)-tmp-sum2[v];
	}
	void push_down( int v, int i, int j ) {
		if ( i < j && bucket[v] ) {
			int k = (i+j)>>1;
			add_value(L(v),i,k, bucket[v]/(j-i+1)*(k-i+1)), add_value(R(v),k+1,j, bucket[v]/(j-i+1)*(j-k));
			bucket[v] = 0;
		}
	}
	void push_up( int v, int i, int j ) {
		if ( i < j ) {
			sum1[v] = sum1[L(v)]+sum1[R(v)];
			sum2[v] = sum2[L(v)]+sum2[R(v)];
		}
	}
	void update( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return ;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) {
			add_value(v,i,j,j-i+1);
			return ;
		}
		int k = (i+j)>>1;
		update(L(v),i,k,qi,qj), update(R(v),k+1,j,qi,qj);
		push_up(v,i,j);
	}
	i64 query( int v, int i, int j, int qi, int qj ) {
		if ( qi > j || qj < i ) return 0;
		push_down(v,i,j);
		if ( qi <= i && j <= qj )
			return (j-i+1)-sum1[v]-sum2[v];
		int k = (i+j)>>1;
		i64 left = query(L(v),i,k,qi,qj), right = query(R(v),k+1,j,qi,qj);
		push_up(v,i,j);
		return left+right;
	}
public:
	ST() {}
	void init( int n ) { this->n = n; }
	void update( int qi, int qj ) { update(1,0,n-1,qi,qj); }
	i64 query( int qi, int qj ) { return query(1,0,n-1,qi,qj); }
} st;

int main() {
	int i,j,k,n,qr;
	scanf("%d %d",&n,&qr);
	for ( st.init(n); qr-- && 3 == scanf("%d %d %d",&k,&i,&j); ) {
		if ( i > j ) swap(i,j);
		if ( k == 1 ) printf("%d\n",st.query(i,j));
		else st.update(i,j);
	}
	return 0;
}

