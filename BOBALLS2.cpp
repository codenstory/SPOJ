/*
 * BOBALLS2
 * TOPIC: maths, simulation
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)
#define DIRS 4
#define N 5007
using i64= std::int64_t;
using location= std::pair<i64,i64>;
const i64 dx[]= {1,-1,-1,1},
          dy[]= {1,1,-1,-1};
using t_dir= int;
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

i64 m,n;
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
    const line &as_line() const {
        assert( line_.X < +oo );
        return line_;
    }

    cell() {
    }

    cell( location loc, t_dir dir ): pos(std::move(loc)), direction(dir) {
        init();
    }

    location at_time( double tau ) const {
        return {pos.first+tau*dx[direction],pos.second+tau*dy[direction]};
    }

    cell next(double &t) const {
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
    return a.direction == b.direction and a.pos == b.pos;
}

template<>
struct std::hash<cell> {
    std::size_t operator() ( const cell &c ) const {
        return std::hash<i64>()(c.pos.first)^std::hash<i64>()(c.pos.second)^std::hash<int>()(c.direction);
    }
};

std::unordered_map<cell,int> cell2id;
std::unordered_map<int,cell> id2cell;

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
std::vector<std::vector<std::pair<int,int>>> adj[2];
std::stack<int> st{};
int scc[DIRS*(N+N)],mark,seen[DIRS*(N+N)],yes;
i64 period[DIRS*(N+N)];

location location_at_( i64 t, int x ) {
    assert( not adj[0][x].empty() );
    auto y= adj[0][x].front().first;
    auto w= adj[0][x].front().second;
    if ( period[scc[x]] )
        t%= period[scc[x]];
    while ( w < t ) {
       t = t - w, x = y, y = adj[0][x].front().first, w = adj[0][x].front().second;
    }
    return id2cell[x].at_time(t);
}

location location_at( i64 t, cell &c ) {
    double tau= 0;
    auto nx= c.next(tau);
    assert( cell2id.count(nx) );
    if ( t <= tau )
        return c.at_time(t);
    return location_at_(t-(int)(tau+1e-9),cell2id[nx]);
}

void answer( i64 t, std::ostream &os ) {
    std::vector<location> res;
    res.reserve(balls.size());
    for ( int i= 0; i < balls.size(); ++i )
        res.push_back(location_at(t,balls[i]));
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

void dfs( int x, int t ) {
    std::stack<std::pair<int,bool>> s{};
    s.push({x,false});
    int y;
    bool done;
    for ( seen[x]= yes; not s.empty(); ) {
        std::tie(y,done)= s.top(); s.pop();
        if ( done ) {
            if ( t )
                st.push(y);
            else scc[y]= mark;
        }
        else {
            if ( adj[t][y].empty() ) continue ;
            if ( seen[adj[t][y].front().first] != yes )
                s.push({y,true}), s.push({adj[t][y].front().first,false}), seen[adj[t][y].front().first]= yes;
        }
    }
}

size_t num_nodes() {
    return cell2id.size();
}

void precalc_graph() {
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
    cell2id.clear(), id2cell.clear();
    std::cerr << "calculating boundary" << std::endl;
    std::cerr << m << " " << n << std::endl;
    for ( i64 x= 0; x <= n; x+= n )
        for ( i64 y= 0; y <= m; ++y )
            for ( t_dir t= 0; t < DIRS; ++t ) {
                if ( x == 0 and y == 0 and (t == 1 or t == 3) )
                    continue ;
                if ( x == 0 and y == m and (t == 0 or t == 2) )
                    continue ;
                if ( x == n and y == m and (t == 1 or t == 3) )
                    continue ;
                if ( x == n and y == 0 and (t == 0 or t == 2) )
                    continue ;
                cell c({x,y},t);
                if ( not cell2id.count(c) ) {
                    auto V= num_nodes();
                    id2cell[cell2id[c]= V]= c;
                }
            }
    std::cerr << "done part 1 calculating boundary" << std::endl;
    for ( i64 y= 0; y <= m; y+= m )
        for ( i64 x= 0; x <= n; ++x )
            for ( t_dir t= 0; t < DIRS; ++t ) {
                if ( x == 0 and y == 0 and (t == 1 or t == 3) )
                    continue ;
                if ( x == 0 and y == m and (t == 0 or t == 2) )
                    continue ;
                if ( x == n and y == m and (t == 1 or t == 3) )
                    continue ;
                if ( x == n and y == 0 and (t == 0 or t == 2) )
                    continue ;
                cell c({x,y},t);
                if ( not cell2id.count(c) ) {
                    auto V= num_nodes();
                    id2cell[cell2id[c]= V]= c;
                }
            }
    std::cerr << "done calculating boundary" << std::endl;
    for ( int t= 0; t <= 1; ++t )
        adj[t].resize(num_nodes(),std::vector<std::pair<int,int>>{});
    for ( auto &[k,v]: cell2id ) {
        double t;
        auto nx= k.next(t);
        assert( cell2id.count(nx) );
        auto fr= v, to= cell2id[nx];
        adj[0][fr].push_back(std::make_pair(to,(int)t));
        adj[1][to].push_back(std::make_pair(fr,(int)t));
    }
    std::cerr << "calculating SCC" << std::endl;
    ++yes;
    for ( int x= 0; x < num_nodes(); ++x )
        if ( seen[x] != yes )
            dfs(x,1);
    mark= -1, ++yes;
    for ( ;not st.empty(); st.pop() )
        if ( seen[st.top()] != yes )
            ++mark, dfs(st.top(),0);
    std::cerr << "done calculating SCC" << std::endl;

    ++yes;
    for ( int x= 0; x < num_nodes(); ++x ) {
        if ( seen[scc[x]] == yes )
            continue ;
        period[scc[x]]= 0;
        seen[scc[x]]= yes;
        if ( scc[adj[0][x].front().first] != scc[x] ) {
            continue ;
        }
        auto &v= period[scc[x]];
        v+= adj[0][x].front().second;
        for ( int y= adj[0][x].front().first, z= x; y != z; ) {
            v+= adj[0][y].front().second, y= adj[0][y].front().first;
        }
    }
    std::cerr << "Computing periods: done" << std::endl;
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
        precalc_graph();
        std::cerr << "Done precalculating graph" << std::endl;
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
        std::vector<i64> reqs;
        is >> qr, reqs.resize(qr);
        for ( auto &v: reqs )
            is >> v;
        answer_requests(reqs,os);
    }
    return 0;
}
