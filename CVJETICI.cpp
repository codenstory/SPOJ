/*
 * CVJETICI
 * TOPIC: BIT, very neat
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#define N (1<<21)
#define L(k) ((k) & (~(k)+1ULL))
using namespace std;
typedef long long i64;

class BIT {
private:
	int n,m,K;
	i64 tr[N];
public:
	void init( int n ) {
		for ( this->n = n, K = 0; (m=(1<<K)) <= n; ++K ) ;
		for ( int i = 0; i < m; tr[i++] = 0 ) ;
	}
	BIT() {}
	BIT( int n ) {
		init(n);
	}
	i64 prefix( unsigned int i ) {
		i64 ans = 0;
		for ( ;i; ans += tr[i], i &= ~L(i) ) ;
		return ans;
	}
	i64 query( unsigned int i, unsigned int j ) {
		return prefix(j)-prefix(i-1);
	}
	void add_at( unsigned int i, i64 dt ) {
		for ( ;i < m; tr[i] += dt, i += L(i) ) ;
	}
} A, B1;

int x[N],y[N];

int main() {
	int i,j,k,n;
	i64 t,nt;
	for ( ;1==scanf("%d",&n); ) {
		A.init(100100), B1.init(100100);
		for ( t = 0, i = 0; i < n; ++i ) {
			scanf("%d %d",&j,&k), x[i] = j, y[i] = k;
			i64 xx = B1.prefix(j), yy = B1.prefix(k);
			B1.add_at(j,-xx), B1.add_at(j+1,xx);
			B1.add_at(k,-yy), B1.add_at(k+1,yy);
			printf("%lld\n",xx+yy);
			B1.add_at(x[i]+1,1), B1.add_at(y[i],-1);
		}
	}
	return 0;
}

