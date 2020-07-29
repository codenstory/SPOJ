/**
 * BOBALLS2
 * TOPIC: simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { NE= 0, NW= 1, SW= 2, SE= 3 };
#define MAXV (8*(5000+5000)+7)

int dx[]= {1,-1,-1,1},
    dy[]= {1,1,-1,-1};

using t_node= std::tuple<int, int, int>;
using t_weight= i64;
using t_int= int;
using t_edge= std::pair<t_int, t_weight>;
using t_aika= i64;
using t_pos= int;

bool operator == ( const t_node &a, const t_node &b ) {
    return a == b;
}

template<>
struct std::hash<t_node> {
    size_t operator() ( const t_node &c ) const {
        auto x= std::get<0>(c), y= std::get<1>(c), t= std::get<2>(c);
        return std::hash<int>()(x)^std::hash<int>()(y)^std::hash<int>()(t);
    }
};

class Solution {

    std::unordered_map<t_node, t_int> node2id;
    t_node inv_map[MAXV];
    t_int head[MAXV];
    t_edge adj[MAXV];
    t_aika period[MAXV], distance_to[MAXV];
    std::vector<int> seen;
    int m,n,yes;

    t_int insert( t_node x ) {
        if ( node2id.count(x) )
            return node2id[x];
        auto z= node2id.size();
        assert( z < 8*(m+n)+7 );
        assert( 0 <= std::get<2>(x) and std::get<2>(x) < 4 );
        inv_map[z]= x;
        return node2id[x]= z;
    }

#define vc(x,y) (0 <= (x) and (x) <= m and 0 <= (y) and (y) <= n)

    std::pair<t_node,t_aika> next_( const t_node &v ) {
        int x= std::get<0>(v),
            y= std::get<1>(v),
            t= std::get<2>(v);
        if ( not vc(x+dx[t],y+dy[t]) ) {
            if ( (x == m and y == 0) or (x == m and y == n) or (x == 0 and y == n) or (x == 0 and y == 0) )
                return std::make_pair(std::make_tuple(x,y,(t+2)&3),0);
            int nt= (t+1)&3;
            for ( ;not(nt != t and nt != ((t+2)&3) and vc(x+dx[nt],y+dy[nt])); ++nt, nt&= 3 ) ;
            assert( nt != t );
            //std::cerr << "The next for (" << x << "," << y << "), t= " << t << " is ";
            //std::cerr << "(" << x << "," << y << "), t= " << nt << std::endl;
            auto next_node= std::make_tuple(x,y,nt);
            assert( node2id.count(next_node) );
            return std::make_pair(next_node,0ll);
        } else {
            t_aika good= 0, bad= std::max(m,n)+0x80, mid;
            assert( not vc(x+dx[t]*bad,y+dy[t]*bad) );
            for ( ;good+1 != bad; ) {
                mid= (good+bad)>>1;
                auto nx= x+dx[t]*mid, ny= y+dy[t]*mid;
                vc(nx,ny)?(good= mid):(bad= mid);
            }
            assert( good >= 1 );
            auto next_node= std::make_tuple(x+dx[t]*good, y+dy[t]*good, t);
            assert( node2id.count(next_node) );
            return std::make_pair(next_node,good);
        }
    }

    void build_graph() {
        for ( auto &[key,val]: node2id ) {
            auto nx= next_(key);
            adj[val]= {node2id[nx.first],nx.second};
        }
    }

    t_int nxt( t_int x ) {
        return adj[x].first;
    }

    i64 gcd( i64 x, i64 y ) {
        return not y?x:gcd(y,x%y);
    }

    void preprocess_() {
        seen.resize(8*(n+m)+7), std::fill(seen.begin(),seen.end(),0), yes= 1;
        for ( auto &[key,val]: node2id ) {
            auto x= val, y= val;
            if ( seen[x] == yes ) continue ;
            std::vector<t_int> orbit;
            seen[x]= yes, distance_to[x]= 0, orbit.push_back(x);
            for ( y= nxt(x); y != x and seen[y] != yes; ) {
                seen[y]= yes, orbit.push_back(y), y= nxt(y);
            }
            assert( y == x );
            for ( distance_to[y= nxt(x)]= period[x]= adj[x].second; y != x; )
                period[x]+= adj[y].second, y= nxt(y), distance_to[y]= period[x];
            for ( auto z: orbit )
                head[z]= x;
            //assert( period[x] == (2*m)*(2*n)/gcd(2*m,2*n) );
        }
    }

	std::pair<i64,i64> location_at_iterative( t_int v, t_aika t ) {
		auto curr= v;
		for (;t >= adj[curr].second or adj[curr].second == 0;
				t-= adj[curr].second, curr= adj[curr].first ) ;
		auto tr= inv_map[curr];
		int direction= std::get<2>(tr);
		return {std::get<0>(tr)+t*dx[direction],std::get<1>(tr)+t*dy[direction]};
	}

    std::pair<i64,i64> location_at( t_int v, t_aika t ) {
        auto x= head[v];
        if ( t <= period[x]-distance_to[v] ) {
            return location_at_iterative(v,t);
        }
        t-= (period[x]-distance_to[v]);
        assert( t > 0 );
        t%= period[x];
        return location_at_iterative(x,t);
    }

public:

    Solution( int m, int n ): m(m), n(n) {
        int i,j;
        node2id.clear();
        assert( m >= 1 and  n >= 1 );
        for ( j= 0; j <= n; j+= n )
            for ( i= 1; i <= m-1; ++i ) {
                for ( int t= NE; t <= SE; ++t ) 
                    insert(std::make_tuple(i,j,t));
            }
        for ( i= 0; i <= m; i+= m )
            for ( j= 1; j <= n-1; ++j ) {
                for ( int t= NE; t <= SE; ++t )
                    insert(std::make_tuple(i,j,t));
            }

        insert(std::make_tuple(0,0,NE));
        insert(std::make_tuple(0,0,SW));
        insert(std::make_tuple(m,n,NE));
        insert(std::make_tuple(m,n,SW));

        insert(std::make_tuple(m,0,SE));
        insert(std::make_tuple(m,0,NW));
        insert(std::make_tuple(0,n,SE));
        insert(std::make_tuple(0,n,NW));

        //std::cerr << "Building graph: start" << std::endl;
        build_graph();
        //std::cerr << "Building graph: done" << std::endl;
        preprocess_();
        //std::cerr << "Preprocess: done" << std::endl;
    }

    std::pair<i64,i64> get_location_at( std::pair<i64,i64> pos, int direction, t_aika tick ) {
        t_node v= std::make_tuple(pos.first,pos.second,direction);
        int x= std::get<0>(v), y= std::get<1>(v), t= std::get<2>(v);
        t_aika good= 0, bad= std::max(m,n)+0x80, mid;
		assert( not vc(x+dx[t]*bad, y+dy[t]*bad) );
        for ( ;good+1 != bad; ) {
            mid= (good+bad)>>1;
            vc(x+dx[t]*mid,y+dy[t]*mid)?(good= mid):(bad= mid);
        }
        if ( tick <= good ) {
            return {x+dx[t]*good,y+dy[t]*good};
        }
        t_node hitting_point= std::make_tuple(x+dx[t]*good,y+dy[t]*good,t);
        assert( node2id.count(hitting_point) );
        tick -= good;
        assert( tick >= 0 );
        return location_at(node2id[hitting_point],tick);
    }
};

i64 modulo( i64 x, i64 m ) {
    if ( x >= 0 )
        return (x%m);
    auto A= ((-x)/m)*m+m;
    for ( x+= A; x < 0; x += A ) ;
    return x%m;
}

std::pair<int,int> balls[3001];
int dxx[3001], dyy[3001];
std::pair<i64,i64> res[3001];

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    for ( int ts= 0, m,n,numballs, qr; is >> m >> n >> numballs; ) {
        if ( ++ts > 1 )
            os << '\n';
        //Solution s(m,n);
        int blocks= 0;
        for ( int i= 0; i < numballs; ++i ) {
            is >> balls[i].first >> balls[i].second >> dxx[i] >> dyy[i];
        }
        for ( is >> qr; qr--; ) {
            if ( ++blocks > 1 )
                os << '\n';
            t_aika time_tick;
            is >> time_tick;
            for ( int i= 0; i < numballs; ++i ) {
                //res[i]= s.get_location_at(std::make_pair(std::get<0>(balls[i]),std::get<1>(balls[i])),std::get<2>(balls[i]),time_tick);
                res[i]= std::make_pair(
                        modulo(balls[i].first+dxx[i]*time_tick,2*m), modulo(balls[i].second+dyy[i]*time_tick,2*n)
                );
                if ( res[i].first > m )
                    res[i].first= 2*m-res[i].first;
                if ( res[i].second > n )
                    res[i].second= 2*n-res[i].second;
            }
            std::sort(res,res+numballs);
            for ( int i= 0; i < numballs; ++i )
                os << res[i].first << ' ' << res[i].second << '\n';
        }
        break ;
    }
    return 0;
}
    
