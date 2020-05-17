/**
 * PYRA
 * TOPIC: trees, dp, recursion
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (10100)

std::vector<int> adj[N],children[N];
int z[N],seen[N],yes;

int calc_z( int x ) {
    if ( z[x] != -1 )
        return z[x];
    z[x]= 1;
    for ( auto y: children[x] )
        z[x]+= (1+calc_z(y));
    return z[x];
}

void orient( int x ) {
    assert( seen[x] != yes );
    seen[x]= yes;
    for ( auto y: adj[x] )
        if ( seen[y] != yes )
            children[x].push_back(y), orient(y);
}

void solve_case( std::istream &is, std::ostream &os ) {
    int n,x,y,i,ans= 0;
    is >> n;
    for ( x= 0; x < n; z[x]= -1, children[x].clear(), adj[x++].clear() ) ;
    for ( i= 0; i < n-1; ++i )
        is >> x >> y, adj[x].push_back(y), adj[y].push_back(x);
    for ( ++yes, orient(0), i= 0; i < n; ans+= calc_z(i++) ) ;
    os << ans << std::endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int i,j,k,ts;
    for ( is >> ts; ts--; solve_case(is,os) ) ;
    return 0;
}
    