/**
 * TOURIST
 * TOPIC: DP, invariants
 * status: Accepted
 */
#include <bits/stdc++.h>
#define SH (7)
#define BIT(k) (1ull<<(k))
#define MASK(k) (BIT(k)-1ull)
#define Q (BIT(3*SH))
#define oo (0xffffffffull)
#define enc(j,i,k) ((j) | ((i)<<SH) | ((k)<<(SH+SH)))
using state= std::uint32_t;
using weight= std::uint32_t;
#define W(ch) ((ch)=='*'?1:0)

std::istream &is = std::cin;
std::ostream &os = std::cout;

int m,n;
std::vector<std::string> g;
std::vector<std::vector<int>> col, prizes;
weight z[Q];

int main() {
    int i,j,k,t,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( is >> ts; ts-- and (is >> n >> m); ) {
        g.resize(m);
        for ( auto &v: g ) {
            is >> v;
            assert( v.size() == n );
            v.push_back('#');
        }
        g.back().back()= '.';
        for ( ++n, prizes.resize(n), col.resize(n), j= 0; j < n; ++j ) {
            col[j].resize(m,0), prizes[j].resize(m,0);
            for ( i= 0; i < m; ++i )
                col[j][i]= g[i][j]=='#'?1:0, prizes[j][i]= g[i][j]=='*'?1:0;
            std::partial_sum(col[j].begin(),col[j].end(),col[j].begin());
            std::partial_sum(prizes[j].begin(),prizes[j].end(),prizes[j].begin());
        }
        std::vector<std::vector<bool>> reachable(m,std::vector<bool>(n,false));
        std::queue<std::pair<int,int>> que{};
        for ( reachable[m-1][n-1]= true, que.push({m-1,n-1}); not que.empty(); que.pop() ) {
            std::tie(i,j)= que.front();
            for ( int ni= i; ni >= i-1; --ni )
                for ( int nj= j; nj >= j-1; --nj )
                    if ( 0 <= ni and ni < m and 0 <= nj and nj < n and abs(ni-i)+abs(nj-j) == 1 )
                        if ( g[ni][nj] != '#' )
                            if ( not reachable[ni][nj] )
                                reachable[ni][nj] = true, que.push({ni, nj});
        }
        std::queue<state> q{};
        memset(z,0xffull,sizeof z);
        weight ans= 0;
        state u,v,src= enc(0,0,0);
        for ( z[src]= W(g.front().front()), q.push(src); not q.empty(); q.pop() ) {
            u= q.front(), j= (u&MASK(SH)), i= ((u>>SH)&MASK(SH)), k= (u>>(SH+SH)), t= i+j-k;
            if ( j == n-1 ) {
                assert( i == m-1 and k == m-1 );
                ans= std::max(ans,z[u]);
                continue ;
            }
            {
                if ( t+1 < n and i+1 < m and reachable[k][t+1] and reachable[i+1][j] ) {
                    weight collected= W(g[k][t+1])+W(g[i+1][j]);
                    if ( i+1 == k and j == t+1 and collected )
                        --collected;
                    else if ( i+1 == k and j == t and W(g[k][t]) ) {
                        assert( collected );
                        --collected;
                    } else if ( t+1 == j and k == i and W(g[i][j]) ) {
                        assert( collected );
                        --collected;
                    }
                    v = enc(j, i+1, k);
                    if ( z[v] == +oo or z[v] < z[u]+collected )
                        z[v]= z[u]+collected, q.push(v);
                }
            }
            {
                if ( j+1 < n and k+1 < m and reachable[k+1][t] and reachable[i][j+1] ) {
                    weight collected= W(g[k+1][t])+W(g[i][j+1]);
                    if ( i == k+1 and t == j+1 and collected )
                        --collected;
                    else if ( j+1 == t and i == k and W(g[k][t]) ) {
                        assert( collected );
                        --collected;
                    } else if ( k+1 == i and t == j and W(g[i][j]) ) {
                        assert( collected );
                        --collected;
                    }
                    v= enc(j+1,i,k+1);
                    if ( z[v] == +oo or z[v] < z[u]+collected )
                        z[v]= z[u]+collected, q.push(v);
                }
            }
            {
                if ( j+1 < n and t+1 < n and reachable[k][t+1] and reachable[i][j+1] ) {
                    weight collected= W(g[i][j+1])+W(g[k][t+1]);
                    if ( i == k and j+1 == t+1 and collected )
                        --collected;
                    else if ( i == k and j+1 == t and W(g[k][t]) )
                        --collected;
                    else if ( i == k and t+1 == j and W(g[i][j]) )
                        --collected;
                    v= enc(j+1,i,k);
                    if ( z[v] == +oo or z[v] < z[u]+collected )
                        z[v]= z[u]+collected, q.push(v);
                }
            }
            {
                if ( i+1 < m and k+1 < m and reachable[i+1][j] and reachable[k+1][t] ) {
                    weight collected= W(g[i+1][j])+W(g[k+1][t]);
                    if ( i+1 == k+1 and j == t and collected )
                        --collected;
                    else if ( j == t and i+1 == k and W(g[k][t]) )
                        --collected;
                    else if ( j == t and k+1 == i and W(g[i][j]) )
                        --collected;
                    v= enc(j,i+1,k+1);
                    if ( z[v] == +oo or z[v] < z[u]+collected )
                        z[v]= z[u]+collected, q.push(v);
                }
            }
        }
        os << ans << '\n';
    }
    return 0;
}
    