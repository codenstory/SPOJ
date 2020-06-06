/**
 * TOURIST
 * TOPIC: DP
 * status:
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

std::istream &is = std::cin;
std::ostream &os = std::cout;

int m,n;
std::vector<std::string> g;
weight z[Q];
std::vector<std::vector<int>> col, prizes;

weight f( int j, int i, int ni, int k, int nk ) {
    assert( i <= k and ni <= nk );
    if ( col[j][ni]-col[j][i] or col[j][nk]-col[j][k] )
        return +oo;
    if ( g[ni][j+1] == '#' or g[nk][j+1] == '#' )
        return +oo;
    assert( prizes[j][ni] >= prizes[j][i] );
    assert( prizes[j][nk] >= prizes[j][k] );
    weight a= prizes[j][ni]-prizes[j][i],
           b= prizes[j][nk]-prizes[j][k];
    if ( g[ni][j+1] == '*' ) ++a;
    if ( g[nk][j+1] == '*' ) ++b;
    int res= a+b;
    if ( ni == nk and g[ni][j+1] == '*' )
        --res;
    assert( res >= 0 );
    if ( ni < k )
        return res;
    assert( ni >= k );
    res-= (prizes[j][ni]-(k==0?0:prizes[j][k-1]));
    if ( i == k and g[i][j] == '*' )
        ++res;
    assert( res >= 0 );
    return res;
}

weight F( int j, int i, int k ) {
    weight a= g[i][j+1]=='*'?1:0, b= g[k][j+1]=='*'?1:0;
    int res= a+b;
    if ( i == k and g[i][j+1] == '*' ) --res;
    assert( res >= 0 );
    return res;
}

weight G( int j, int i, int ni, int k, int nk ) {
    assert( i <= k and ni <= nk );
    if ( col[j][ni]-col[j][i] or col[j][nk]-col[j][k] )
        return +oo;
    if ( g[ni][j+1] == '#' or g[nk][j+1] == '#' )
        return +oo;
    assert( prizes[j][ni] >= prizes[j][i] );
    assert( prizes[j][nk] >= prizes[j][k] );
    weight a= prizes[j][ni]-prizes[j][i],
           b= prizes[j][nk]-prizes[j][k];
    int res= a+b;
    assert( res >= 0 );
    if ( ni < k )
        return res;
    assert( ni >= k );
    res-= (prizes[j][ni]-(k==0?0:prizes[j][k-1]));
    if ( i == k and g[i][j] == '*' )
        ++res;
    assert( res >= 0 );
    return res;
}

weight H( int j, int i, int ni, int k, int nk ) {
    weight a= g[ni][j]=='*'?1:0, b= g[nk][j]=='*'?1:0;
    int res= a+b;
    return res;
}

int main() {
    int i,j,k,ts;
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
                                reachable[ni][nj]= true, que.push({ni,nj});
        }
        memset(z,0xffull,sizeof z);
        std::queue<state> q{};
        state u,v,src= enc(0,0,0);
        z[src]= (g[0][0]=='*'?1:0);
        weight ans= 0;
        for ( q.push(src); not q.empty(); q.pop() ) {
            u= q.front(), j= (u&MASK(SH)), i= ((u>>SH))&MASK(SH), k= (u>>(SH+SH));
            assert( i <= k );
            if ( j == n-1 ) {
                if ( i == m-1 and k == m-1 )
                    ans= std::max(ans,z[u]);
                continue ;
            }
            assert( j < n-1 );

            /*
            for ( int ni= i; ni < m and (ni+j+1+k+j+1 < 2*(m+n)); ++ni ) {
                if (not reachable[ni][j]) break;
                if (not reachable[ni][j + 1]) continue;
                for (int nk = std::max(k, ni); nk < m and (ni+j+1+nk+j+1 < 2*(m+n)); ++nk) {
                    if ( not reachable[nk][j] )
                        break ;
                    if (reachable[nk][j + 1]) {
                        weight collected = f(j, i, ni, k, nk);
                        if (collected < +oo)
                            if (z[v = enc(j + 1, ni, nk)] == +oo or z[v] < z[u] + collected)
                                z[v] = z[u] + collected, q.push(v);
                    }
                }
            }
            */

            // stay in the same column
            for ( int ni= i; ni <= i+1 and ni < m; ++ni ) {
                if (not reachable[ni][j]) break;
                for (int nk = std::max(k,ni); nk <= k+1 and nk < m; ++nk) {
                    if ( not reachable[nk][j] )
                        break ;
                    if ( ni == i and k == nk  ) continue ;
                    weight collected = G(j, i, ni, k, nk);
                    if (collected < +oo)
                        if (z[v = enc(j,ni,nk)] == +oo or z[v] < z[u] + collected)
                            z[v] = z[u] + collected, q.push(v);
                }
            }
            // move to the next column
            if ( reachable[i][j+1] and reachable[k][j+1] ) {
                weight collected = F(j, i, k);
                if (collected < +oo)
                    if (z[v = enc(j + 1, i, k)] == +oo or z[v] < z[u] + collected)
                        z[v] = z[u] + collected, q.push(v);
            }
        }
        os << ans << '\n';
    }
    return 0;
}
    