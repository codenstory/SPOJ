/**
 * SETNJA
 * TOPIC: big integer, binary trees
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { BASE = 100000ll, DIGL= 5 };

std::istream &is = std::cin;
std::ostream &os = std::cout;

class bignum {
private:
    std::vector<i64> c;
    bignum &normalize() {
        int i;
        for ( i= 1; i < c[0]; ++i )
            if ( c[i] >= BASE )
                c[i+1]+= c[i]/BASE, c[i]%= BASE;
        for ( ;c[i] >= BASE; ++i )
            c.push_back(c[i]/BASE), c[i]%= BASE;
        c[0]= c.size()-1;
        return *this;
    }
public:
    bignum( i64 n ) {
        c.clear(), c.push_back(0);
        int i= 0;
        for (;n > 0; c.push_back(n%BASE), n/= BASE, ++i ) ;
        c[0]= (not i)?c.push_back(0),1:i;
    }
    bignum &operator *= ( i64 x ) {
        int i;
        for ( i= 1; i <= c[0]; c[i++]*= x ) ;
        return normalize();
    }
    bignum &operator += ( i64 x ) {
        c[1]+= x;
        return normalize();
    }
    bignum &operator += ( const bignum &rhs ) {
        int i;
        for ( i= 1; i <= c.front() and i <= rhs.c.front(); ++i )
            c[i]+= rhs.c[i];
        for ( ;i <= rhs.c.front(); c[i]+= rhs.c[i], ++i ) ;
        return normalize();
    }
    friend std::ostream &operator << ( std::ostream &out, const bignum &b ) ;
};

std::ostream &operator<<(std::ostream &out, const bignum &b) {
    int i;
    for ( i= b.c.front(); i and not b.c[i]; --i ) ;
    if ( not i )
        return out << '0';
    out << b.c[i--];
    for (;i; out << std::setfill('0') << std::setw(DIGL) << b.c[i--] ) ;
    return out;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( std::string s; is >> s; ) {
        bignum S(1), t(1);
        for ( auto ch: s ) {
            switch (ch) {
                case 'L':
                    S *= 2;
                    break;
                case '*':
                    (S *= 5) += t, t *= 3;
                    break;
                case 'R':
                    (S *= 2) += t;
                    break;
                default: assert( ch == 'P' );
                            break ;
            }
        }
        os << S << std::endl;
        break ;
    }
    return 0;
}
    