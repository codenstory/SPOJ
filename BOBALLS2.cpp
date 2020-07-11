#include <utility>

/*
 * BOBALLS2
 * TOPIC: maths, simulation
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)

using location= std::pair<int,int>;
const int dx[]= {1,-1,-1,1},
          dy[]= {1,1,-1,-1};
using t_dir= int;
using i64= std::int64_t;

i64 m,n;
std::vector<std::vector<std::pair<int,t_dir>>> ball2wall= {
        {{0,1},{1,3}},//1,3
        {{1,2},{2,0}},//2,0
        {{2,3},{3,1}},//3,1
        {{0,2},{3,0}} //2,0
};
std::vector<std::vector<std::pair<int,t_dir>>> ball2corner= {
        {{0,2}},
        {{1,3}},
        {{2,0}},
        {{3,1}}
};

struct line {
    double x0,y0,X,Y;
};

std::pair<double,double> operator * ( const line &a, const line &b ) {
    double det= a.X*(-b.Y)-a.Y*(-b.X),
           dt= (b.X-a.X)*(-b.Y)-(b.Y-a.Y)*(-b.X),
           dtau= a.X*(b.Y-b.Y)-(b.X-a.X)*a.Y;
    if ( fabs(det) < 1e-7 )
        return {+oo,+oo};
    double t= dt/det, tau= dtau/det;
    return {t,tau};
}

struct cell;
line L[]= {
        {.x0= 0,.y0= 0,.X= (double)n,.Y= 0},
        {.x0= (double)n,.y0= 0,.X= 0,.Y= (double)m},
        {.x0= (double)n,.y0= (double)m,.X= -(double)n, .Y= 0},
        {.x0= 0, .y0= (double)m, .X= 0, .Y= -(double)m}
};

line C[]= {
        {.x0= 0,.y0= 0,.X= 0,.Y= 0},
        {.x0= (double)n,.y0= 0,.X= 0,.Y= 0},
        {.x0= (double)n,.y0= (double)m,.X= 0, .Y= 0},
        {.x0= 0, .y0= (double)m, .X= 0, .Y= 0}
};

double meeting_time( const line &, const line & ) ;

struct cell {
    location pos;
    t_dir direction;
    line line_;
    void init() {
        line_= line{.x0= (double)pos.first,
                .y0= (double)pos.second,
                .X= (double)dx[direction],
                .Y= (double)dy[direction]};
    }
    const line &as_line() {
        return line_;
    }

    cell() {
        init();
    }

    cell( location loc, t_dir dir ): pos(std::move(loc)), direction(dir) {
        init();
    }

    location at_time( double tau ) {
        return {pos.first+tau*dx[direction],pos.second+tau*dy[direction]};
    }

    cell next() {
        double soonest= +oo;
        t_dir new_dir= -1;
        for ( const auto &vec: ball2wall[direction] ) {
            double tau= meeting_time(as_line(),L[vec.first]);
            if ( tau < +oo )
                if ( tau < soonest )
                    soonest= tau, new_dir= vec.second;
        }
        for ( const auto &vec: ball2corner[direction] ) {
            double tau= meeting_time(as_line(),C[vec.first]);
            if ( tau < +oo )
                if ( tau < soonest ) {
                    soonest= tau, new_dir= vec.second;
                }
        }
        assert( soonest < +oo );
        assert( new_dir != -1 );
        return cell(at_time(soonest),new_dir);
    }
};

double meeting_time( const line &k, const line &r ) {
    double f,s;
    std::tie(f,s)= (k*r);
    assert( f >= 0 );
    if ( 0 <= s and s <= 1 )
        return f;
    return +oo;
}

std::vector<cell> balls;
std::vector<double> reqs;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( int numballs, qr; (is >> n >> m); ) {
        is >> numballs, balls.resize(numballs);
        for ( auto &v: balls ) {
            is >> v.pos.first >> v.pos.second;
            int xx,yy;
            is >> xx >> yy;
            for ( int i= 0; i < 4; ++i )
                if ( xx == dx[i] and yy == dy[i] )
                    v.direction= i;
            v.init();
        }
        is >> qr, reqs.resize(qr);
        for ( auto &v: reqs )
            is >> v;
    }
    return 0;
}
    