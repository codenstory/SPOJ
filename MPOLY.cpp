/*
 * MPOLY: Polygon
 * TOPIC: convex hull, dp, left turns
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N 0x80
#define x first
#define y second
#define tol 1e-13
#include <cmath>
typedef long long i64;
using namespace std;

int n,z[N][N];

const double pi = 2*acos(0.00);

typedef struct cell {
	i64 x,y,dist;
	double ang;
	cell() {x=y=0;}
	cell( i64 x, i64 y ) {
		this->x=x,this->y=y;
	}
	i64 operator [] ( const int i ) const {
		return i==0?x:y;
	}
} cell;

i64 operator ^ ( const cell &a, const cell &b ) {
	return a.x*b.y-a.y*b.x;
}

bool operator < ( const cell &a, const cell &b ) {
	if ( fabs(a.ang-b.ang) < tol )
		return a.dist > b.dist;
	return a.ang < b.ang;
}

i64 operator , ( const cell &a, const cell &b ) {
	return a.x*b.x+a.y*b.y;
}

cell operator - ( const cell &a, const cell &b ) {
	return cell(a.x-b.x,a.y-b.y);
}

double operator | ( const cell &a, const cell &b ) {
	double A = sqrt((a,a)), B = sqrt((b,b)),
		   co = (a,b)/A/B, si = (a^b)/A/B;
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
	//i64 co = (u,v), si = (u^v);
	i64 co = (b.x-a.x)*(c.x-a.x)+(b.y-a.y)*(c.y-a.y),
		si = (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
	if ( si < 0 ) return false ;
	if ( si > 0 ) return true ;
	assert( co >= 0 );
	return false ;
}

cell c[N],UNIT(1,0);

int main() {
	int i,j,k,ans;
	for (;1==scanf("%d",&n);) {
		for ( i = 1; i <= n; scanf("%lld %lld",&c[i].x,&c[i].y), ++i ) ;
		for ( i = 1; i <= n; ++i )
			c[i].dist = (c[i],c[i]), c[i].ang = (UNIT|c[i]);
		for ( sort(c+1,c+n+1), i = n; --i>=1;)
			for ( j = i+1; j <= n; ++j )
				for ( z[i][j] = 3, k = j+1; k <= n; ++k )
					if ( is_left_turn(c[i],c[j],c[k]) )
						z[i][j] = max(z[i][j],z[j][k]+1);
		for ( ans = 0, i = 1; i <= n; ++i )
			for ( j = i+1; j <= n; ++j )
				ans = max(ans,z[i][j]);
		printf("%d\n",ans);
	}
	return 0;
}

