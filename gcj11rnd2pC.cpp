/**
 * TOPIC: LCM, prime factorisation, constructive
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1ull << 21)
using i64= std::int64_t;

bool isprime[N];
std::vector<i64> p;

int main() {
    i64 i, j, k, n, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( isprime[2]= true, i= 3; i < N; isprime[i]= true, i+= 2 ) ;
    for ( i= 3; i < N; i+= 2 )
        for ( j= i+i; isprime[i] and j < N; isprime[j]= false, j+= i ) ;
    p.clear();
    for ( i= 0; i < N; ++i )
        if ( isprime[i] )
            p.push_back(i);
    for ( is >> ts; ts--; ) {
        is >> n;
        if ( n == 1 ) {
           os << "Case #" << ++cs << ": " << 0 << std::endl;
           continue ;
        }
        std::vector<i64> alpha{};
        for ( i= 0; i < p.size() and p[i] <= n; ++i ) {
            i64 ax= p[i];
            for ( j= 1; ax <= n/p[i]; ax*= p[i], ++j ) ;
            alpha.push_back(j);
        }
        for ( i= 0; i < p.size() and p[i] <= n/p[i]; ++i )
            for ( ;not(n%p[i]); n/= p[i] ) ;
        os << "Case #" << ++cs << ": " << (1+std::accumulate(alpha.begin(),alpha.end(),0ll)-alpha.size()) << std::endl;
    }
    return 0;
}   
 