/**
 * DISQUERY
 */
#include <bits/stdc++.h>
#define N (1<<17)
#define MAXE (N*2)

class tree {

    enum { MN= 0, MX= 1 };

    using t_node= std::int32_t;
    using t_weight= std::int32_t;

    const t_weight oo= (1<<29);

    bool seen[N];
    t_node to[MAXE],p[N];
    t_weight w[N],longest[N],d[N];
    int n,next[MAXE],last[N],E,best[N],
        which_ladder[N],pos[N];
    std::vector<std::vector<t_node>> ladders, anc;
    std::vector<std::vector<t_weight>> dp[2];
    std::vector<std::vector<std::pair<t_node,t_weight>>> adj;

    void add_arc( t_node x, t_node y, t_weight weight ) {
        int i= E++, j= E++;
        to[i]= y, next[i]= last[x], last[x]= i;
        to[j]= x, next[j]= last[y], last[y]= j;
        w[i]= w[j]= weight;
    }

    void dfs( t_node x ) {
        assert( not seen[x] ) ;
        seen[x]= true;
        for ( auto i= last[x]; i != -1; i= next[i] ) {
            auto y= to[i];
            if ( not seen[y] ) {
                dfs(y), p[y]= x;
                adj[x].push_back({y,w[i]});
            }
        }
    }

    void calc_anc( t_node x ) {
        assert( not seen[x] );
        seen[x]= true ;
        int k;
        for ( auto [y,wgt]: adj[x] ) {
            assert( not seen[y] );
            d[y]= d[x]+1;
            size_t sz= 33-__builtin_clz(d[y]);
            anc[y].resize(sz,-1), dp[MN][y].resize(sz,+oo), dp[MX][y].resize(sz,-oo);
            for ( anc[y][0]= x, k= 1; (1<<k) <= d[y] and anc[y][k-1] != -1; anc[y][k]= anc[anc[y][k-1]][k-1], ++k ) ;
            for ( dp[MN][y][0]= dp[MX][y][0]= wgt, k= 1; (1<<k) <= d[y] and anc[y][k-1] != -1; ++k ) {
                dp[MN][y][k]= std::min(dp[MN][y][k-1],dp[MN][anc[y][k-1]][k-1]);
                dp[MX][y][k]= std::max(dp[MX][y][k-1],dp[MX][anc[y][k-1]][k-1]);
            }
            calc_anc(y);
        }
    }

    int calc_ladder_length( t_node x ) {
        assert( not seen[x] );
        seen[x]= true, longest[x]= 1, best[x]= -1;
        for ( int i= 0; i < adj[x].size(); ++i ) {
            auto y= adj[x][i].first;
            assert( not seen[y] );
            auto len= 1+calc_ladder_length(y);
            if ( (longest[x]= std::max(longest[x],len)) == len )
                best[x]= i;
        }
        return longest[x];
    }

    void preprocess() {
        int x,y,k;
        for ( x= 0; x < n; seen[x++]= false ) ;
        calc_ladder_length(0);
        std::queue<int> q{};
        for ( x= 0; x < n; seen[x++]= false ) ;
        std::unordered_set<t_node> in_chain{};
        for ( ladders.clear(), seen[0]= true, q.push(0); not q.empty(); q.pop() ) {
            auto X= q.front();
            x= q.front();
            std::vector<int> chain{};
            chain.push_back(x);
            for ( int i=best[x]; i!=-1; x=adj[x][i].first, chain.push_back(x), i=best[x] ) ;
            for ( auto z: chain ) {
                assert( not in_chain.count(z) );
                seen[z] = true, in_chain.insert(z);
            }
            for ( auto z: chain )
                for ( auto [t,_]: adj[z] )
                    if ( not seen[t] )
                        seen[t]= true, q.push(t);
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
            return +oo;
        assert( d[px] < d[x] );
        auto k= 31-__builtin_clz(d[x]-d[px]);
        assert( (1ll<<k) <= d[x]-d[px] and (1ll<<k) >= (d[x]-d[px])/2 );
        return std::min(dp[MN][x][k],dp[MN][level_ancestor(x,d[x]-d[px]-(1<<k))][k]);
    }

    int max_query( t_node px, t_node x ) {
        if ( px == x )
            return -oo;
        assert( d[px] < d[x] );
        auto k= 31-__builtin_clz(d[x]-d[px]);
        assert( (1ll<<k) <= d[x]-d[px] and (1ll<<k) >= (d[x]-d[px])/2 );
        return std::max(dp[MX][x][k],dp[MX][level_ancestor(x,d[x]-d[px]-(1<<k))][k]);
    }

public:

    tree( std::istream &is ) {
        is >> n, E= 0;
        int x,y,cost;
        for ( d[0]= 0, x= 0; x < n; seen[x]= false, last[x++]= -1 ) ;
        for ( int k= 0; k < n-1; is >> x >> y >> cost, add_arc(x-1,y-1,cost), ++k ) ;
        adj.resize(n,std::vector<std::pair<t_node,t_weight>>{});
        dfs(0);
        anc.resize(n,std::vector<t_node>{});
        dp[MN].resize(n,std::vector<t_weight>{}), dp[MX].resize(n,std::vector<t_weight>{});
        for ( preprocess(), x= 0; x < n; seen[x++]= false ) ;
        calc_anc(0);
    }

    t_node level_ancestor( t_node x, int k ) {
        if ( k == 0 )
            return x;
        if ( k >= d[x] )
            return 0;
        if ( pos[x] >= k )
            return ladders[which_ladder[x]][pos[x]-k];
        auto i= 31-__builtin_clz(d[x]-k);
        return level_ancestor(anc[x][i],k-(1<<i));
    }

    t_node lca( t_node x, t_node y ) {
        if ( x == y )
            return x;
        int good= std::max(d[x],d[y]), bad= 0;
        for (;bad+1 != good;) {
            auto mid= (good+bad)/2;
            if ( level_ancestor(x,mid) == level_ancestor(y,mid) )
                good= mid;
            else bad= mid;
        }
        return level_ancestor(x,good);
    }

    std::pair<int,int> query( t_node x, t_node y ) {
        auto z= lca(x,y);
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

    for ( int ts= 0; ts < 2; ++ts ) {
        auto T = new tree(is);
        int x, y, qr, a, b;
        for (is >> qr; qr--;) {
            is >> x >> y;
            std::tie(a, b) = T->query(x - 1, y - 1);
            os << a << ' ' << b << '\n';
        }
        delete T;
    }
    return 0;
}
    
