/**
 * KSEQ
 * TOPIC: Maths
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    i64 x,y,z,ts,ans;
    for ( is >> ts; ts--; ) {
        is >> x >> y >> z, ans= 0;
        if ( z ) {
            for (i64 n = 1; n <= z/n; ++n)
                if (not(z % n)) {
                    auto k = z / n;
                    if (not(y % (n + k))) {
                        auto m = y / (n + k);
                        if (n+m+k >= 2 and 2 * x == k * (k - 1) + m * (m - 1) + n * (n - 1))
                            ans += (n == k ? 1 : 2);
                    }
                }
        } else {
            if ( y ) {
                for ( i64 m= 1; m <= y/m; ++m )
                    if ( not (y%m) ) {
                        auto k= y/m;
                        if ( m+k >= 2 and 2*x == k*(k-1)+m*(m-1) )
                            ans+= 2*(m==k?1:2);
                    }
            } else {
                if ( x ) {
                    i64 k= static_cast<i64>((sqrt(8*x+1)+1)/2.00);
                    for ( auto t: {k-7,k-6,k-5,k-4,k-3,k-2,k-1,k,k+1,k+2,k+3,k+4,k+5,k+6,k+7} )
                        if ( t*(t-1) == 2*x and t >= 2 )
                            ans+= 3;
                } else {
                    assert( not x and not y and not z );
                    ans= 0;
                }
            }
        }
        os << ans << std::endl;
    }
    return 0;
}
    