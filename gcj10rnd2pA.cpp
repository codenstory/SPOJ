/**
 * TOPIC: DP, trees, greedy
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (11)
#define Q (1<<N)
using i64= std::int64_t;
using u64= std::uint64_t;
#define oo (0xffffffffffffffffull)
#define ROOT (1)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))

int n,m[1<<N];
std::vector<int> price[N];
u64 z[N][Q],maximum[Q];
bool seen[N][Q];

bool is_leaf( unsigned int u ) {
    return u >= (1ul<<(n-1));
}

u64 calc_max( unsigned int u ) {
    if ( maximum[u] < +oo )
        return maximum[u];
    if ( is_leaf(u) )
        return maximum[u]= std::max(m[L(u)-(1ul<<n)],m[R(u)-(1ul<<n)]);
    return maximum[u]= std::max(calc_max(L(u)),calc_max(R(u)));
}

size_t level( unsigned int u ) {
    return n-static_cast<size_t>(ceil(log(u)/log(2)+1e-7));
}

size_t position( unsigned int u ) {
    return u-(1<<static_cast<size_t>(1e-7+floor(log(u)/log(2))));
}

u64 calc_z( int k, unsigned int u ) {
    if ( seen[k][u] )
        return z[k][u];
    seen[k][u]= true ;
    if ( k >= calc_max(u) )
        return z[k][u]= 0;
    if ( is_leaf(u) ) {
        if ( k+1 >= calc_max(u) )
            z[k][u]= price[level(u)][position(u)];
    }
    else {
        if ( calc_z(k,L(u)) < +oo and calc_z(k,R(u)) < +oo )
            z[k][u]= calc_z(k,L(u))+calc_z(k,R(u));
        if ( calc_z(k+1,L(u)) < +oo and calc_z(k+1,R(u)) < +oo )
            z[k][u]= std::min(z[k][u],price[level(u)][position(u)]+calc_z(k+1,L(u))+calc_z(k+1,R(u)));
    }
    return z[k][u];
}


int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    {
        n= 3;
        assert( position(6) == 2 );
        assert( level(6) == 0 );
        assert( position(4) == 0 );
        assert( level(4) == 0 );
        assert( position(1) == 0 );
        assert( level(1) == n-1 );
        assert( position(7) == 3 );
        assert( level(7) == 0 );
    }
    for ( is >> ts; ts--; ) {
        for ( is >> n, i= 0; i < n; price[i++].clear() ) ;
        for ( i= 0; i < (1<<n); is >> m[i], m[i]= n-m[i], ++i ) ;
        for ( i= 0, k= n-1; k >= 0; --k, ++i ) {
            price[i].resize(1<<k);
            for ( auto &v: price[i] )
                is >> v;
        }
        memset(z,0xff,sizeof z);
        memset(maximum,0xff,sizeof maximum);
        memset(seen,0,sizeof seen);
        os << "Case #" << ++cs << ": " << calc_z(0,ROOT) << std::endl;
    }
    return 0;
}   
 