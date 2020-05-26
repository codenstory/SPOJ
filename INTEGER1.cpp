/**
 * INTEGER1
 * TOPIC: oberflows, DP, recurrences, interview-level
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 63
#define tol 1e-7
using i64= std::uint64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

i64 A,B;
bool seen[N];
i64 z[N],ra[N],rb[N];

i64 calc_z( int k ) {
    assert( k < N );
    assert( k > 0 );
    if ( seen[k] )
        return z[k];
    seen[k]= true, z[k]= (rb[k]-ra[k]+1);
    assert( z[k] >= 0 );
    if ( z[k] ) {
        for ( int t= 2; t*k < N; ++t ) {
            // if ( rb[t*k]-ra[t*k]+1 == 0 ) break ;
            if ( not(calc_z(t*k) <= z[k]) ) {
                std::cerr << calc_z(t*k) << std::endl;
                std::cerr << z[k] << std::endl;
                std::cerr << t*k << std::endl;
                std::cerr << k << std::endl;
                assert( false );
            }
            z[k]-= calc_z(t*k);
            if ( not z[k] )
                break ;
        }
    }
    assert( z[k] >= 0 );
    return z[k];
}

i64 mypow( i64 t, i64 k ) {
    i64 ax= 1ll, n= k;
    if ( k*log(t)/log(2) >= N )
        return 0xffffffffffffffffull;
    for (i64 y = t; n; n >>= 1, y *= y)
        if (n & 1)
            ax *= y;
    return ax;
}

void correct( i64 &val, i64 x, i64 k ) {
    for ( auto t: {(i64)std::max((std::int64_t)0,((std::int64_t)val)-1),val,val+1} ) {
        if ( t < 0 ) continue ;
        i64 ax= mypow(t,k);
        if ( ax == x ) {
            val= t;
            return ;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( ;(is >> A >> B) and (A or B); ) {
        if ( A > B )
            std::swap(A,B);
        memset(seen,0,sizeof seen);
        ra[1]= A, rb[1]= B;
        for ( int k= 2; k < N; ++k ) {
            ra[k]= static_cast<i64>(std::pow(A,1.00/k));
            rb[k]= static_cast<i64>(std::pow(B,1.00/k));
            correct(ra[k],A,k);
            correct(rb[k],B,k);
            if ( mypow(ra[k],k) != A )
                for ( ;mypow(ra[k],k) < A; ++ra[k] ) ;
            if ( mypow(rb[k],k) != B )
                for ( ;mypow(rb[k],k) > B; --rb[k] ) ;
            assert( mypow(ra[k],k) >= A and (ra[k] == 0 or mypow(ra[k]-1,k) < A) );
            if ( not(mypow(rb[k],k) <= B and (mypow(rb[k]+1,k) > B)) ) {
                std::cerr << A << " " << B << " " << k << " " << rb[k] << std::endl;
                assert( false );
            }
        }
        //std::cerr << ra[5] << " " << rb[5] << std::endl;
        //std::cerr << ra[1] << " " << rb[1] << std::endl;
        i64 ans= 0;
        //std::cerr << calc_z(1) << std::endl;
        //std::cerr << calc_z(5) << std::endl;
        for ( int k= 1; k < N; ans+= k*calc_z(k), ++k ) ;
        os << ans << std::endl;
    }
    return 0;
}
    