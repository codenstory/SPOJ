/**
 * NG0FRCTN
 * TOPIC: simulation, recursion
 * status: Accepted
 */
#include <bits/stdc++.h>
#define B(k) (1ll<<(k))
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::pair<i64,i64> get_coords( i64 n ) {
    if ( n == 0 ) {
        return {0,0};
    }
    auto k= std::max(static_cast<int>(log(n)/log(2))-1,0);
    for (;B(k)-1 <= n; ++k ) ;
    for (;B(k)-1  > n; --k ) ;
    assert( B(k)-1 <= n );
    assert( B(k+1)-1 > n );
    assert( k >= 0 );
    i64 rownum= k, colnum= n-(B(k)-1);
    return {rownum,colnum};
}

std::pair<i64,i64> f( i64 r, i64 c ) {
    assert( c <= B(r) );
    if ( r == 0 )
        return {1,1};
    assert( r >= 1 );
    i64 m,n;
    std::tie(m,n)= f(r-1,(c>>1));
    return not (c&1)?std::make_pair(m,m+n):std::make_pair(m+n,n);
}

std::pair<i64,i64> get_ans( i64 N ) {
    i64 r,c;
    std::tie(r,c)= get_coords(N);
    return f(r,c);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    {
        i64 n= 11;
        i64 r,c;
        std::tie(r,c)= get_coords(n);
        assert( r == 3 );
        assert( c == 4 );
    }
    {
        i64 n= 6;
        i64 r,c;
        std::tie(r,c)= get_coords(n);
        assert( r == 2 and c == 3 );
    }
    for ( i64 n,x,y; (is >> n) and n; ) {
        std::tie(x,y)= get_ans(n-1);
        os << x << '/' << y << '\n';
    }
    return 0;
}
    