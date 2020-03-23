/**
 * TOPIC: greedy, constructive, perms, transposition,s inversion count
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x40

std::string g[N];
int n,rightmost[N];

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n;
        for ( i= 0; i < n; ++i ) {
            is >> g[i];
            for ( j= n-1; j >= 0 and g[i][j] == '0'; --j ) ;
            rightmost[i]= j;
        }
        bool taken[n];
        int perm[n];
        for ( i= 0; i < n; taken[i++]= false ) ;
        int ans= 0;
        for ( i= 0; i < n; ++i ) {
            for ( k= 0; k < n and (taken[k] or rightmost[k] > i); ++k ) ;
            assert( k < n );
            taken[k]= true, perm[k]= i;
        }
        for ( i= 0; i < n; ++i )
            for ( j= i+1; j < n; ++j )
                if ( perm[i] > perm[j] )
                    ++ans;
        std::cout << "Case #" << ++cs << ": " << ans << std::endl;
    }
    return 0;
}   
 