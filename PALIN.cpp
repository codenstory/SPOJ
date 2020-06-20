/**
 * PALIN
 * TOPIC: ad hoc, greedy
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is= std::cin;
std::ostream &os= std::cout;

std::string S;
size_t n;

void reflect( std::string &c, size_t i ) {
    for ( size_t t= 0, k= c.size(); t < i; c[--k]= c[t++] ) ;
}

void f( std::string &s ) {
    int i,k;
    if ( std::find_if(s.begin(),s.end(),[](auto ch) {
            return ch != '9';
    }) == s.end() ) {
        std::string res(n+1,'0');
        res.front()= res.back()= '1';
        s= std::move(res);
        return ;
    }
    if ( n == 1 ) {
         assert( s[0] < '9' );
         ++s[0];
         return ;
    }
    if ( n&1 ) {
        if ( s[n/2] != '9' ) {
            assert( s[n/2] < '9' );
            ++s[n/2], reflect(s,n/2);
            return ;
        }
        for ( k= n/2+1, i= n/2-1; i >= 0 and s[i] == '9'; --i, ++k ) ;
        if ( i < 0 ) {
            reflect(s,n/2);
            return ;
        }
        assert( i >= 0 and s[i] < '9' );
        reflect(s,i);
        for ( s[k]= ++s[i]; ++i <= --k; s[i]= s[k]= '0' ) ;
        return ;
    }
    for ( k= n/2, i= n/2-1; i >= 0 and s[i] == '9'; --i, ++k ) ;
    if ( i < 0 ) {
        reflect(s,n/2-1);
        return ;
    }
    assert( i >= 0 and s[i] < '9' );
    reflect(s,i);
    for ( s[k]= ++s[i]; ++i <= --k; s[i]= s[k]= '0' ) ;
    return ;
}

int main() {
    int ts,i,k;
    for ( is >> ts; ts--; ) {
        is >> S, n= S.size();
        auto candidate1= S;
        f(candidate1);
        if ( candidate1.size() > S.size() ) {
            os << candidate1 << std::endl;
            continue ;
        }
        auto candidate2= S;
        reflect(candidate2,n/2);
        if ( candidate2 > S )
            os << std::min(candidate1,candidate2) << std::endl;
        else os << candidate1 << std::endl;
    }
    return 0;
}
