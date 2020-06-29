/**
 * DISQUERY
 */
#include <bits/stdc++.h>
#define N (1<<17)
#define MAXE (N*2)

class tree {

    enum { MN= 0, MX= 1 };

    using t_node= std::int32_t;
    t_node to[MAXE],mapping[N],V,inv[N];
    int n,next[MAXE],last[N],E,w[N],distance[N],
        p[N],longest[N],best[N],
        which_ladder[N],pos[N],d[N];
    std::vector<std::vector<int>> ladders, dp[2];
    std::vector<std::vector<t_node>> anc;
    bool seen[N];

    void add_arc( t_node x, t_node y, int weight ) {
        int i= E++, j= E++;
        to[i]= y, next[i]= last[x], last[x]= i;
        to[j]= x, next[j]= last[y], last[y]= j;
        w[i]= w[j]= weight;
    }

    std::vector<std::vector<std::pair<t_node,int>>> adj;

    void dfs( t_node x ) {
        assert( not seen[x] ) ;
        seen[x]= true, inv[mapping[x]= V++]= x+1;
        for ( auto i= last[x]; i != -1; i= next[i] ) {
            auto y= to[i];
            if ( not seen[y] ) {
                dfs(y), p[mapping[y]]= mapping[x];
                adj[mapping[x]].push_back({mapping[y],w[i]});
            }
        }
    }

    void calc_anc( t_node x ) {
        assert( not seen[x] );
        seen[x]= true ;
        int k;
        for ( auto [y,wgt]: adj[x] ) {
            assert( not seen[y] );
            d[y]= d[x]+1, distance[y]= distance[x]+wgt;
            anc[y].resize(log(d[y])/log(2)+2);
            for ( anc[y][0]= x, k= 1; (1<<k) <= d[y] and anc[y][k-1] != -1; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
            dp[MN][y][0]= dp[MX][y][0]= wgt;
            for ( k= 1; (1<<k) <= d[y] and anc[y][k-1] != -1; ++k ) {
                dp[MN][y][k]= std::min(dp[MN][y][k-1],dp[MN][anc[y][k-1]][k-1]);
                dp[MX][y][k]= std::max(dp[MX][y][k-1],dp[MX][anc[y][k-1]][k-1]);
            }
            calc_anc(y);
        }
    }

    int calc_ladder_length( t_node x ) {
        if ( seen[x] )
            return longest[x];
        seen[x]= true, longest[x]= 1, best[x]= -1;
        for ( int i= 0; i < adj[x].size(); ++i ) {
            auto y= adj[x][i].first;
            assert( not seen[y] );
            if ( (longest[x]= std::max(longest[x],1+calc_ladder_length(y))) == 1+calc_ladder_length(y) )
                best[x]= i;
        }
        return longest[x];
    }

    void preprocess() {
        int x,y,k;
        for ( x= 0; x < n; seen[x++]= false ) ;
        calc_ladder_length(x);
        std::queue<int> q{};
        for ( ladders.clear(), q.push(0); not q.empty(); q.pop() ) {
            auto X= q.front();
            x= q.front();
            std::vector<int> chain{};
            chain.push_back(x);
            for ( int i=best[x]; i!=-1; x=adj[x][i].first, chain.push_back(x), i=best[x] ) ;
            for ( auto z: chain )
                for ( auto [nd,_]: adj[z] )
                    q.push(nd);
            for ( auto z: chain )
                which_ladder[z]= ladders.size();
            std::reverse(chain.begin(),chain.end());
            for ( y= chain.back(), k= chain.size(); k and y != 0; chain.push_back(p[y]), y= p[y], --k ) ;
            std::reverse(chain.begin(),chain.end());
            for ( k= 0; k < chain.size() and chain[k] != X; ++k ) ;
            assert( k < chain.size() );
            for ( ;k < chain.size(); pos[chain[k]]= k, ++k ) ;
            ladders.push_back(std::move(chain));
        }
    }

    int min_query( t_node px, t_node x ) {
        if ( px == x )
            return std::numeric_limits<int>::max();
        auto k= 31-__builtin_clz(d[x]-d[px]);
        return std::max(dp[MX][x][k],dp[MX][level_ancestor(inv[x],d[x]-d[px]-(1<<k))][k]);
    }

    int max_query( t_node px, t_node x ) {
        if ( px == x )
            return std::numeric_limits<int>::max();
        auto k= 31-__builtin_clz(d[x]-d[px]);
        return std::max(dp[MN][x][k],dp[MN][level_ancestor(inv[x],d[x]-d[px]-(1<<k))][k]);
    }

public:

    tree( std::istream &is ) {
        is >> n, E= 0;
        int x,y,cost;
        for ( d[0]= 0, x= 0; x < n; seen[x]= false, last[x++]= -1 ) ;
        for ( int k= 0; k < n-1; is >> x >> y >> cost, add_arc(x-1,y-1,cost), ++k ) ;
        adj.resize(n,std::vector<std::pair<t_node,int>>{});
        V= 0, dfs(0);
        assert( V == n );
        anc.resize(n), dp[MN].resize(n), dp[MX].resize(n);
        for ( preprocess(), x= 0; x < n; seen[x++]= false ) ;
        calc_anc(0);
    }

    t_node level_ancestor( t_node x, int k ) {
        x= mapping[x-1];
        if ( k == 0 )
            return inv[x];
        if ( k >= d[x] )
            return 0;
        auto i= 31-__builtin_clz(k);
        auto y= anc[x][i];
        assert( pos[y] >= (k-(1<<i)) );
        return inv[ladders[which_ladder[y]][pos[y]-(k-(1<<i))]];
    }

    t_node lca( t_node x, t_node y ) {
        if ( x == y )
            return x;
        x= mapping[x-1], y= mapping[y-1];
        int good= std::max(d[x],d[y]), bad= 0;
        for (;bad+1 != good;) {
            auto mid= (good+bad)/2;
            if ( level_ancestor(x,mid) == level_ancestor(y,mid) )
                good= mid;
            else bad= mid;
        }
        return level_ancestor(x,good);
    }

    int cost( t_node px, t_node x ) {
        return distance[mapping[x-1]]-distance[mapping[px-1]];
    }

    std::pair<int,int> query( t_node x, t_node y ) {
        auto z= lca(x,y);
        x= mapping[x-1], y= mapping[y-1], z= mapping[z-1];
        return {
            std::min(min_query(z,x),min_query(z,y)),
            std::max(max_query(z,x),max_query(z,y))
        };
    }

};

int main() {

    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif

    std::istream &is = std::cin;
    std::ostream &os = std::cout;

    auto T= new tree(is);
    int x,y,qr,a,b;
    for ( is >> qr; qr--; ) {
        is >> x >> y;
        std::tie(a,b)= T->query(x,y);
        os << a << ' ' << b << '\n';
    }
    delete T;

    return 0;

}
    