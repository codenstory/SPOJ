/**
 * TOPIC: binary search, greedy, constructive, precision
 * status: Accepted
 */
#include <bits/stdc++.h>
#define oo (1ll<<60)
#define tol (1e-8)
using i64= std::int64_t;

int n;
i64 D;
std::vector<std::pair<i64,int>> data;
std::vector<i64> c;

bool f( i64 T ) {
    i64 L= -oo;
    for ( auto i= 0; i < c.size(); ++i ) {
        auto leftmost= std::max(L,c[i]-T);
        if ( labs(leftmost-c[i]) > T )
            return false ;
        L= leftmost+D;
    }
    return true ;
}

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> D, D<<= 1;
        data.resize(n), c.clear();
        for ( auto &v: data ) {
            is >> v.first >> v.second;
            for ( i= 0; i < v.second; ++i )
                c.push_back(v.first*2);
        }
        std::sort(c.begin(),c.end());
        i64 bad= 0, good= +oo, mid;
        if ( f(bad) ) {
            good= bad;
            os << "Case #" << ++cs << ": " << (good/2) << "." << ((good&1)?"5000000":"0000000") << std::endl;
            continue ;
        }
        for ( ;bad+1 != good; ) {
            if ( f(mid= (good+bad)>>1) )
                good= mid;
            else bad= mid;
        }
        os << "Case #" << ++cs << ": " << (good/2) << "." << ((good&1)?"5000000":"0000000") << std::endl;
    }
    return 0;
}   
 