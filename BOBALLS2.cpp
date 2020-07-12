/*
 * BOBALLS2
 * TOPIC: maths, simulation
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)

using i64= std::int64_t;
using location= std::pair<i64,i64>;
const i64 dx[]= {1,-1,-1,1},
          dy[]= {1,1,-1,-1};
using t_dir= int;

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
    i64 x0,y0,X,Y;
    line() {}
    line( i64 x0, i64 y0, i64 X, i64 Y ): x0(x0), y0(y0), X(X), Y(Y) {}
};

std::pair<double,double> operator * ( const line &a, const line &b ) {
    double det= a.X*(-b.Y)-a.Y*(-b.X),
           dt= (b.x0-a.x0)*(-b.Y)-(b.y0-a.y0)*(-b.X),
           dtau= a.X*(b.y0-a.y0)-(b.x0-a.x0)*a.Y;
    if ( fabs(det) < 1e-7 ) {
        return {+oo, +oo};
    }
    double t= dt/det, tau= dtau/det;
    return {t,tau};
}

struct cell;
std::vector<line> L,C;

double meeting_time( const line &, const line & ) ;
double hitting_time( const line &, const line & ) ;

struct cell {
    location pos;
    t_dir direction= 0;
    line line_{+oo,+oo,+oo,+oo};
    void init() {
        line_= line(pos.first,
                pos.second,
                dx[direction],
                dy[direction]);
    }
    const line &as_line() {
        assert( line_.X < +oo );
        init();
        return line_;
    }

    cell() {
    }

    cell( location loc, t_dir dir ): pos(std::move(loc)), direction(dir) {
        init();
    }

    location at_time( double tau ) {
        return {pos.first+tau*dx[direction],pos.second+tau*dy[direction]};
    }

    cell next(double &t) {
        double soonest= +oo;
        t_dir new_dir= -1;
        assert( 0 <= direction and direction <= 3 );
        for ( const auto &vec: ball2wall[direction] ) {
            double tau= meeting_time(as_line(),L[vec.first]);
            if ( tau < soonest )
                soonest= tau, new_dir= vec.second;
        }
        for ( const auto &vec: ball2corner[direction] ) {
            double tau= hitting_time(as_line(),C[vec.first]);
            if ( tau <= soonest )
               soonest= tau, new_dir= vec.second;
        }
        assert( soonest < +oo );
        assert( new_dir != -1 );
        return cell(at_time(t= soonest),new_dir);
    }
};

bool operator < ( const cell &a, const cell &b ) {
    if ( a.direction != b.direction )
        return a.direction < b.direction;
    return a.pos < b.pos;
}

bool operator == ( const cell &a, const cell &b ) {
    return not(a < b) and not(b < a);
}

std::vector<std::vector<std::pair<i64,cell>>> trajectory, pre, periodic;
std::vector<i64> preperiod, period;

double meeting_time( const line &k, const line &r ) {
    double f,s;
    std::tie(f,s)= (k*r);
    assert( f >= 0 );
    if ( 0 <= s and s <= 1 )
        return f;
    return +oo;
}

double hitting_time( const line &k, const line &r ) {
    double tx= (r.x0-k.x0+0.00)/k.X,
           ty= (r.y0-k.y0+0.00)/k.Y;
    if ( fabs(tx-ty) < 1e-7 and tx > 0 )
        return tx;
    return +oo;
}

std::vector<cell> balls;

location location_at( i64 t, int idx ) {

    if ( t <= preperiod[idx] ) {
        auto it= std::lower_bound(pre[idx].begin(),pre[idx].end(),std::make_pair(t,cell({+oo,+oo},3)));
        if ( it->first == t ) {
            return it->second.pos;
        } else {
            assert( it != pre[idx].begin() );
            assert( it->first > t );
            it--;
        }
        return it->second.at_time(t-it->first);
    }

    i64 rem= (t-preperiod[idx])%period[idx];
    auto it= std::lower_bound(periodic[idx].begin(),periodic[idx].end(),std::make_pair(rem,cell({+oo,+oo},3)));
    assert( it != periodic[idx].end() );
    if ( it->first == rem ) {
        return it->second.pos;
    } else {
        assert( it != periodic[idx].begin() );
        assert( it->first > rem );
        it--;
    }
    return it->second.at_time(rem-it->first);
}

void answer( i64 t, std::ostream &os ) {
    std::vector<location> res;
    res.reserve(balls.size());
    for ( int i= 0; i < balls.size(); ++i )
        res.push_back(location_at(t,i));
    std::sort(res.begin(),res.end());
    for ( auto &v: res )
        os << (int)(v.first) << ' ' << (int)(v.second) << '\n';
}

void answer_requests( const std::vector<i64> &reqs, std::ostream &os ) {
    for ( int i= 0; i < reqs.size(); ++i ) {
        if ( i ) os << '\n';
        answer(reqs[i],os);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( int numballs, qr; is >> n >> m; ) {
        if ( not(is >> numballs) )
            break ;
        L= {
            line(n,0,0,m),
            line(n,m,-n,0),
            line(0,m,0,-m),
            line(0,0,n,0)
        };
        C= {
            line(n,m,0,0),
            line(0,m,0,0),
            line(0,0,0,0),
            line(n,0,0,0)
        };
        balls.resize(numballs);
        for ( auto &v: balls ) {
            is >> v.pos.first >> v.pos.second;
            int xx,yy;
            is >> xx >> yy, v.direction= -1;
            for ( int i= 0; i < 4; ++i )
                if (xx == dx[i] and yy == dy[i]) {
                    v.direction = i;
                    break ;
                }
            assert( v.direction != -1 );
            assert( dx[v.direction] == xx and dy[v.direction] == yy );
            v.init();
        }
        trajectory.resize(numballs);
        period.resize(numballs),  preperiod.resize(numballs);
        periodic.resize(numballs), pre.resize(numballs);
        for ( int j,i= 0; i < numballs; ++i ) {
            trajectory[i].clear();
            std::map<cell,i64> seen{};
            auto &v= trajectory[i];
            double t= 0,tau;
            auto curr= balls[i];
            v.emplace_back((i64)t,curr), seen[curr]= (i64)t;
            for (;;) {
                auto nc= curr.next(tau);
                v.emplace_back((i64)(t+= tau),nc);
                if ( seen.count(curr= nc) )
                    break ;
                seen[curr]= (i64)t;
            }
            preperiod[i]= seen[curr], period[i]= (i64)t-preperiod[i];
            assert( not trajectory[i].empty() );
            pre[i].clear(), periodic[i].clear();
            pre[i].push_back(trajectory[i].front());
            for ( j= 1; j < trajectory[i].size() and not(trajectory[i][j].second == curr); ++j )
                pre[i].push_back(trajectory[i][j]);
            pre[i].push_back(trajectory[i][j++]);
            for ( --j; j < trajectory[i].size(); ++j )
                periodic[i].push_back({trajectory[i][j].first-preperiod[i],trajectory[i][j].second});
            assert( not pre[i].empty() );
            assert( not periodic[i].empty() );
            assert( periodic[i].back().first == period[i] );
            std::cerr << "[" << n << "," << m << "] ";
            std::cerr << balls[i].pos.first << ", " << balls[i].pos.second << ": " << period[i] << std::endl;
        }
        std::vector<i64> reqs;
        is >> qr, reqs.resize(qr);
        for ( auto &v: reqs )
            is >> v;
        answer_requests(reqs,os);
    }
    return 0;
}
