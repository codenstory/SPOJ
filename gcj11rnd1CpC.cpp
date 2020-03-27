/**
 * TOPIC: number theory, constructive
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define oo (1ll<<60)

std::vector<i64> LCM,GCD,freq;
i64 L,H;
int n;

i64 gcd(i64 x,i64 y) { return !y?x:gcd(y,x%y);}

i64 get_lcm( i64 x, i64 y ) {
    i64 g= gcd(x,y);
    if ( x/(0.00+g) > H/(0.00+y) )
        return +oo;
    return (x/g)*y;
}

void check( i64 x, std::set<i64> &c, bool strict= false ) {
    i64 alpha= L/x;
    if ( alpha*x < L )
        ++alpha;
    auto candidate= alpha*x;
    if ( not strict ) {
        if (L <= candidate and candidate <= H)
            c.insert(candidate);
    }
    else if ( alpha == 1 and L <= candidate and candidate <= H )
        c.insert(candidate);
}

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> L >> H, freq.resize(n);
        for ( auto &v: freq )
            is >> v;
        freq.push_back(1), ++n;
        std::sort(freq.begin(),freq.end());
        LCM.resize(n), GCD.resize(n);
        for ( LCM[0]= freq[0], i= 1; i < n; ++i )
            if ( LCM[i-1] < +oo )
                LCM[i]= get_lcm(LCM[i-1],freq[i]);
            else LCM[i]= +oo;
        for ( GCD[n-1]= freq[n-1], i= n-2; i >= 0; --i )
            GCD[i] = gcd(GCD[i+1], freq[i]);
        std::set<i64> c{};
        for ( i= 1; i < n-1; ++i )
            if ( LCM[i] < +oo and 0 == (GCD[i+1]%LCM[i]) ) {
                i64 kappa= GCD[i+1]/LCM[i];
                for ( i64 z= 1; z <= kappa/z+7; ++z )
                    if ( 0 == (kappa%z) ) {
                        check(z*LCM[i],c,true);
                        check((kappa/z)*LCM[i],c,true);
                    }
            }
        if ( LCM[n-1] < +oo )
            check(LCM[n-1],c);
        for ( i64 z= 1; z <= GCD[1]/z+7; ++z ) {
            if ( 0 == (GCD[1]%z) ) {
                auto t= GCD[1]/z;
                assert( z*t == GCD[1] );
                check(z,c,true),check(t,c,true);
            }
        }
        os << "Case #" << ++cs << ": ";
        if ( c.empty() )
            os << "NO" << std::endl;
        else os << *c.begin() << std::endl;
    }
    return 0;
}   
 