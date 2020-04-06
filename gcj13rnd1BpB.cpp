/**
 * TOPIC: probabilities, Binomial distribution, binary search, arithmetic sequence
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define N (1<<21)

std::istream &is = std::cin;
std::ostream &os = std::cout;

const double log_2= log(2);

double f[N];

double logC( int n, int k ) {
    assert( k <= n );
    auto res= f[n]-f[n-k]-f[k];
    assert( res >= 0 );
    return res;
}

void solve( int cs ) {
    os << "Case #" << cs << ": " << std::setprecision(7) << std::fixed;
    auto sum= [](i64 m)->i64 {
        return 2*m*m-m;
    };
    i64 n,sx,sy,m;
    is >> n >> sx >> sy;
    m= [&]()-> i64 {
        i64 low= 0, high= (1<<29), mid;
        for ( ;low+1 < high; ) {
            mid= (low+high)>>1;
            if ( sum(mid) <= n )
                low= mid;
            else high= mid;
        }
        return low;
    }();
    assert( sum(m) <= n and n < sum(m+1) );
    if ( sy-sx <= 2*(m-1) and sy+sx <= 2*(m-1) ) {
        os << 1.00 << std::endl;
        return ;
    }
    if ( not(sy-sx <= 2*m and sy+sx <= 2*m) ) {
        os << 0.00 << std::endl;
        return ;
    }
    auto excess= n-sum(m);
    if ( sy == 2*m ) {
        assert( excess < 4*(m+1)+1 );
        os << 0.00 << std::endl;
        return ;
    }
    assert( sy < 2*m );
    if ( sx < 0 ) sx= -sx;
    assert( sx > 0 );
    auto to_the_right_of= 2*m-sx;
    assert( to_the_right_of >= 0 );
    auto on_the_left= 2*m+1;
    auto room= on_the_left+to_the_right_of;
    if ( room <= excess ) {
        os << 1.00 << std::endl;
        return ;
    }
    double ans= 0.00;
    for ( i64 k= 2*m+1, j; k >= 0; --k ) {
        j= excess-k;
        if ( 0 <= j and j <= to_the_right_of ) {
            assert(excess >= j);
            ans += exp(logC(excess, j)-excess*log_2);
        }
    }
    os << std::max(0.00,std::min(1.00,1-ans)) << std::endl;
}

int main() {
    int i,cs= 0,ts;
    for ( f[0]= 0, i= 1; i < N; ++i )
        f[i]= f[i-1]+log(i);
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 