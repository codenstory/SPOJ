/**
 * WIJT
 * TOPIC: binary search, bfs
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x400
#define tol (1e-9)

std::istream &is= std::cin;
std::ostream &os= std::cout;

int n;
std::vector<std::vector<int>> segments;
double g[N][N];

double G( double x, double y, const std::vector<int> &c ) {
    double a,b,na,nb;
    for ( a= 0, b= 1; fabs(b-a) >= tol; ) {
        na= (2*a+b)/3, nb= (2*b+a)/3;
        auto xa= c[0]+na*(c[2]-c[0]);
        auto ya= c[1]+na*(c[3]-c[1]);
        auto xb= c[0]+nb*(c[2]-c[0]);
        auto yb= c[1]+nb*(c[3]-c[1]);
        if ( (x-xa)*(x-xa)+(y-ya)*(y-ya) < (x-xb)*(x-xb)+(y-yb)*(y-yb) )
            b= nb;
        else a= na;
    }
    auto xa= c[0]+a*(c[2]-c[0]);
    auto ya= c[1]+a*(c[3]-c[1]);
    return (x-xa)*(x-xa)+(y-ya)*(y-ya);
}

double f( const std::vector<int> &a, const std::vector<int> &b ) {
    double x0=  a[0], x1=  a[2], y0=  a[1], y1=  a[3];
    double x0p= b[0], x1p= b[2], y0p= b[1], y1p= b[3];
    double a11= (x1-x0), a12= -(x1p-x0p);
    double a21= (y1-y0), a22= -(y1p-y0p);
    double b1= x0p-x0, b2= y0p-y0;
    double det= a11*a22-a12*a21;
    if ( fabs(det) <= tol ) {
        return std::sqrt(std::min(std::min(G(x0,y0,b),G(x1,y1,b)),std::min(G(x0p,y0p,a),G(x1p,y1p,a))));
    } else {
        auto t=  (a22*b1-b2*a12)/det;
        auto tp= (a11*b2-b1*a21)/det;
        if ( 0 <= t and t <= 1 and 0 <= tp and tp <= 1 ) {
            return 0.00;
        } else {
            return std::sqrt(std::min(std::min(G(x0,y0,b),G(x1,y1,b)),std::min(G(x0p,y0p,a),G(x1p,y1p,a))));
        }
    }
}

int seen[N],yes;

bool connected( double limit ) {
    std::queue<int> q;
    int x,y;
    for ( seen[0]= ++yes, q.push(0); (not q.empty()) and seen[1] != yes; )
        for ( x= q.front(), q.pop(), y= 0; y < n; ++y )
            if ( g[x][y] <= limit and seen[y] != yes )
                seen[y]= yes, q.push(y);
    return seen[1] == yes;
}

int main() {
    int i,j,k,t;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for (;(is>>n) and n;) {
        segments.clear(), segments.resize(n);
        for (i = 0; i < n; ++i) {
            is >> j >> k >> t;
            if (t > 0)
                segments[i] = std::vector<int>{j, k, j + t, k};
            else
                segments[i] = std::vector<int>{j, k, j, k - t};
        }
        for (i = 0; i < n; ++i)
            for (j = i + 1; j < n; ++j) {
                g[j][i] = g[i][j] = f(segments[i], segments[j]);
            }
        double good = 2e9, bad = 0, mid;
        if (connected(bad)) {
            os << std::setprecision(2) << std::fixed << bad << std::endl;
        } else {
            for (; fabs(good - bad) >= tol;) {
                mid = (good + bad) / 2;
                if (connected(mid))
                    good = mid;
                else bad = mid;
            }
            assert(connected(good));
            os << std::setprecision(2) << std::fixed << good << std::endl;
        }
    }
}
