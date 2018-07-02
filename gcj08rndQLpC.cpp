/*
 * TOPIC: geometry, probablity
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#define tol (1e-13)
#define pi (2*acos(0.00))
using namespace std;

class Solution {
	double f,R,t,r,g;
	inline double S( double x ) { return x*x; }
	inline double integral( double x0, double y0, double R, double l, double r ) {
		double phi0= 2*asin((l+x0)/R),
			   phi1= 2*asin((r+x0)/R);
		return S(R)/4*(phi1-phi0) + S(R)/4*(sin(phi1)-sin(phi0)) - (r-l)*y0;
	}
	double intersection_area( double x0, double y0, double R, double g ) {
		double d,x,y,l,r;

		assert( x0 <= R && y0 <= R );
		if ( S(x0) + S(y0) >= S(R) )
			return 0.00;

		if ( S(x0+g)+S(y0+g) <= S(R) )
			return g*g;

		y= sqrt(S(R)-S(x0))-y0;
		if ( 0 <= y && y <= g ) {
			l= min(g,sqrt(S(R)-S(y0))-x0);
			return integral(x0,y0,R,0,l);
		}

		x= sqrt(S(R)-S(y0+g))-x0;
		if ( 0 <= x && x <= g ) {
			l= x, r= min(g,sqrt(S(R)-S(y0))-x0);
			return l*g + integral(x0,y0,R,l,r);
		}
		assert( 0 );
		return g*g;
	}
public:
	void read_input() {
		cin >> f >> R >> t >> r >> g;
	}
	Solution() {};
	double solve() {
		int i,j,k;
		double x0,y0,ans= 0;
		for ( i= 0; (x0= r+2*r*i+i*g+f) <= R-t-f; ++i )
			for ( j= 0; (y0= r+2*r*j+j*g+f) <= R-t-f; ++j ) 
				ans+= intersection_area(x0,y0,max(0.00,R-t-f),max(0.00,g-2*f));
		return ans/(pi*S(R)/4);
	}
};

Solution sol;

int main() {
	int ts,cs =0;
	for ( scanf("%d",&ts); ts--; sol.read_input(), printf("Case #%d: %.6lf\n",++cs,1-sol.solve()) ) ;
	return 0;
}

