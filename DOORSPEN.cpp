/*
 * DOORSPEN: Doors and Penguins
 * TOPIC: convex hull
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#define N 0x800
#define tol 1e-13
typedef long long i64;
using namespace std;

const double pi = 2*acos(0.00);

typedef struct cell {
	i64 x,y,dist;
	double ang;
	cell() { x=y=0; }
	cell( i64 x, i64 y ) {
		this->x = x, this->y = y;
	}
} cell;

bool operator < ( const cell &a, const cell &b ) {
	if ( fabs(a.x-b.x) < tol )
		return a.y<b.y;
	return a.x<b.x;
}

cell operator - ( const cell &a, const cell &b ) {
	return cell(a.x-b.x,a.y-b.y);
}

cell operator + ( const cell &a, const cell &b ) {
	return cell(a.x+b.x,a.y+b.y);
}

i64 operator , ( const cell &a, const cell &b ) {
	return a.x*b.x+a.y*b.y;
}

i64 operator ^ ( const cell &a, const cell &b ) {
	return a.x*b.y-a.y*b.x;
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

struct cell_comp {
	bool operator () ( const cell &a, const cell &b ) {
		if ( fabs(a.ang-b.ang) < tol )
			return a.dist > b.dist;
		return a.ang < b.ang;
	}
};

bool left_turn( const cell &p, const cell &q, const cell &r ) {
	cell u = q-p, v = r-p;
	i64 co = (u,v), si = (u^v);
	if ( si < 0 ) return false ;
	if ( si > 0 ) return true ;
	assert( co >= 0 );
	return false ;
}

enum { D, P };
cell c[N],pole,UNIT(1,0),p[N],ZERO,a[2][N];
int n[2],m,*top,st[N];

bool on_segment( const cell &p, const cell &a, const cell &b ) {
	cell u = a-p, v = b-p;
	return (u,v) <= 0 && (u^v) == 0;
}

bool inside( const cell &p, const cell c[], const int n ) {
	assert( n >= 2 );
	if ( n == 2 ) 
		return on_segment(p,c[0],c[1]);
	assert( n >= 3 );
	double w = 0;
	for ( int i = 0; i < n; ++i ) {
		if ( on_segment(p,c[i],c[(i+1)%n]) )
			return true ;
		w += (c[i]-p)|(c[(i+1)%n]-p);
	}
	return fabs(w-2*pi) < tol || fabs(w+2*pi) < tol;
}

bool f( const cell &p, const cell &q, const cell &a, const cell &b ) {
	double dt, dtau, t, tau, det,
		   A = q.x-p.x, B = -(b.x-a.x),
		   C = q.y-p.y, D = -(b.y-a.y),
		   Q = a.x-p.x,
		   R = a.y-p.y;
	det = (A*D-B*C);
	if ( fabs(det) < tol ) {
		if ( on_segment(p,a,b) || on_segment(q,a,b) || on_segment(a,p,q) || on_segment(b,p,q) )
			return true ;
		return false ;
	}
	else {
		dt = Q*D-R*B, dtau = A*R-Q*C;
		t = dt/det, tau = dtau/det;
		return 0 <= t && t <= 1 && 0 <= tau && tau <= 1;
	}
}

int main() {
	int i,j,k,t,cs = 0,ok;
	i64 x,y,nx,ny;
	for ( ;2 == scanf("%d %d",n+D,n+P) && (n[D]||n[P]); ) {
		if ( cs >= 1 ) putchar('\n');
		printf("Case %d: ",++cs), ok = 1;
		for ( t = D; t <= P; ++t ) {
			for ( n[t] <<= 2, i = 0; i < n[t]; i += 4 ) {
				scanf("%lld %lld %lld %lld",&x,&y,&nx,&ny);
				c[i].x = x, c[i].y = y, c[i+1].x = x, c[i+1].y = ny, c[i+2].x = nx, c[i+2].y = y, c[i+3].x = nx, c[i+3].y = ny;
			}
			for ( sort(c,c+n[t]), pole = c[0], i = 1; i < n[t]; ++i )
				c[i] = c[i]-pole, c[i].ang = (UNIT|c[i]), c[i].dist = (c[i],c[i]);
			for ( m = 0, p[m++] = ZERO, sort(c+1,c+n[t],cell_comp()), i = 1; i < n[t]; i = j ) 
				for ( p[m++] = c[i], j = i+1; j < n[t] && fabs(c[i].ang-c[j].ang) < tol; ++j ) ;
			if ( m <= 2 ) {
				for ( n[t] = 0, i = 0; i < m; a[t][n[t]++] = p[i++]+pole ) ;
				continue ;
			}
			assert( m >= 3 );
			for ( top = st, *++top = 0, *++top = 1, *++top = 2, i = 3; i < m; *++top = i++ )
				for ( ;top-st>=2 && !left_turn(p[*(top-1)],p[*top],p[i]); --top ) ;
			for ( n[t] = 0; top > st; a[t][n[t]++] = p[*top--]+pole ) ;
		}
		assert( n[D] >= 1 && n[P] >= 1 );
		if ( n[D] >= 2 && n[P] >= 2 ) {
				/*
			for ( t = D; t <= P; ++t ) {
				printf("n = %d\n",n[t]);
				for ( i = 0; i < n[t]; ++i )
					printf("(%lld,%lld) ",a[t][i].x,a[t][i].y);
				puts("");
			}
			*/
			for ( i = 0; i < n[D] && ok; ++i )
				for ( j = 0; j < n[P] && ok; ++j )
					if ( f(a[D][i],a[D][(i+1)%n[D]],a[P][j],a[P][(j+1)%n[P]]) )
						ok = 0;
			for ( t = D; t <= P && ok; ++t )
				for ( i = 0; i < n[t] && ok; ++i )
					if ( inside(a[t][i],a[t^1],n[t^1]) )
						ok = 0;
		}
		else {
			if ( n[D] == 1 ) {
				if ( inside(a[D][0],a[P],n[P]) ) ok = 0;
			}
			else {
				assert( n[P] == 1 );
				if ( inside(a[P][0],a[D],n[D]) ) ok = 0;
			}
		}
		puts(ok?"It is possible to separate the two groups of vendors.":"It is not possible to separate the two groups of vendors.");
	}
	return 0;
}

