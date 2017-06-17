/*
 * TFOSS: Fossil in the Ice
 * TOPIC: discrete ternary search, computational geometry, convex hull
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <cfloat>
#define tol 1e-13
#define N (1<<17)
#define DIM 3
#define oo (DBL_MAX-0x400)
typedef long long i64;
using namespace std;

const long double pi = 2*acos(0.00);
int st[N],*top;

typedef struct cell {
	long double x[DIM],ang,dist;
	long double operator [] ( const int i ) const {
		return x[i];
	}
	cell() { memset(x,0,sizeof x); };
	cell( long double a, long double b ) {
		0[x] = a, 1[x] = b, 2[x] = 0;
	}
} cell;

cell operator + ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM; r.x[i] = a[i]+b[i], ++i ) ;
	return r;
}

cell operator - ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM; r.x[i] = a[i]-b[i], ++i ) ;
	return r;
}

long double operator , ( const cell &a, const cell &b ) {
	long double s = 0;
	for ( int i = 0; i < DIM; s += a[i]*b[i], ++i ) ;
	return s;
}

cell operator ^ ( const cell &a, const cell &b ) {
	cell r;
	for ( int k,j,i = 0; i < DIM; ++i )
		k = ((j = (i+1)%DIM)+1)%DIM, r.x[i] = a[j]*b[k]-a[k]*b[j];
	return r;
}

long double operator | ( const cell &a, const cell &b ) {
	long double A = sqrt((a,a)), B = sqrt((b,b)),
		   co = (a,b)/A/B, si = (a^b)[DIM-1]/A/B;
	if ( co >= 0 && si >= 0 )
		return acos(co);
	if ( co <= 0 && si >= 0 )
		return acos(co);
	if ( co >= 0 && si <= 0 )
		return asin(si);
	return -pi+acos(-co);
}

cell operator * ( const cell &a, const long double t ) {
	cell r;
	for ( int i = 0; i < DIM; ++i )
		r.x[i] = a[i]*t;
	return r;
}

cell operator ^ ( const cell &a, const long double t ) {
	long double co = cos(t), si = sin(t);
	return cell(a[0]*co-a[1]*si,a[0]*si+a[1]*co);
}

bool operator < ( const cell &a, const cell &b ) {
	for ( int i = 0; i < DIM; ++i )
		if ( fabs(a[i]-b[i]) >= tol )
			return a[i] < b[i];
	return false;
}

struct cell_comparator {
	bool operator () ( const cell &a, const cell &b ) {
		if ( fabs(a.ang-b.ang) < tol )
			return a.dist > b.dist;
		return a.ang < b.ang;
	}
};

bool left_turn( const cell &p, const cell &q, const cell &r ) {
	cell u = q-p, v = r-p;
	long double co = (u,v), si = (u^v)[DIM-1];
	if ( si > 0 ) return true ;
	if ( si < 0 ) return false ;
	assert( co >= 0 );
	return false ;
}

long double dist( const cell &a, const cell &b ) {
	return (a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1]);
}

int n,m;
cell c[N],a[N],UNIT(1,0),pole;

int main() {
	int i,j,k,ts,left,right,lleft,rright;
	long double ans;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%Lf %Lf",&c[i].x[0],&c[i].x[1]), ++i ) ;
		if ( n <= 1 ) {
			nx1:
			puts("0");
			continue ;
		}
		if ( n == 2 ) {
			nx2:
			printf("%lld\n",(i64)(dist(c[0],c[1])+tol));
			continue ;
		}
		assert( n >= 3 );
		for ( sort(c,c+n), pole = c[0], i = 0; i < n; c[i] = c[i]-pole, ++i ) ;
		for ( i = 1; i < n; ++i )
			c[i].dist = (c[i],c[i]), c[i].ang = (UNIT|c[i]);
		sort(c+1,c+n,cell_comparator());
		for ( m = 0, a[m++] = c[0], i = 1; i < n; i = j ) 
			for ( a[m++] = c[i], j = i+1; j < n && fabs(c[j].ang-c[i].ang) < tol; ++j ) ;
		if ( m == 2 ) {
			c[0] = a[0], c[1] = a[1];
			goto nx2;
		}
		assert( m >= 3 );
		for ( top = st, *++top = 0, *++top = 1, *++top = 2, i = 3; i < m; *++top = i++ ) 
			for ( ;top-st >= 2 && !left_turn(a[*(top-1)],a[*top],a[i]); --top ) ;
		for ( n = 0; top > st; c[n++] = a[*top--] ) ;
		if ( n == 1 ) goto nx1;
		if ( n == 2 ) goto nx2;
		assert( n >= 3 );
		for ( ans = -oo, i = 0; i < n; ++i ) {
			for ( left = 1, right = n-1; abs(right-left) >= 4; ) {
				lleft = (2*left+right)/3, rright = (left+2*right)/3;
				if ( dist(c[i],c[(lleft+i)%n]) < dist(c[i],c[(rright+i)%n]) )
					left = lleft;
				else right = rright;
			}
			for (;left <= right; ans = max(ans,dist(c[i],c[(left+i)%n])), ++left ) ;
		}
		printf("%lld\n",(i64)(ans+tol));
	}
	return 0;
}

