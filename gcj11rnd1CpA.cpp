/**
 * TOPIC: greedy, constructive
 * status: Accepted
 */
#include <bits/stdc++.h>
#define vc(x,y) (0<=(x) and (x)<m and 0<=(y) and (y)<n)

int m,n,
    dx[]= {-1,0,1,0},
    dy[]= {0,1,0,-1};
std::vector<std::string> g;

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> m >> n;
        g.resize(m);
        for ( auto &v: g )
            is >> v;
        std::vector<std::pair<int,int>> pos{};
        for ( i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j )
                if ( g[i][j] == '#' ) {
                    pos.emplace_back(i, j);
                    auto ni= i+1, nj= j;
                    if ( vc(ni,nj) and g[ni][nj] == '#' ) {
                        g[ni][nj]= '$';
                    }
                    else goto nx;
                    ni= i, nj= j+1;
                    if ( vc(ni,nj) and g[ni][nj] == '#' ) {
                        g[ni][nj]= '$';
                    }
                    else goto nx;
                    ni= i+1, nj= j+1;
                    if ( vc(ni,nj) and g[ni][nj] == '#' ) {
                        g[ni][nj]= '$';
                    }
                    else goto nx;
                }
        for ( auto pr: pos ) {
            i= pr.first, j= pr.second;
            auto ni= i+1, nj= j;
            g[ni][nj]= '\\';
            ni= i, nj= j+1, g[ni][nj]= '\\';
            ni= i+1, nj= j+1, g[ni][nj]= '/';
            g[i][j]= '/';
        }
        os << "Case #" << ++cs << ":\n";
        for ( auto &s: g )
            os << s << "\n";
        continue ;
        nx: os << "Case #" << ++cs << ":\nImpossible\n";
    }
    return 0;
}   
 