/**
 * SEQ6
 * TOPIC: maths, divisors
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

using i64= std::int64_t;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
    i64 x,y,m,n,ans,k;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( i64 N; (is >> N) and N; os << ans << std::endl ) {
        N <<= 3, ans= 0;
        for ( x= 2; x <= N/x; x+= 2 )
            if ( not (N%x) ) {
                y= N/x;
                if ( (x+y)&1 )
                    continue ;
                k= (x+y)>>1;
                if ( not((k-x)&1) )
                    continue ;
                n= ((k-x)+1)>>1;
                if ( (k+(2*n-1)) != y )
                    continue ;
                if ( not(k&1) )
                    continue ;
                i64 m1= -n+(-1-k)/2, m2= -n+(-1+k)/2;
                ans+= (m1 == m2)?1:2;
            }
    }
    return 0;
}
    