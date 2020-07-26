/**
 * BOBALLS2
 * TOPIC: simulation
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { NE, NW, SW, SE };

int dx[]= {1,-1,-1,1},
    dy[]= {1,1,-1,-1};

class Solution {

    using t_node= std::tuple<int, int, int>;
    using t_weight= i64;
    using t_int= int;
    using t_edge= std::pair<t_int,t_weight>;

    std::map<t_node, t_int> node2id;
    std::map<t_int,std::set<t_edge>> adj;
    int m,n;

    int insert( t_node x ) {
        if ( node2id.count(x) )
            return node2id[x];
        auto z= node2id.size();
        return node2id[x]= z;
    }

#define vc(x,y) (0 <= (x) and (x) <= m and 0 <= (y) and (y) <= n)

    void build_graph() {
        for ( auto &[key,val]: node2id ) {
            int x= std::get<0>(key),
                y= std::get<1>(key),
                t= std::get<2>(key);
            if ( not vc(x+dx[t],y+dy[t]) ) {
                int nt= (t+1)&3;
                for ( ;nt != t and not vc(x+dx[nt],y+dy[nt]); ++nt, nt&= 3 ) ;
                assert( nt != t );
                auto next_node= std::make_tuple(x,y,nt);
                assert( node2id.count(next_node) );
                adj[val].insert({node2id[next_node],0});
            } else {
                int good= 0, bad= (1<<29), mid;
                for ( ;good+1 != bad; ) {
                    mid= (good+bad)>>1;
                    auto nx= x+dx[t]*mid, ny= y+dy[t]*mid;
                    vc(nx,ny)?(good= mid):(bad= mid);
                }
                auto next_node= std::make_tuple(x+dx[t]*good,y+dy[t]*good,t);
                assert( node2id.count(next_node) );
                adj[val].insert({node2id[next_node],good});
            }
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

    }

};

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    return 0;
}
    