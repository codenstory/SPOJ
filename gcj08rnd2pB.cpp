/**
 * TOPIC: puzzle, constructive, modulo operator
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

i64 A,n,m;

double area( std::pair<i64,i64> a, std::pair<i64,i64> b, std::pair<i64,i64> c ) {
    std::pair<i64,i64> L= {b.first-a.first,b.second-a.second};
    std::pair<i64,i64> K= {c.first-a.first,c.second-a.second};
    return labs(L.first*K.second-L.second*K.first)/2.00;
}

int main() {
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    int ts,cs =0;
    for ( is >> ts; ts--; ) {
        is >> n >> m >> A;
        os << "Case #" << ++cs << ": ";
        auto q= A/m;
        if ( A > m*n ) {
            os << "IMPOSSIBLE\n";
            continue ;
        }
        std::pair<i64,i64> u{0,0}, v{n,1}, w{0,0};
        if ( 0 == (A%n) )
            w.second= A/n, w.first= 0;
        else {
            w.first= n-(A%n);
            w.second= (A+w.first)/n;
        }
        assert( area(u,v,w)*2 == A );
        os << u.first << " " << u.second << " " << v.first << " " << v.second << " " << w.first << " " << w.second << "\n";
    }
    return 0;
}

