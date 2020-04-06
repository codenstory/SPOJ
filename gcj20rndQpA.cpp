/**
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

void solve( int cs ) {
    int i,j,k,n;
    is >> n;
    std::vector<std::vector<int>> g(n),e(n);
    for ( auto &v: g ) {
        v.resize(n);
        for ( auto &z: v )
            is >> z;
    }
    for ( auto &v: e )
        v.resize(n);
    for ( i= 0; i < n; ++i )
        for ( j= 0; j < n; ++j )
            e[i][j]= g[j][i];
    auto f= [](const std::vector<std::vector<int>> &c) {
        std::vector<int> r{};
        std::transform(c.begin(),c.end(),std::back_inserter(r),[](const auto &v) {
            auto u= v;
            std::sort(u.begin(),u.end());
            return std::unique(u.begin(),u.end())-u.begin();
        });
        return std::accumulate(r.begin(),r.end(),0,[&c](auto acc,auto z) {
            return acc+= (z != c.size())?1:0;
        });
    };
    for ( k= 0, i= 0; i < n; k+= g[i][i], ++i ) ;
    os << "Case #" << cs << ": " << k << " " << f(g) << " " << f(e) << std::endl;
}

int main() {
    int cs = 0, ts;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 