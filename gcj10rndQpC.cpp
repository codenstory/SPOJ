/**
 * TOPIC: simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
using u64= std::uint64_t;
#define N 0x400
#define oo (0xffffffffffffffffull)

i64 R,k,n,g[N];
u64 w[N][N],total;
int nx[N];

int main() {
    int i,j,cs= 0,ts;
    std::istream &is= std::cin;
    for ( is >> ts; ts--; ) {
        is >> R >> k >> n;
        for ( total= 0ull, i= 0; i < n; is >> g[i], total+= g[i++] ) ;
        if ( n == 1 ) {
            std::cout << "Case #" << ++cs << ": " << R*g[0] << std::endl;
            continue ;
        }
        memset(w,0xff,sizeof w);
        for ( i= 0; i < n; ++i ) {
            u64 sm= g[i];
            for ( j= (i+1)%n; i != j and sm+g[j] <= k; sm+= g[j], ++j, j%= n ) ;
            w[i][nx[i]= j]= sm;
        }
        u64 ans= 0ull;
        for ( i= 0; R-- > 0; ans+= w[i][nx[i]], i= nx[i] ) ;
        std::cout << "Case #" << ++cs << ": " << ans << std::endl;
    }
    return 0;
}
