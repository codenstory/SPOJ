/**
 * TOPIC: simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x40

int n,K;
std::vector<std::vector<char>> grid,ans;

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> K, grid.resize(n), ans.resize(n);
        for ( i= 0; i < n; ++i ) {
            std::string tmp;
            grid[i].resize(n), ans[i].resize(n);
            is >> tmp;
            for (j = 0; j < n; ++j )
                grid[i][j]= tmp[j];
        }
        std::for_each(grid.begin(),grid.end(),[](auto &v) {
            std::stable_partition(v.begin(),v.end(),[](auto ch) {
                return ch == '.';
            });
        });
        for ( i= 0; i < n; ++i )
            for ( j= 0; j < n; ++j )
                ans[i][j]= grid[j][n-1-i];
        for ( i= 0; i < n; ++i ) {
            for ( j= 0; j < n; ++j )
                std::cerr << ans[i][j] << ' ';
            std::cerr << '\n';
        }
        std::map<char,bool> res{};
        for ( auto ch: {'B','R'} ) {
            res[ch]= false ;
            for ( i= 0; i < n and not res[ch]; ++i ) {
                for ( k= 0; k < n; k= j ) {
                    for (j = k; j < n and ans[i][j] != ch; ++j);
                    auto len= 0;
                    for ( ;j < n and ans[i][j] == ch; ++j, ++len ) ;
                    res[ch]|= (len >= K);
                }

            }
            for ( j= 0; j < n and not res[ch]; ++j ) {
                for ( k= 0; k < n; k= i ) {
                    for ( i= k; i < n and ans[i][j] != ch; ++i ) ;
                    auto len= 0;
                    for ( ;i < n and ans[i][j] == ch; ++i, ++len ) ;
                    res[ch]|= (len >= K);
                }
            }
            for ( i= 0; i < n and not res[ch]; ++i )
                for ( j= 0; j < n and not res[ch]; ++j )
                    if ( ans[i][j] == ch ) {
                        auto len= 0;
                        for ( auto ni= i, nj= j; 0 <= ni and ni < n and 0 <= nj and nj < n and ans[ni][nj] == ch; ) {
                            ++len, ++ni, ++nj;
                        }
                        res[ch]|= (len >= K);
                    }
            for ( i= 0; i < n and not res[ch]; ++i )
                for ( j= 0; j < n and not res[ch]; ++j )
                    if ( ans[i][j] == ch ) {
                        auto len= 0;
                        for ( auto ni= i, nj= j; 0 <= ni and ni < n and 0 <= nj and nj < n and ans[ni][nj] == ch; ) {
                            ++len, --ni, ++nj;
                        }
                        res[ch]|= (len >= K);
                    }
        }
        auto answer= res['B'] and res['R'] ? "Both" : ((not res['B'] and not res['R'])?"Neither":\
        not res['R']?"Blue":"Red");
        os << "Case #" << ++cs << ": " << answer << std::endl;
    }
    return 0;
}   
 