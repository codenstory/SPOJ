/**
 * MUL2COM
 * TOPIC: two's complement, big integer
 * status: Accepted
 */
#include <bits/stdc++.h>
enum{ BASE = 2ll };
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

class bigint {
private:
    mutable std::vector<i64> c;
    i64 operator[] ( int i ) const {
        return c[i];
    }
    i64 &operator[] ( int i ) {
        return c[i];
    }
public:
    bool negate() const {
        int i;
        for ( i= 1; i <= c.size()-1; c[i++]^= 1 ) ;
        for ( i= 1; i <= c.size()-1 and c[i] == BASE-1; c[i++]= 0 ) ;
        if ( i > size() )
            return false ;
        c[i]= BASE-1;
        for ( i= c.size()-1; i >= 1 and not c[i]; --i ) ;
        c[0]= (not i)?1:i;
        return true ;
    }
    bool operator == ( const bigint &other ) const {
        if ( size() != other.size() )
            return false ;
        return c == other.c;
    }
    explicit bigint( bool NONE= false ) {
        if ( not NONE )
            c= std::vector<i64>{1,0};
        else c.clear();
    }
    bool is_negative() const {
        return c.back() == 1;
    }
    int size() const {
        return c.empty() ? 0 : c[0];
    }
    explicit bigint( std::istream &istr ) {
        std::string s;
        istr >> s;
        c.clear(), c.push_back(s.size());
        std::transform(s.rbegin(),s.rend(),std::back_inserter(c),[](char ch) {
            return ch-'0';
        });
        int i;
        for ( i= c.size()-1; i >= 1 and not c[i]; --i ) ;
        c[0]= (not i)?1:i;
    }
    bool is_zero() const {
        return size() == 1 and c[1] == 0;
    }
    bool is_one() const {
        return size() == 1 and c[1] == 1;
    }
    bool is_negative_bound() const {
        return size() == c.size()-1 and std::find_if(c.begin()+1,c.end(),[](i64 x) {
            return x == 0;
        }) == c.end();
    }

    bigint operator * ( const bigint &rhs ) {

        if ( is_zero() or rhs.is_zero() )
           return is_zero()?*this:rhs;

        if ( is_one() or rhs.is_one() )
            return rhs.is_one()?*this:rhs;

        if ( is_negative_bound() or rhs.is_negative_bound() )
            return bigint(true);

        bool na= is_negative(), nb= rhs.is_negative();
        if ( na ) negate();
        if ( nb ) rhs.negate();

        std::vector<i64> r(c.size()+rhs.c.size()+7,0);
        int i,j;
        for ( i= 1; i <= size(); ++i )
            for ( j= 1; j <= rhs.size(); ++j )
                r[i+j-2]+= c[i]*rhs[j];
        if ( na ) negate();
        if ( nb ) rhs.negate();
        for ( i= 0; i < r.size(); ++i )
            if ( r[i] >= BASE )
                r[i+1]+= r[i]>>1, r[i]&= 1;
        for ( i= r.size()-1; i >= 0 and not r[i]; --i ) ;
        assert( i >= 0 );
        if ( i+1 <= c.size()-1 ) {
            bigint R;
            R.c.resize(c.size(),0);
            for ( j= 1; j <= i+1; ++j )
                R[j]= r[j-1];
            R[0]= i+1;
            if ( R.is_negative() )
                return bigint(true);
            if ( na ) R.negate();
            if ( nb ) R.negate();
            return R;
        } else {
            return bigint(true);
        }
    }

    friend std::ostream &operator << ( std::ostream &out, const bigint &b ) ;
};

std::ostream &operator<<(std::ostream &out, const bigint &b) {
    for ( int i= b.c.size()-1; i >= 1; out << b[i--] ) ;
    return out;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int n;
    for ( ;(is >> n) and n; ) {
        bigint a(is), b(is);
        bigint r= a*b;
        if ( r.size() == 0 ) {
            os << "overflow\n";
            continue ;
        }
        os << r << std::endl;
    }
    return 0;
}
    