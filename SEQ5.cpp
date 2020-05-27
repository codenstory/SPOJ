/**
 * SEQ5
 * TOPIC: color counting, chaining, queries
 * status: Accepted
 */
#include <bits/stdc++.h>

#define N (1<<21)
using u64= std::uint64_t;

int n,xx[N],yy[N],z[N];
u64 L,R,c[N];
std::unordered_map<u64,int> prev;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int i,j,k,l,r;
    u64 ans;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( ;(is >> n >> L >> R) and n; os << ans << std::endl ) {
        for ( ans= 0, prev.clear(), i= 1; i <= n; is >> c[i++] ) ;
        // Muthukrishnan's chaining
        for ( i= 1; i <= n; ++i )
            z[i]= (prev.count(c[i])?prev[c[i]]:0), prev[c[i]]= i;
        for ( i= 1; i <= n; xx[i]= n, yy[i]= -1, ++i ) ;
        std::unordered_set<int> lp{},rp{};
        // initialization, "seeding"
        for ( r= 0, l= 0, i= 1; i <= n; ++i ) {
            if ( z[i] )
                rp.insert(z[i]), lp.insert(z[i]);
            if (z[i] == 0) {
                if ( ++r, (++l == L) )
                    break;
            }
        }
        assert( l == L or i == n+1 );
        if ( i == n+1 ) continue ;
        for ( xx[i]= yy[i]= 1; not lp.empty() and lp.count(yy[i]); lp.erase(yy[i]++) ) ;
        // sweepline of sorts
        for ( ;++i <= n; ) {
            assert( l == L );
            assert( l <= r and r <= R );
            assert( not lp.count(yy[i-1]) );
            yy[i]= yy[i-1];
            if ( z[i] < yy[i] ) {
                assert( yy[i] >= 1 );
                for ( ++yy[i]; not lp.empty() and lp.count(yy[i]); lp.erase(yy[i]++) ) ;
            } else {
                assert( l == L );
                assert( z[i] >= 1 );
                assert( not lp.count(z[i]) );
                if ( z[i] == yy[i] ) {
                    for ( ++yy[i]; not lp.empty() and lp.count(yy[i]); lp.erase(yy[i]++) ) ;
                } else {
                    lp.insert(z[i]);
                }
            }
            xx[i]= xx[i-1];
            if ( z[i] < xx[i] and r == R ) {
                assert( xx[i] >= 1 );
                for ( ;not rp.empty() and rp.count(xx[i]); rp.erase(xx[i]++) ) ;
                ++xx[i];
            } else {
                if ( z[i] == 0 ) ++r;
                if ( z[i] )
                    rp.insert(z[i]);
            }
            assert( not lp.count(yy[i]) );
        }
        for ( i= 1; i <= n; ++i )
            ans+= std::max(0,yy[i]-xx[i]+1);
    }
    return 0;
}
    