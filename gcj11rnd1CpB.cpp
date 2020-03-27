/**
 * TOPIC: greedy, constructive, maths
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

i64 L,T,n,C;
std::vector<i64> a,d,t;

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> L >> T >> n >> C, a.resize(C);
        for ( auto &v: a ) is >> v;
        for ( d.clear(), i= 0; i < n; )
            for ( k= 0; k < C and i < n; ++i, d.push_back(a[k++]) ) ;
        for ( t.clear(), t.push_back(0), i= 1; i <= n; ++i )
            t.push_back(t[i-1]+2*d[i-1]);
        for ( i= 0; i < n; ++i )
            if ( d[i] - (std::max(T,t[i])-t[i])/2.00 >= 0 )
                break ;
        if ( i == n ) {
            os << "Case #" << ++cs << ": " << t[n] << std::endl;
            continue ;
        }
        if ( n-i <= L ) {
            double savings= d[i]-(std::max(T,t[i])-t[i])/2.00;
            for ( ;++i < n; )
                savings+= d[i];
            //assert( not(savings & 1) );
            i64 res;
            os << "Case #" << ++cs << ": " << (res= static_cast<i64>(t[n]-savings)) << std::endl;
            assert( fabs(res-t[n]+savings) < 1e-7 );
            continue ;
        }
        std::vector<double> s{};
        for ( s.push_back(d[i]-(std::max(T,t[i])-t[i])/2.00); ++i < n; s.push_back(d[i]) ) ;
        std::sort(s.begin(),s.end());
        i64 res;
        os << "Case #" << ++cs << ": " << (res=
                static_cast<i64>(t[n]-std::accumulate(s.rbegin(),s.rbegin()+L,0.00))) << std::endl;
        assert( fabs(res-t[n]+std::accumulate(s.rbegin(),s.rbegin()+L,0.00)) < 1e-7 );
    }
    return 0;
}   
 