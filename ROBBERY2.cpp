/**
 * TOBBERY2
 * TOPIC: maths
 * status: Accepted
 */
#include <bits/stdc++.h>
using u64= std::uint64_t;

std::istream &is= std::cin;
std::ostream &os= std::cout;

u64 n,k;

int main() {
    int ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( is >> ts; ts--; ) {
        is >> n >> k;
        u64 L= static_cast<u64>(sqrt(2*(n)));
        for ( ;L*(L+1)/2 > n; --L ) ;
        for ( ;(L+1)*(L+2) <= 2*n; ++L ) ;
        assert( L*(L+1)/2 <= n and (L+1)*(L+2)/2 > n );
        auto rem= n-L*(L+1)/2;
        auto m= L/k, r= L%k;
        std::vector<u64> ans(k,0);
        u64 i;
        for ( i= 1; i <= k; ++i ) {
            ans[i-1]= i*m+(m-1)*m/2*k;
        }
        for ( i= 1; i <= r; ++i )
            ans[i-1]+= i+m*k;
        for ( ;rem; ++i ) {
            if ( i == k+1 )
                i= 1;
            ans[i-1]+= rem, rem= 0;
        }
        bool first= true ;
        for ( i= 1; i <= k; ++i ) {
            if ( not first )
                os << ' ';
            os << ans[i-1];
            first= false;
        }
        os << '\n';
    }
    return 0;
}
