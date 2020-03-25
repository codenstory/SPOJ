/**
 * TOPIC: modulo, number theory, greedy, constructive, parameters
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

i64 n,PD,PG;

i64 gcd( i64 x, i64 y ) { return !y?x:gcd(y,x%y); }

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> PD >> PG;
        os << "Case #" << ++cs << ": ";
        if ( PD > 0 and PG == 0 ) {
            os << "Broken\n";
            continue ;
        }
        if ( PD < 100 and PG == 100 ) {
            os << "Broken\n";
            continue ;
        }
        if ( (PD==100 and PG==100) or (PD==0 and PG==0) or (PD==0) or (PD==100) ) {
            os << "Possible\n";
            continue ;
        }
        i64 hprime= 100/gcd(100,PG);
        i64 hprimeprime= 100/gcd(100,PD);
        if ( n < hprimeprime ) {
            os << "Broken\n";
            continue ;
        }
        os << "Possible\n";
    }
    return 0;
}   
 