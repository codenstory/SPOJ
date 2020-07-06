/**
 * BRTREE
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

const i64 M= 1234567890;
#define oo (1ll<<62)

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( i64 n,K,i,t; (is >> n >> K) and n; ) {
        if ( K == 0 ) {
            os << n-1 << '\n';
            continue ;
        }
        if ( K <= 33 ) {
            std::vector<i64> c[2];
            i64 w[2];
            c[0].resize(K + 1, 0), c[1].resize(K + 1, 0);
            c[t = 0].front() = 1, w[t] = w[t ^ 1] = 0;
            for (int year = 1; year < n and w[t] <= M; ++year) {
                t ^= 1, c[t][0] = std::accumulate(c[t ^ 1].begin(), c[t ^ 1].begin() + K, 0ll);
                for (i = 1; i <= K-1; ++i)
                    c[t][i] = c[t^1][i-1];
                c[t][K] = c[t^1][K-1] + c[t^1][K];
                auto dw= std::accumulate(c[t ^ 1].begin(), c[t ^ 1].end(), 0ll);
                assert( w[t^1] < +oo-dw );
                w[t] = w[t ^ 1] + dw;
            }
            os << w[t] << '\n';
        } else {
            unsigned alpha = 0;
            for (int year = 1; (1ull << alpha) <= M and year < n; ++year, ++alpha);
            os << (1ull << (alpha)) - 1 << '\n';
        }
    }
    return 0;
}
    