/**
 *  GOALFR
 *  TOPIC: geometry, special cases, circle-segment intersection
 *  status: Accepted
 */
#include <bits/stdc++.h>
#define tol (1e-7)

std::istream &is = std::cin;
std::ostream &os = std::cout;

bool is_solvable( double a, double b, double c, double L, double R ) {
    if ( fabs(a) < tol ) {
        if ( fabs(b) < tol ) {
            return fabs(c) < tol;
        }
        double x= -c/b;
        return L <= x and x <= R;
    } else {
        double D= b*b-4*a*c;
        if ( D < 0 )
            return false ;
        double x0= (-b-sqrt(D))/2/a,
               x1= (-b+sqrt(D))/2/a;
        return (L<= x0 and x0 <= R) or (L<=x1 and x1 <= R);
    }
}

bool segment_intersection( double x0, double y0, double x1, double y1, double gx, double gy, double r ) {
    double m= x1-x0, n= y1-y0, p= gx-x0, q= gy-y0;
    double a= m*m+n*n, b= -2*(m*p+n*q), c= p*p+q*q-r*r;
    return is_solvable(a,b,c,0.00,1.00);
}

bool inside( double x, double y, double gx, double gy, double r ) {
    return (gx-x)*(gx-x) + (gy-y)*(gy-y) <= r*r;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts;
    for ( is >> ts; ts--; ) {
        double x,y,gx,gy,r;
        is >> x >> y >> gx >> gy >> r;
        double gpx0= 52.5, gpx1= 52.5, gpy0= -3.66, gpy1= 3.66;
        if ( inside(x,y,gx,gy,r) ) {
            os << "No goal...\n";
            continue ;
        }
        if ( inside(gpx0,gpy0,gx,gy,r) and inside(gpx1,gpy1,gx,gy,r) ) {
            os << "No goal...\n";
            continue ;
        }
        if ( segment_intersection(x,y,gpx0,gpy0,gx,gy,r) and segment_intersection(x,y,gpx1,gpy1,gx,gy,r) )
            os << "No goal...\n";
        else os << "Goal!\n";
    }
    return 0;
}
    