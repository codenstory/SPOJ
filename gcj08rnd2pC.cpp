/**
 * TOPIC: 3D geometry, binary search, algebra
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x400
#define oo (1<<29)

int n;
double a[N],b[N],c[N],p[N];

template<typename T>
std::pair<T,T> operator * ( const std::pair<T,T> &a, const std::pair<T,T> &b ) {
    if ( a.second < b.first or b.second < a.first )
        return {0,-1};
    if ( a.first > a.second or b.first > b.second )
        return {0,-1};
    if ( a.first <= b.first and b.second <= a.second )
        return b;
    if ( b.first <= a.first and a.second <= b.second )
        return a;
    if ( a.first <= b.first and a.second <= b.second )
        return {b.first,a.second};
    return {a.first,b.second};
}

bool v( const std::pair<double,double> &o ) {
    return o.first <= o.second;
}

bool f( double q ) {
    double A,B,C,D,E,F,G,H;
    std::pair<double,double> ra= {-oo,+oo}, rb= ra, rc= ra, rd= ra;
    for ( int i= 0; i < n; ++i ) {
        A= a[i]+b[i]+c[i]-p[i]*q;
        B= a[i]+b[i]+c[i]+p[i]*q;
        C= a[i]+b[i]-c[i]-p[i]*q;
        D= a[i]+b[i]-c[i]+p[i]*q;
        E= a[i]-b[i]+c[i]-p[i]*q;
        F= a[i]-b[i]+c[i]+p[i]*q;
        G= -a[i]+b[i]+c[i]-p[i]*q;
        H= -a[i]+b[i]+c[i]+p[i]*q;
        std::pair<double,double> pr= {A,B};
        ra= ra*pr;
        pr= {C,D};
        rb= rb*pr;
        pr= {E,F};
        rc= rc*pr;
        pr= {G,H};
        rd= rd*pr;
    }
    return v(ra) and v(rb) and v(rc) and v(rd);
}

int main() {
    int i,j,k,cs= 0,ts;
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    for ( is >> ts; ts--; ) {
        for ( is >> n,  i= 0; i < n; ++i )
            is >> a[i] >> b[i] >> c[i] >> p[i];
        double bad= 0, good= +oo, mid;
        for ( ;fabs(good-bad) >= 1e-7; )
            f(mid= (good+bad)/2)?(good= mid):(bad= mid);
        std::setprecision(6);
        os << "Case #" << ++cs << ": " << std::fixed << good << std::endl;
    }
    return 0;
}

