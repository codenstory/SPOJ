/**
 * BOBALLS2
 * TOPIC: simulation
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { NE, NW, SW, SE };

int dx[]= {1,-1,-1,1},
    dy[]= {1,1,-1,-1};

using t_node= std::tuple<int, int, int>;
using t_weight= i64;
using t_int= int;
using t_edge= std::pair<t_int,t_weight>;

class Solution {

    std::map<t_node, t_int> node2id;
    std::map<t_int,std::set<t_edge>> adj;
    std::map<t_int,i64> period,pre_period;
    std::map<t_int,t_int> P;
    std::map<t_int,t_node> inv_map;
    std::vector<int> seen;
    int m,n,yes;

    int insert( t_node x ) {
        if ( node2id.count(x) )
            return node2id[x];
        auto z= node2id.size();
        inv_map[z]= x;
        return node2id[x]= z;
    }

#define vc(x,y) (0 <= (x) and (x) <= m and 0 <= (y) and (y) <= n)

    std::pair<t_node,i64> next_( const t_node &v ) {
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
            int good= 0, bad= (1<<29), mid;
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
    }

    void preprocess_() {
        seen.resize(n+m+7), std::fill(seen.begin(),seen.end(),0), yes= 0;
        for ( auto &[key,val]: node2id ) {
            seen[val]= ++yes, period[val]= pre_period[val]= 0;
            auto x= val;
            for ( x= adj[x].begin()->first; seen[x] != yes;
            seen[x]= yes, x= adj[x].begin()->first ) ;
            for ( auto y= val; y != x; ) {
                pre_period[val]+= adj[y].begin()->second;
                y= adj[y].begin()->first;
            }
            period[val]= adj[x].begin()->second, P[val]= x;
            for ( auto y= adj[x].begin()->first; y != x; ) {
                period[val]+= adj[y].begin()->second;
                y= adj[y].begin()->first;
            }
        }
    }

    std::pair<i64,i64> location_at( t_int v, i64 t ) {
        if ( t < pre_period[v] ) {
            auto curr= v;
            for ( ;t >= adj[curr].begin()->second; t-= adj[curr].begin()->second, curr= adj[curr].begin()->first ) ;
            auto tr= inv_map[curr];
            return {std::get<0>(tr),std::get<1>(tr)};
        } else {
            t-= pre_period[v], t%= period[v];
            return location_at(P[v],t);
        }
    }

public:

    Solution( int m, int n ): m(m), n(n) {
        int i,j;
        for ( j= 0; j <= n; j+= n )
            for ( i= 1; i <= m-1; ++i ) {
                for ( int t= NE; t <= SE; ++t ) {
                    insert(std::make_tuple(i,j,t));
                }
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
        i64 good= 0, bad= (1<<29), mid;
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
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( int ts= 0, m,n,numballs, qr; is >> n >> m; ) {
        if ( ++ts > 1 )
            os << '\n';
        Solution s(m,n);
        if ( not(is >> numballs) )
            break ;
        std::vector<t_node> balls(numballs);
        for ( auto &v: balls ) {
            is >> std::get<0>(v) >> std::get<1>(v);
            int xx,yy;
            is >> xx >> yy;
            for ( int i= 0; i < 4; ++i )
                if (xx == dx[i] and yy == dy[i]) {
                    std::get<2>(v) = i;
                    break ;
                }
        }
        for ( is >> qr; qr--; ) {
            i64 time_tick;
            is >> time_tick;
            std::vector<std::pair<i64,i64>> res(numballs);
            for ( int i= 0; i < numballs; ++i )
                res[i]= s.get_location_at(std::make_pair(std::get<0>(balls[i]),std::get<1>(balls[i])),std::get<2>(balls[i]),time_tick);
            std::sort(res.begin(),res.end());
            for ( int i= 0; i < numballs; ++i )
                os << res[i].first << ' ' << res[i].second << '\n';
        }
    }
    return 0;
}
    