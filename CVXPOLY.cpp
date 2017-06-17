/*
 * CVXPOLY: Convex Polygons
 * TOPIC: dp, O(n^4), convex hulls
 * status: Accepted
 */
#include <cassert>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#define N 0x80
#define tol 1e-14
#define DIM 3
#define MOD 1234567LL
#define T (1LL<<60)
typedef long long i64;
using namespace std;

const double pi = 2*acos(0.00);

typedef struct cell {
	i64 x[DIM];
	pair<i64,i64> ang;
	cell() { memset(x,0,sizeof x); }
	cell( i64 a, i64 b ) {
		0[x] = a, 1[x] = b, 2[x] = 0;
	}
	i64 operator [] ( const int i ) const {
		return x[i];
	}
} cell;

cell operator - ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM-1; r.x[i] = a[i]-b[i], ++i ) ;
	return r;
}

cell operator + ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM-1; r.x[i] = a[i]+b[i], ++i ) ;
	return r;
}

i64 operator , ( const cell &a, const cell &b ) {
	i64 s = 0;
	for ( int i = 0; i < DIM-1; s += a[i]*b[i], ++i ) ;
	return s;
}

cell operator ^ ( const cell &a, const cell &b ) {
	cell r;
	for ( int j,k,i = 0; i < DIM; ++i )
		j = ((i+1)%DIM), k = ((j+1)%DIM), r.x[i] = a[j]*b[k]-a[k]*b[j];
	return r;
}

long double operator | ( const cell &a, const cell &b ) {
	long double A = sqrt((a,a)+0.00), B = sqrt((b,b)+0.00),
				co = (a,b)/A/B, si = (a^b)[DIM-1]/A/B;
	if ( co >= 0 && si >= 0 )
		return acos(co);
	if ( co <= 0 && si >= 0 )
		return acos(co);
	if ( co >= 0 && si <= 0 )
		return asin(si);
	return -pi+acos(-co);
}

bool is_left_turn( const cell &a, const cell &b, const cell &c ) {
	//cell u = b-a, v = c-a;
	//i64 co = (u,v), si = (u^v)[DIM-1];
	i64 co = (b[0]-a[0])*(c[0]-a[0])+(b[1]-a[1])*(c[1]-a[1]),
		si = (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
	if ( si < 0 ) return false ;
	if ( si > 0 ) return true ;
	assert( co >= 0 );
	return false ;
}

int n,m;
i64 deg[N],e[N],f[N],r[N];
cell c[N],a[N],UNIT(1,0),ZERO;

bool is_convex_polygon( const cell c[], int m ) {
	int i,j,k;
	if ( m <= 2 ) return false;
	for ( i = 1, j = (i+1)%m, k = 0; k < 2*m; i = j, ++j, j %= m, ++k )
		if ( !is_left_turn(c[(i+m-1)%m],c[i],c[j]) )
			return false ;
	return true ;
}

struct cell_comparator {
	bool operator () ( const cell &a, const cell &b ) {
		i64 x = a.ang.first, y = a.ang.second,
			z = b.ang.first, t = b.ang.second;
		if ( x <= 0 && z > 0 )
			return true;
		if ( x > 0 && z <= 0 )
			return false ;
		if ( x >= 0 && z < 0 )
			return false ;
		if ( x == 0 && z == 0 )
			return false ;
		if ( x < 0 && z < 0 )
			return x*x*t > z*z*y;
		if ( x > 0 && z > 0 )
			return x*x*t < z*z*y;
		if ( x < 0 && z >= 0 )
			return true ;
		/*
		printf("x = %lld, z = %lld\n",x,z);
		assert( false );*/
		return asin(x/sqrt(y)) < asin(z/sqrt(t));
	}
};

bool operator < ( const cell &a, const cell &b ) {
	for ( int i = 0; i < DIM-1; ++i ) 
		if ( fabs(a[i]-b[i]) >= tol )
			return a[i] < b[i];
	return false ;
}

int brute_force() {
	unsigned int u;
	cell z[N],a[N],pole;
	int i,j,k,m,d[N] = {0},idx;
	i64 ans = 0;
	for ( u = 0; u < (1<<n); ++u ) {
		for ( m = 0, i = 0; i < n; ++i )
			if ( (u>>i)&1 )
				z[m++] = c[i];
		if ( m <= 2 ) continue ;
		assert( m >= 3 );
		sort(z,z+m);
		assert( m >= 3 );
		/*
		 * funny bug: a[k++] = ZERO does not work
		 */
		for ( idx = -1, i = 0; i < n; ++i )
			if ( z[0][0] == c[i][0] && z[0][1] == c[i][1] )
				idx = i;
		assert( idx >= 0 );
		for ( k = 0, pole = z[0], a[k] = ZERO, ++k, i = 1; i < m; ++i )
			//a[k++] = z[i]-pole, a[k-1].ang = (UNIT|a[k-1]), a[k-1].dist = (a[k-1],a[k-1]);
			a[k++] = z[i]-pole, a[k-1].ang = make_pair(a[k-1][1],(a[k-1],a[k-1]));
		assert( k >= 3 );
		sort(a+1,a+k,cell_comparator());
		/*
		for ( i = 1; i+1 < k; ++i )
			assert( a[i].ang < a[i+1].ang );*/
		if ( is_convex_polygon(a,k) )
			++ans, ++d[idx];
		else {
			/*
			printf("This is not a convex polygon (k = %d):\n",k);
			for ( i = 0; i < k; ++i )
				printf("%lld %lld\n",a[i][0],a[i][1]);
			*/
		}
	}
	/*
	for ( i = 0; i < n; ++i )
		printf("ans[%d] = %d\n",i,d[i]);*/
	return ans;
}

i64 dp[N][N];
cell x[N];

int main() {
	int i,j,k,ts,idx,K,cs = 0;

	i64 ans,dans,bf;
	cell pole;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n) && ++cs; ) {
		for ( i = 0; i < n; scanf("%lld %lld",&c[i].x[0],&c[i].x[1]), ++i ) ;
		if ( n <= 2 ) {
			puts("0");
			continue ;
		}
		if ( n == 3 ) {
			puts("1");
			continue ;
		}
		for ( ans = 0, idx = 0; idx < n; ++idx ) {
			for ( pole = c[idx], m = 0, a[m++] = ZERO, j = 0; j < n; ++j )
				if ( j != idx )
					a[m++] = c[j]-pole, a[m-1].ang = make_pair(a[m-1][1],(a[m-1],a[m-1]));
			/*
			for ( i = 1; i+1 < m; ++i ) {
				if ( fabs(a[i].ang-a[i+1].ang) < tol )
					goto next;
				assert( fabs(a[i].ang-a[i+1].ang) >= tol );
				assert( a[i].ang < a[i+1].ang );
			}
			*/
			for ( K = 0, i = 1; i < m; ++i )
				if ( a[i][0] >= 0 && !(a[i][0] == 0 && a[i][1] > 0) )
					x[++K] = a[i];
			sort(x+1,x+K+1,cell_comparator());
			if ( K <= 1 ) continue ;
			assert( K >= 2 );
			for ( i = K; --i >= 1; )
				for ( j = i+1; j <= K; ++j ) 
					for ( dp[i][j] = 1LL, k = j+1; k <= K; ++k )
						if ( is_left_turn(x[i],x[j],x[k]) )
							if ( (dp[i][j] += dp[j][k]) >= T )
								dp[i][j] %= MOD;
			for ( dans = 0, i = 1; i <= K; ++i )
				for ( j = i+1; j <= K; ++j )
					if ( (dans += dp[i][j]) >= T )
						 dans %= MOD;
			/*printf("idx = %d, K = %d, dans = %lld\n",idx,K,dans);*/
			if ( (ans += dans) >= T )
				 ans %= MOD;
		}
		/*
		bf = brute_force();
		if ( ans != bf ) 
			printf("Case %d: Difference: %lld %lld (n = %d) ",cs,ans,bf,n);*/
		printf("%lld\n",ans%MOD);
	}
	return 0;
}

