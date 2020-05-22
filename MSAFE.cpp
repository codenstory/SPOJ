/**
 * MSAFE
 * TOPIC: greedy, median, Manhattan distance
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;


std::istream &is = std::cin;
std::ostream &os = std::cout;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    const i64 M= 10'000'000;
    int n,i,j,k;
    for (;(is >> n) and n;) {
        std::vector<i64> c(n), d;
        for ( auto &v: c )
            is >> v;
        d= c, d.reserve(2*n);
        for ( auto z: c )
            d.push_back(M-z+1);
        std::nth_element(d.begin(),d.begin()+n/2,d.end());
        auto med1= d[n/2];
        std::nth_element(d.begin(),d.begin()+n/2-1,d.end());
        auto med2= d[n/2-1], med3= med2;
        if ( n >= 3 ) {
            std::nth_element(d.begin(),d.begin()+n/2+1,d.end());
            med3= d[n/2+1];
        }
        std::vector<i64> candidates;

        for ( auto med: {med1,med2,med3,(med1+med2)/2,(med2+med3)/2,(med1+med3)/2,(med1+med2)/2+1,(med2+med3)/2+1,(med1+med3)/2+1} ) {
            for ( k= 0; k <= 7; ++k ) {
                candidates.push_back(med+k);
                candidates.push_back(med-k);
                candidates.push_back(med+M/2+k);
                candidates.push_back(med+M/2-k);
                candidates.push_back(med-M/2-k);
                candidates.push_back(med-M/2+k);
                candidates.push_back(M/2-med-k);
                candidates.push_back(M/2-med+k);
            }
        }

        std::for_each(candidates.begin(),candidates.end(),[M](auto &x) {
            if ( not(1 <= x and x <= M) ) {
                if ( x < 1 ) {
                    for ( ;not(1<=x and x<=M); x+= M ) ;
                } else {
                    assert( x > M );
                    for ( ;not(1<=x and x<=M); x-= M ) ;
                }
            }
        });
        std::vector<i64> vals{};
        std::transform(candidates.begin(),candidates.end(),std::back_inserter(vals),[M,&c](auto T) {
            return std::accumulate(c.begin(),c.end(),0ll,[M,T](i64 acc, auto x) {
                return acc + std::min(labs(x-T),M-labs(x-T));
            });
        });
        os << *std::min_element(vals.begin(),vals.end()) << std::endl;
        //break ;
    }
    return 0;
}
    