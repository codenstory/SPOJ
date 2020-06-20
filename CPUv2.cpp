/**
 * TOPIC: ad hoc, primes, precalc
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<21)

using i64= std::int64_t;
bool isprime[N];
std::vector<i64> p;

bool is_prime( i64 x ) {
    if ( x < N )
        return isprime[x];
    for ( auto c: p ) {
        if ( c*c > x )
            break ;
        if ( not(x%c) )
            return false;
    }
    return true ;
}

std::vector<std::pair<i64,i64>> ans;

int main() {
    i64 i,j,k;
    for ( isprime[2]= true, i= 3; i < N; isprime[i]= true, i+= 2 ) ;
    for ( i= 3; i < N; i+= 2 )
        if ( isprime[i] )
            for ( j= i*i; j < N; isprime[j]= false, j+= i ) ;
    for ( i= 0; i < N; ++i )
        if ( isprime[i] )
            p.push_back(i);
    std::set<std::pair<i64,i64>> s{};
    //std::cerr << p.size() << std::endl;
    for ( i= 0; i < p.size(); ++i )
        for ( i64 A= -1000; A <= 1000; ++A ) {
            i64 B= p[i]-A;
            i64 P= A*p[i]+B, Q= P*A+B;
            if ( Q > 2'000'000'000 ) break;
            if ( A+B != P and P != Q and A+B != Q )
                if ( A+B >= 0 and A+B <= 2'000'000'000 )
                    if ( 0 <= P and P <= 2'000'000'000 )
                        if ( 0 <= Q and Q <= 2'000'000'000 ) {
                            if ( is_prime(P) and is_prime(Q) )
                                s.insert({A,B});
                        }
        }
    //std::cout << s.size() << std::endl;
    for ( auto &item: s )
        //std::cout << "{" << item.first << "," << item.second << "}" << std::endl;
        ans.push_back(item);
    int ts;
    i64 L,H;
    for ( std::cin >> ts; ts--; ) {
        std::cin >> L >> H;
        i64 res= 0ll;
        for ( const auto &item: ans ) {
            auto A= item.first, B= item.second;
            if ( true ) {
                i64 cur= A+B, ax= cur;
                assert( is_prime(cur) );
                for ( k= 1;; ++k ) {
                    if ( k >= 3 and L <= ax and ax <= H )
                        ++res;
                    cur= cur*A+B;
                    //if ( A == 3 and B == 2 and L == 4505 ) {
                        //std::cerr << ax << " and " << cur << std::endl;
                    //}
                    if ( not is_prime(cur) )
                        break ;
                    ax *= cur;
                    if ( ax > H )
                        break ;
                }
            }
        }
        std::cout << res << std::endl;
    }
    return 0;
}
