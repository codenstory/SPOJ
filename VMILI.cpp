/*
 * VMILI: Military Story
 * TOPIC: convex hull, gift wra
 * status:
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#define DIM 3
#define tol 1e-13
#define N 4008
using namespace std;

const double pi = 2*acos(0.00);

typedef struct cell {
	double x[DIM], ang, dist;
	cell() { 
		memset(x,0,sizeof x); 
	};
	double operator [] ( const int i ) const {
		return x[i];
	};
	cell( double a, double b ) {
		0[x]=a,1[x]=b,2[x]=0;
	};
} cell;

cell operator - ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM; r.x[i] = a[i]-b[i], ++i ) ;
	return r;
}

cell operator + ( const cell &a, const cell &b ) {
	cell r;
	for ( int i = 0; i < DIM; r.x[i] = a[i]+b[i], ++i ) ;
	return r;
}

double operator , ( const cell &a, const cell &b ) {
	double s = 0;
	for ( int i = 0; i < DIM; s += a[i]*b[i], ++i );
	return s;
}

cell operator * ( const cell &a, const double &t ) {
	cell r;
	for ( int i = 0; i < DIM; r.x[i] = a[i]*t, ++i ) ;
	return r;
}

bool operator < ( const cell &a, const cell &b ) {
	for ( int i = 0; i < DIM; ++i ) 
		if ( fabs(a[i]-b[i]) >= tol )
			return a[i] < b[i];
	return false ;
}

cell operator ^ ( const cell &a, const cell &b ) {
	cell r;
	for ( int j,k,i = 0; i < DIM; ++i )
		k = (((j = ((i+1)%DIM))+1)%DIM), r.x[i] = a[j]*b[k]-a[k]*b[j];
	return r;
}

double operator | ( const cell &a, const cell &b ) {
	double A = sqrt((a,a)), B = sqrt((b,b)),
		   co = (a,b)/A/B, si = (a^b)[DIM-1]/A/B;
	if ( co >= 0 && si >= 0 )
		return acos(co);
	if ( co <= 0 && si >= 0 )
		return acos(co);
	if ( co >= 0 && si <= 0 )
		return asin(si);
	return -pi+acos(-co);
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
	double si = (u^v)[DIM-1], co = (u,v);
	if ( si > 0 ) return true ;
	if ( si < 0 ) return false ;
	assert( fabs(si) < tol ) ;
	assert( co > 0 );
	return false ;
}

cell c[N],pole,ONE(1,0),ZERO,p[N];
int n,m,st[N],*top,len,a[N],h[N],
	used[N],yes;

int main() {
	int i,j,k;
	for ( ;1==scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i ) 
			scanf("%lf %lf",&c[i].x[0],&c[i].x[1]);
		for ( k = 0; n >= 3; ++k ) {
			for ( sort(c,c+n), pole = c[0], i = 0; i < n; c[i] = c[i]-pole, ++i ) ;
			for ( i = 1; i < n; ++i )
				c[i].dist = (c[i],c[i]), c[i].ang = ONE|c[i];
			for ( sort(c+1,c+n,cell_comparator()), m = 0, a[m++] = 0, i = 1; i < n; i = j ) 
				for ( a[m++] = i, j = i+1; j < n && fabs(c[i].ang-c[j].ang) < tol; ++j ) ;
			for ( top = st, *++top = 0, *++top = 1, *++top = 2, i = 3; i < m; *++top = i++ ) 
				for ( ;top-st >= 2 && !left_turn(c[a[*(top-1)]],c[a[*top]],c[a[i]]); --top );
			for ( ++yes, len = 0; top > st; used[h[len++] = a[*top--]] = yes ) ;
			if ( len <= 2 ) {
				++k;
				break ;
			}
			for ( m = 0, i = 0; i < n; ++i )
				if ( used[i] != yes ) a[m++] = i;
			for ( n = 0, i = 0; i < m; p[n++] = c[a[i++]] ) ;
			for ( i = 0; i < n; ++i ) c[i] = p[i];
		}
		printf("%d\n",k);
	}
	return 0;
}

