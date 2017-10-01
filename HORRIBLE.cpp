/*
 * HORRIBLE
 * TOPIC: BIT, range update / range query
 * status: Accepted
 */
#include <algorithm>
#include <cassert>
#include <cstdio>
using namespace std;
typedef long long i64;
#define N (1<<22)
#define L(k) ((k) & (~(k)+1ULL))

class BIT {
private:
	int m,K,n;
	i64 tr[N];
public:
	void init( int n ) {
		for ( this->n = n, K = 0; (m=(1<<K)) <= n; ++K ) ;
		for ( int i = 0; i < m; tr[i++] = 0 ) ;
	}
	BIT() {};
	BIT( int n ) {
		init(n);
	}
	i64 prefix( unsigned int i ) {
		i64 ans = 0;
		for ( ;i; ans += tr[i], i &= ~L(i) ) ;
		return ans;
	}
	i64 sum( unsigned int i, unsigned int j ) {
		return prefix(j)-prefix(i-1);
	}
	void add_at( unsigned int i, i64 dt ) {
		for ( ;i < m; tr[i] += dt, i += L(i) ) ;
	}
} A, B1, B2;

int n;

int main() {
	int i,j,k,ts,qr,comm,p,q;
	i64 v;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&qr);
		A.init(N), B1.init(N), B2.init(n);
		for ( ;qr-- && 1 == scanf("%d",&comm); ) {
			if ( comm == 0 ) {
				assert( 3 == scanf("%d %d %lld",&p,&q,&v) );
				B1.add_at(p,v);
				B2.add_at(p,(p-1)*v);
				B1.add_at(q+1,-v);
				B2.add_at(q+1,-q*v);
			}
			else {
				assert( 2 == scanf("%d %d",&p,&q) );
				if ( p > q ) swap(p,q);
				printf("%lld\n",q*B1.prefix(q)-B2.prefix(q) - ((p-1)*B1.prefix(p-1)-B2.prefix(p-1)));
			}
		}
	}
	return 0;
}

