/**
 * FACT0
 * TOPIC: sieve, factorization
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<21)
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::bitset<N> b;
std::vector<i64> p;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    i64 i,j,k,n;
    for ( b.set(2,true), i= 3; i < N; b.set(i,true), i+= 2 ) ;
    for ( i= 3; i < N; i+= 2 )
        for ( j= i*i; b.test(i) and j < N; b.set(j,false), j+= i ) ;
    for ( i= 0; i < N; ++i )
        if ( b.test(i) )
            p.push_back(i);
    for (;(is >> n) and n;) {
        std::vector<std::pair<i64,int>> f{};
        for ( i= 0; i < p.size() and p[i] <= n/p[i]; ++i )
            if ( not (n%p[i]) ) {
                for (k = 0; not(n % p[i]); n /= p[i], ++k);
                f.emplace_back(p[i],k);
            }
        if ( i == p.size() ) {
            for ( j= p.back()+2; j <= n/j; j+= 2 )
                if ( not (n%j) ) {
                    for (k = 0; not(n % j); n /= j, ++k);
                    f.emplace_back(j,k);
                }
        }
        if ( n > 1 )
            f.emplace_back(n,1);
        for ( i= 0; i < f.size(); ++i ) {
            if ( i )
                os << ' ';
            os << f[i].first << '^' << f[i].second;
        }
        os << std::endl;
    }
    return 0;
}
    