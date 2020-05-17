/**
 * FERT21_0
 * TOPIC: simulation, maths
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts,n,i,j,k,r;
    for ( is >> ts; ts--; ) {
        is >> n;
        if ( not --n ) {
            os << 1 << std::endl;
            continue ;
        }
        os << '0' << '.';
        std::vector<int> c(0x400,0);
        c.front()= 5, k= 1;
        for (--n; n--;) {
            for ( r= 0, i= 0; i < k; ++i ) {
                auto q= (10*r+c[i])>>1;
                r= (10*r+c[i])&1;
                c[i]= q;
            }
            if ( k < c.size() )
                c[k++]= (10*r)>>1;
            else {
                c.push_back((10*r)>>1);
                k= c.size();
            }
        }
        for ( i= 0; i < k; os << c[i++] ) ;
        os << std::endl;
    }
    return 0;
}
    