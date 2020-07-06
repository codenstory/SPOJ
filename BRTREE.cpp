/**
 * BRTREE
 * TOPIC: fast simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

const i64 M= 1'234'567'890LL;
#define oo (1ll<<62)

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( i64 K,n,i,j,t; (is >> n >> K) and n; ) {
        /*
        i64 ans= 0;
        std::vector<i64> dp(n+1,0ll);
        dp[1]= 0;
        for ( i= 2; i <= n; ++i ) {
            dp[i]= i-1;
            for ( j= std::max((i64)1,i-K); j <= i-1; ++j )
                dp[i]= dp[i]+dp[j];
            if ( dp[i] > M ) {
                ans = dp[i];
                goto nxt;
            }
        }
        ans= dp[n];
        nxt: os << ans << '\n';
         */
        if ( K == 0 ) {
            os << std::min(M+1,n-1) << '\n';
            continue ;
        }
        if ( K <= 33 ) {
            std::vector<i64> c[2];
            i64 w[2];
            c[0].resize(K + 1, 0), c[1].resize(K + 1, 0);
            c[t = 0].front() = 1, w[t] = w[t ^ 1] = 0;
            for (i64 year = 1; year < n and w[t] <= M; ++year) {
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
            for ( i64 year = 1; year < n and (1ull << alpha)-1 <= M; ++year, ++alpha);
            os << (1ull << alpha) - 1 << '\n';
        }
    }
    return 0;
}
    
