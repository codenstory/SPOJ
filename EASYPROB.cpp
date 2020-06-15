/**
 * EASYPROB
 * TOPIC: recursion
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

i64 n;

void dump( i64 x, std::ostream &out ) {
    if ( x ) {
        bool first= true ;
        if ( x <= 7 ) {
            for ( int k= 2; k >= 0; --k ) {
                if ( x & (1<<k) ) {
                    if ( not first )
                        out << "+";
                    if ( k == 1 )
                        out << "2";
                    else
                        out << "2(" << k << ")";
                    first= false ;
                }
            }
            return ;
        }
        for ( int k= 17; k >= 0; --k ) {
            if ( x & (1<<k) ) {
                if ( not first )
                    out << "+";
                if ( k == 0 )
                    out << "2(0)";
                else if ( k == 1 )
                    out << "2";
                else
                    out << "2(", dump(k,out), out << ")";
                first= false ;
            }
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    while ( (is >> n) and n ) {
        std::stringstream str{};
        dump(n,str);
        os << n << "=" << str.str() << std::endl;
    }
    return 0;
}
    