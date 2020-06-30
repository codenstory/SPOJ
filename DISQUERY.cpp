/**
 * DISQUERY
 * TOPIC: LCA, path min queries, ladder approach
 * REFERENCES: https://link.springer.com/content/pdf/10.1007%2F3-540-45995-2.pdf
 * status: Accepted
 */
#include <bits/stdc++.h>
#ifndef ONLINE_JUDGE
#define N (1000007)
#else
#define N (100007)
#endif
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
        which_ladder[N],pos[N],e[N];
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
                dfs(y), adj[x].push_back({y,w[i]});
                e[y]= adj[p[y]= x].size()-1;
            }
        }
    }

    int tick, tin[N], tout[N];

    void calc_anc( t_node x ) {
        assert( not seen[x] );
        seen[x]= true ;
        tin[x]= ++tick;
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
        tout[x]= ++tick;
    }

    bool is_ancestor( t_node px, t_node x ) {
        return tin[px] <= tin[x] and tout[x] <= tout[px];
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
        for ( x= 0; x < n; seen[x++]= false )
			assert( seen[x] );
        for ( ladders.clear(), seen[0]= true, q.push(0); not q.empty(); q.pop() ) {
            x= q.front();
            std::vector<int> chain{};
            chain.push_back(x);
            for ( int i=best[x]; i!=-1; x=adj[x][i].first, chain.push_back(x), i=best[x] ) ;
            for ( auto z: chain )
                seen[z] = true;
            for ( auto z: chain )
                for ( auto [t,_]: adj[z] )
                    if ( not seen[t] )
                        seen[t]= true, q.push(t);
            for ( auto z: chain )
                which_ladder[z]= ladders.size();
            std::reverse(chain.begin(),chain.end());
            for ( y= chain.back(), k= chain.size(); k and y != 0; chain.push_back(p[y]), y= p[y], --k ) ;
            if ( y == 0 )
                chain.push_back(0);
            std::reverse(chain.begin(),chain.end());
            for ( k= 0; k < chain.size() and chain[k] != q.front(); ++k ) ;
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
        assert( k < dp[MN][x].size() );
        return std::min(dp[MN][x][k],dp[MN][level_ancestor(x,d[x]-d[px]-(1<<k))][k]);
    }

    int max_query( t_node px, t_node x ) {
        if ( px == x )
            return -oo;
        assert( d[px] < d[x] );
        auto k= 31-__builtin_clz(d[x]-d[px]);
        assert( (1ll<<k) <= d[x]-d[px] and (1ll<<k) >= (d[x]-d[px])/2 );
        assert( k < dp[MX][x].size() );
        return std::max(dp[MX][x][k],dp[MX][level_ancestor(x,d[x]-d[px]-(1<<k))][k]);
    }

public:

    tree( std::istream &is ) {
        is >> n, E= 0, tick= -1;
        int x,y,cost;
        for ( d[0]= 0, x= 0; x < n; seen[x]= false, last[x++]= -1 ) ;
        for ( int k= 0; k < n-1; is >> x >> y >> cost, add_arc(x-1,y-1,cost), ++k ) ;
        adj.resize(n,std::vector<std::pair<t_node,t_weight>>{});
        dfs(0);
        for ( x= 0; x < n; ++x )
            assert( seen[x] );
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
        auto i= 31-__builtin_clz(k);
        assert( (1<<i) <= k );
        assert( i < anc[x].size() );
        assert( k-(1<<i) <= pos[anc[x][i]] );
        return level_ancestor(anc[x][i],k-(1<<i));
    }

    t_node lca( t_node x, t_node y ) {
        if ( x == y )
            return x;
        if ( d[x] > d[y] )
            x= level_ancestor(x,d[x]-d[y]);
        if ( d[x] < d[y] )
            y= level_ancestor(y,d[y]-d[x]);
        assert( d[x] == d[y] );
        if ( x == y )
            return x;
        int good= std::max(d[x],d[y]), bad= 0;
        assert( level_ancestor(x,good) == level_ancestor(y,good) );
        assert( level_ancestor(x,bad)  != level_ancestor(y,bad) );
        for (;bad+1 != good;) {
            auto mid= (good+bad)/2;
            if ( level_ancestor(x,mid) == level_ancestor(y,mid) )
                good= mid;
            else bad= mid;
        }
        assert( level_ancestor(x,good) == level_ancestor(y,good) );
        assert( level_ancestor(x,good-1)  != level_ancestor(y,good-1) );
        return level_ancestor(x,good);
    }

    std::pair<t_weight,t_weight> query( t_node x, t_node y ) {
        auto z= lca(x,y);
        return {
            std::min(min_query(z,x),min_query(z,y)),
            std::max(max_query(z,x),max_query(z,y))
        };
    }

    t_node bf_lca( t_node x, t_node y ) {
        for (;d[x] > d[y]; x= p[x] ) ;
        for (;d[y] > d[x]; y= p[y] ) ;
        assert( d[x] == d[y] );
        for ( ;x != y; x= p[x], y= p[y] ) ;
        return x;
    }

    std::pair<t_weight,t_weight> bf( t_node x, t_node y ) {
        auto z= lca(x,y);
        int i;
#ifndef ONLINE_JUDGE
        auto t= bf_lca(x,y);
        assert( z == t );
#endif
        t_weight mn= +oo, mx= -oo;
        for ( ;x != z; x= p[x] ) {
            mn= std::min(mn,adj[p[x]][e[x]].second);
            mx= std::max(mx,adj[p[x]][e[x]].second);
        }
        for ( ;y != z; y= p[y] ) {
            mn= std::min(mn,adj[p[y]][e[y]].second);
            mx= std::max(mx,adj[p[y]][e[y]].second);
        }
        return {mn,mx};
    }

    t_node level_anc_bf( t_node x, int k ) {
        for ( ;k--; x= p[x] ) ;
        return x;
    }
};

int main() {

    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif

    std::istream &is = std::cin;
    std::ostream &os = std::cout;

    for ( int ts= 0; ts < 1; ++ts ) {
        auto T = new tree(is);
        int x, y, qr, a, b;
        for (is >> qr; qr--;) {
            is >> x >> y;
            std::tie(a, b) = T->query(x - 1, y - 1);
            /*
#ifndef ONLINE_JUDGE
            int ca,cb;
            std::tie(ca,cb)= T->bf(x-1,y-1);
            if ( ca != a ) {
                std::cerr << ca << " vs " << a << std::endl;
            }
            assert( ca == a );
            assert( cb == b );
#endif
             */
            os << a << ' ' << b << '\n';
            //assert( T->level_ancestor(x-1,y-1) == T->level_anc_bf(x-1,y-1) );
        }
        delete T;
    }
    return 0;
}
    
