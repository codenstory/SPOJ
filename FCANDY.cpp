/**
 * FCANDY
 * TOPIC: greedy, invariants, observations
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 101
#define W 256
#define Q (N*W+7)

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n;
std::vector<std::pair<int,int>> c;
bool s[2][Q];

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,t,l,T,best;
    for ( ;(is >> n) and n; ) {
        for ( T= 0, c.resize(n), i= 0; i < n; ++i ) {
            is >> k >> j, t= std::min(k,W/j+1);
            if ( t != k ) {
                assert( t < k );
                t-= ((k-t)&1);
            }
            assert( 0 == ((k-t)&1) );
            k= t;
            c[i]= {j,k}, T+= j*k;
            // assert( c[i].first*c[i].second <= W );
        }
        for ( memset(s,0,sizeof s), t= 0, s[t][0]= true, i= 0; i < n; ++i )
            for ( t^= 1, memset(s[t],0,sizeof s[t]), j= 0; j <= T and j < Q; ++j )
                for ( l= 0; s[t^1][j] and l <= c[i].second; ++l )
                    if ( c[i].first*l+j <= T and c[i].first*l+j < Q )
                        s[t][c[i].first*l+j]= true ;
        for ( best= (1<<21), j= 0; j <= T and j < Q; ++j )
            if ( s[t][j] ) {
                auto rest= T-j;
                if ( abs(rest-j) < best )
                    best= abs(rest-j);
            }
        os << best << std::endl;
    }
    return 0;
}
    