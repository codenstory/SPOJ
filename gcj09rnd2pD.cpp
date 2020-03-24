/**
 * TOPIC: binary search, 2D geometry
 * status: WA
 */
#include <bits/stdc++.h>
#define N 0x40
#define tol (1e-7)
#define oo (1<<29)

int n;
std::vector<std::pair<double,double>> p;
std::vector<double> r;
std::vector<std::pair<double,double>> common[N][N];

struct circle {
    std::pair<double,double> c;
    double r;
};

bool operator == ( const circle &A, const circle &B ) {
    return A.c == B.c and A.r == B.r;
}

double angle( std::pair<double,double> A, std::pair<double,double> B ) {
    double a= hypot(A.first,A.second), b= hypot(B.first,B.second);
    double co= (A.first*B.first+A.second*B.second)/a/b;
    double si= (A.first*B.second-A.second*B.first)/a/b;
    if ( co >= 0 and si >= 0 )
        return acos(co);
    if ( co >= 0 and si <= 0 )
        return asin(si);
    if ( co <= 0 and si >= 0 )
        return acos(co);
    return -M_PI+acos(-co);
}

using point= std::pair<double,double>;

std::vector<std::pair<double,double>> operator * ( circle A, circle B ) {
    double d;
    if ( (d= hypot(A.c.first-B.c.first,A.c.second-B.c.second)) > A.r+B.r )
        return {};
    if ( d+A.r < B.r or d+B.r < A.r )
        return {};
    if ( A == B ) {
        return {{A.c.first-A.r,A.c.second},{A.c.first+A.r,A.c.second}};
    }
    auto origin= B.c;
    A.c.first-= origin.first, A.c.second-= origin.second;
    B.c= {0,0};
    double ang= -angle({1,0},A.c);
    double co= cos(ang), si= sin(ang);
    point AA= {A.c.first*co-A.c.second*si,A.c.first*si+A.c.second*co};
    assert( fabs(AA.second) < tol );
    double x= (B.r*B.r-A.r*A.r+AA.first*AA.first)/2/AA.first;
    double y1= -sqrt(B.r*B.r-x*x), y2= sqrt(B.r*B.r-x*x);
    point E1= {x,y1}, E2= {x,y2};
    E1= {E1.first*co+E1.second*si, -E1.first*si+E1.second*co};
    E2= {E2.first*co+E2.second*si, -E2.first*si+E2.second*co};
    E1.first+= origin.first, E1.second+= origin.second;
    E2.first+= origin.first, E2.second+= origin.second;
    return {E1,E2};
}

std::vector<std::pair<double,double>> find_circle( double M, int i, int j ) {
    if ( M < r[i] or M < r[j] )
        return {};
    circle A{p[i],M-r[i]}, B{p[j],M-r[j]};
    return A*B;
}

bool covers( double M, std::pair<double,double> c, std::pair<double,double> P, double R ) {
    return M >= hypot(c.first-P.first,c.second-P.second)+R;
}

bool covers_all( double M, std::pair<double,double> a, std::pair<double,double> b ) {
    for ( int i= 0; i < n; ++i )
        if ( not covers(M,a,p[i],r[i]) and not covers(M,b,p[i],r[i]) )
            return false ;
    return true ;
}

bool f( double R ) {
    int i,j,k;
    if ( n == 1 )
        return R >= r[0];
    if ( n == 2 )
        return R >= r[0] and R >= r[1];
    assert( n >= 3 );
    for ( i= 0; i < n; ++i )
        for ( j= i+1; j < n; ++j )
            common[j][i]= common[i][j]= find_circle(R,i,j);
    for ( i= 0; i < n; ++i )
        for ( j= i+1; j < n; ++j )
            for ( auto cc: common[i][j] )
                for ( k= 0; k < n; ++k )
                    if ( covers_all(R,cc,p[k]) )
                        return true ;
    for ( i= 0; i < n; ++i )
        for ( j= i+1; j < n; ++j )
            for ( auto cc: common[i][j] )
                for ( int l= 0; l < n; ++l )
                    for ( int t= l+1; t < n; ++t )
                        for ( auto zz: common[l][t] )
                            if ( not (cc == zz) )
                                if ( covers_all(R,cc,zz) )
                                    return true ;
    for ( i= 0; i < n; ++i )
        for ( j= 0; j < n; ++j )
            if ( covers_all(R,p[i],p[j]) )
                return true ;
    return false ;
}

int main() {
    int i,cs= 0,ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n, p.resize(n), r.resize(n);
        for ( i= 0; i < n; ++i )
            is >> p[i].first >> p[i].second >> r[i];
        double good= +oo, bad= 0.00, mid;
        for ( ;fabs(good-bad) >= tol; )
            if ( f(mid= (good+bad)/2) )
                good= mid;
            else bad= mid;
        std::setprecision(6);
        os << "Case #" << ++cs << ": " << std::fixed << good << std::endl;
    }
    return 0;
}   
 