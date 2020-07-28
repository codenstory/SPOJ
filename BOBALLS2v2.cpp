/**
 * BOBALLS2
 * TOPIC: simulation
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { NE= 0, NW= 1, SW= 2, SE= 3 };

int dx[]= {1,-1,-1,1},
    dy[]= {1,1,-1,-1};

using t_node= std::tuple<int, int, int>;
using t_weight= i64;
using t_int= int;
using t_edge= std::pair<t_int,t_weight>;
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
    std::map<t_int,std::set<t_edge>> adj;
    std::unordered_map<t_int,i64> period, distance_to;
    std::map<t_int,t_int> head;
    std::unordered_map<t_int,t_node> inv_map;
    std::vector<int> seen;
    int m,n,yes;

    t_int insert( t_node x ) {
        if ( node2id.count(x) )
            return node2id[x];
        auto z= node2id.size();
        assert( z < 8*(m+n)+7 );
        inv_map[z]= x;
        return node2id[x]= z;
    }

#define vc(x,y) (0 <= (x) and (x) <= m and 0 <= (y) and (y) <= n)

    std::pair<t_node,t_aika> next_( const t_node &v ) {
        int x= std::get<0>(v),
            y= std::get<1>(v),
            t= std::get<2>(v);
        if ( not vc(x+dx[t],y+dy[t]) ) {
            int nt= (t+1)&3;
            for ( ;nt != t and not vc(x+dx[nt],y+dy[nt]); ++nt, nt&= 3 ) ;
            assert( nt != t );
            auto next_node= std::make_tuple(x,y,nt);
            assert( node2id.count(next_node) );
            return std::make_pair(next_node,0ll);
        } else {
            t_aika good= 0, bad= std::max(m,n)+0x80, mid;
            for ( ;good+1 != bad; ) {
                mid= (good+bad)>>1;
                auto nx= x+dx[t]*mid, ny= y+dy[t]*mid;
                vc(nx,ny)?(good= mid):(bad= mid);
            }
            auto next_node= std::make_tuple(x+dx[t]*good,y+dy[t]*good,t);
            assert( node2id.count(next_node) );
            return std::make_pair(next_node,good);
        }
    }

    void build_graph() {
        for ( auto &[key,val]: node2id ) {
            auto nx= next_(key);
            adj[val].insert({node2id[nx.first],nx.second});
        }
        head.clear(), period.clear();
    }

    t_int nxt( t_int x ) {
        return adj[x].begin()->first;
    }

    void preprocess_() {
        seen.resize(8*(n+m)+7), std::fill(seen.begin(),seen.end(),0), yes= 0;
        for ( auto &[key,val]: node2id ) {
            auto x= val, y= val;
            if ( seen[x] == yes )
                continue ;
            std::vector<t_int> orbit;
            seen[x]= yes, distance_to[x]= 0, orbit.push_back(x);
            for ( y= nxt(x); y != x; seen[y]= yes, orbit.push_back(y), y= nxt(y) ) ;
            for ( distance_to[y= nxt(x)]= period[x]= adj[x].begin()->second; y != x; )
                period[x]+= adj[y].begin()->second, y= nxt(y), distance_to[y]= period[x];
            for ( auto z: orbit )
                head[z]= x;
        }
    }

	std::pair<t_pos,t_pos> location_at_iterative( t_int v, t_aika t ) {
		auto curr= v;
		for (;t >= adj[curr].begin()->second; t-= adj[curr].begin()->second, curr= adj[curr].begin()->first ) ;
		auto tr= inv_map[curr];
		int direction= std::get<2>(tr);
		return {std::get<0>(tr)+t*dx[direction],std::get<1>(tr)+t*dy[direction]};
	}

    std::pair<t_pos,t_pos> location_at( t_int v, t_aika t ) {
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
        node2id.clear(), inv_map.clear();
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

        build_graph();
        preprocess_();
    }

    std::pair<i64,i64> get_location_at( std::pair<i64,i64> pos, int direction, i64 tick ) {
        t_node v= std::make_tuple(pos.first,pos.second,direction);
        int x= std::get<0>(v), y= std::get<1>(v), t= std::get<2>(v);
        i64 good= 0, bad= std::max(m,n)+0x80, mid;
        for ( ;good+1 != bad; ) {
            mid= (good+bad)>>1;
            vc(x+dx[t]*mid,y+dy[t]*mid)?(good= mid):(bad= mid);
        }
        if ( good <= tick ) {
            return {x+dx[t]*good,y+dy[t]*good};
        }
        t_node hitting_point= std::make_tuple(x+dx[t]*good,y+dy[t]*good,t);
        assert( node2id.count(hitting_point) );
        tick -= good;
        assert( tick >= 0 );
        return location_at(node2id[hitting_point],tick);
    }
};

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    for ( int ts= 0, m,n,numballs, qr; is >> m >> n; ) {
        if ( ++ts > 1 )
            os << '\n';
        Solution s(m,n);
        if ( not(is >> numballs) )
            break ;
        std::vector<t_node> balls(numballs);
        for ( auto &v: balls ) {
            is >> std::get<0>(v) >> std::get<1>(v);
            assert( 0 <= std::get<0>(v) and std::get<0>(v) <= m );
            assert( 0 <= std::get<1>(v) and std::get<1>(v) <= n );
			std::get<2>(v)= -1;
            int xx,yy;
            is >> xx >> yy;
            for ( int i= 0; i < 4; ++i )
                if (xx == dx[i] and yy == dy[i]) {
                    std::get<2>(v) = i;
                    break ;
                }
			assert( std::get<2>(v) != -1 );
        }
        for ( is >> qr; qr--; ) {
            t_aika time_tick;
            is >> time_tick;
            std::vector<std::pair<t_pos,t_pos>> res(numballs);
            for ( int i= 0; i < numballs; ++i )
                res[i]= s.get_location_at(std::make_pair(std::get<0>(balls[i]),std::get<1>(balls[i])),std::get<2>(balls[i]),time_tick);
            std::sort(res.begin(),res.end());
            for ( int i= 0; i < numballs; ++i )
                os << res[i].first << ' ' << res[i].second << '\n';
        }
        break ;
    }
    return 0;
}
    
