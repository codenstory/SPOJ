/**
 * BRTREE
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

const i64 M= 1234567890ll;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    i64 w[2];
    int i,j,k,t;
    for ( i64 n,K; (is >> n >> K) and n; ) {
        if ( K == 0 ) {
            os << n-1 << '\n';
            continue ;
        }
        if ( K <= 43 ) {
            std::vector<i64> c[2];
            c[0].resize(K + 1, 0), c[1].resize(K + 1, 0);
            c[t = 0].front() = 1, w[t] = w[t ^ 1] = 0;
            for (int year = 1; year < n and w[t] <= M; ++year) {
                t ^= 1, c[t][0] = std::accumulate(c[t ^ 1].begin(), c[t ^ 1].begin() + K, 0ll);
                for (i = 1; i <= K-1; ++i)
                    c[t][i] = c[t^1][i-1];
                c[t][K] = c[t ^ 1][K - 1] + c[t ^ 1][K];
                w[t] = w[t ^ 1] + std::accumulate(c[t ^ 1].begin(), c[t ^ 1].end(), 0ll);
                if ( w[t] > M ) {
                    os << w[t] << '\n';
                    goto nx;
                }
            }
            os << w[t] << '\n';
            nx: continue ;
        } else {
            if ( n >= 47 ) {
                unsigned alpha = 0;
                for (int year = 1; (1ull << alpha) <= M and year < n; ++year, ++alpha);
                os << (1ul << (alpha)) - 1 << '\n';
            } else {
                std::vector<i64> c[2];
                auto m= std::min(n+2,K+1);
                c[0].resize(m, 0), c[1].resize(m, 0);
                c[t = 0].front() = 1, w[t] = w[t ^ 1] = 0;
                for (int year = 1; year < n and w[t] <= M; ++year) {
                    t ^= 1, c[t][0] = std::accumulate(c[t ^ 1].begin(), c[t ^ 1].begin()+std::min(K,m), 0ll);
                    for (i = 1; i < m and i <= K-1; ++i)
                        c[t][i] = c[t^1][i-1];
                    if ( i == K )
                        c[t][K]= c[t^1][K-1]+c[t^1][K];
                    auto dw= std::accumulate(c[t ^ 1].begin(), c[t ^ 1].begin()+std::min(K+1,m), 0ll);
                    if ( w[t^1] > M-dw ) {
                        os << w[t^1]+dw << '\n';
                        goto nx2;
                    }
                    w[t]= w[t^1]+dw;
                }
                os << w[t] << '\n';
                nx2: continue ;
            }
        }
    }
    return 0;
}
    