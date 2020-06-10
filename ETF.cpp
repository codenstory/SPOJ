/**
 * ETF
 * TOPIC: sieve
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define N (1'000'001)

std::istream &is = std::cin;
std::ostream &os = std::cout;

bool is_prime[N];
i64 tot[N];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    i64 i,j,k,ts;
    for ( is_prime[2]= true, i= 3; i < N; is_prime[i]= true, i+= 2 ) ;
    for ( i= 0; i < N; tot[i]= i, ++i ) ;
    for ( tot[0]= 0, i= 2; i < N; ++i )
        if ( is_prime[i] )
            for ( tot[i]= (i-1), j= i+i; j < N; is_prime[j]= false, j+= i )
                tot[j]/= i, tot[j]*=(i-1);
    for ( is >> ts; ts--; ) {
        is >> k, os << tot[k] << '\n';
    }
    return 0;
}
    