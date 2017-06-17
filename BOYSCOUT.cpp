/*
 * BOYSCOUT: Boy Scouts
 * TOPIC: convex hull, dp, left turns
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>
#include <cmath>
#define tol 1e-13
#define N 0x80
using namespace std;

const double pi = 2*acos(0.00);

typedef struct cell {
	double x,y,ang,dist;
	cell() {x=y=0;}
	cell( double x, double y ) {
		this->x = x, this->y = y;
	}
} cell;

cell operator - ( const cell &a, const cell &b ) {
	return cell(a.x-b.x,a.y-b.y);
}

double operator , ( const cell &a, const cell &b ) {
	return a.x*b.x + a.y*b.y;
}

double operator ^ ( const cell &a, const cell &b ) {
	return a.x*b.y - a.y*b.x;
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

bool operator < ( const cell &a, const cell &b ) {
	if ( fabs(a.ang-b.ang) < tol )
		return a.dist > b.dist;
	return a.ang < b.ang;
}

bool is_left_turn( const cell &a, const cell &b, const cell &c ) {
	double co = (b.x-a.x)*(c.x-a.x)+(b.y-a.y)*(c.y-a.y),
		   si = (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
	if ( si < 0 ) return false ;
	if ( si > 0 ) return true ;
	assert( co >= 0 ) ;
	return false ;
}

int m,n,z[N][N];
cell c[N],p[N],pole,UNIT(1,0);

int main() {
	int i,j,k,idx,ans;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%lf %lf",&c[i].x,&c[i].y), ++i ) ;
		for ( ans = 0, idx = 0; idx < n; ++idx ) {
			for ( pole = c[idx], m = 0, i = 0; i < n; ++i ) {
				if ( i == idx ) continue ;
				cell a = c[i]-pole; a.ang = (UNIT|a);
				if ( fabs(a.ang-pi/2) < tol || -pi/2 < a.ang && a.ang <= pi/2 )
					p[++m] = a, p[m].dist = (p[m],p[m]);
			}
			if ( m <= 1 ) continue ;
			//printf("idx = %d, m = %d\n",idx,m);
			for ( sort(p+1,p+m+1), i = m; --i>=1; )
				for ( j=i+1; j <= m; ++j )
					for ( z[i][j]=3,k=j+1; k <= m; ++k )
						if ( is_left_turn(p[i],p[j],p[k]) )
							z[i][j] = max(z[i][j],1+z[j][k]);
			for ( i = 1; i <= m; ++i )
				for ( j = i+1; j <= m; ++j )
					ans = max(z[i][j],ans);
		}
		printf("%d\n",ans);
	}
	return 0;
}

