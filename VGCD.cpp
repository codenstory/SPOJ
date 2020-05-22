/**
 * VGCD
 * TOPIC: Fibonacci, bignum
 * status: Accepted
 */
#include <bits/stdc++.h>
enum { BASE= 100000ll, DIGL= 5 };
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

class bignum {
private:
    std::vector<i64> c;
    i64 &operator [] ( int i ) {
        return c[i];
    }
    i64 operator [] ( int i ) const {
        return c[i];
    }
public:
    bignum() {
        c.clear();
    }
    explicit bignum( i64 x ) {
        int i= 0;
        for ( c= std::vector<i64>(2,0); x; c.push_back(0), c[++i]= x%BASE, x/= BASE ) ;
        c.front()= (not i)?1:i;
    }
    bool operator == ( const bignum &rhs ) const {
        if ( c.empty() and rhs.c.empty() )
            return true ;
        if ( c.empty() or rhs.c.empty() )
            return false ;
        if ( c.front() != rhs.c.front() )
            return false ;
        for ( int i= 1; i <= c.front(); ++i )
            if ( c[i] != rhs[i] )
                return false ;
        return true ;
    }
    void value_of( const std::string &s ) {
        if ( s.empty() ) {
            c.clear();
            return ;
        }
        int i,j,k,l,n= s.size();
        c.clear(), c.resize(13+n/DIGL,0);
        for ( l= 0, i= n-1; i >= 0; i= j ) {
            std::string t{};
            for ( k= 0, j= i; k < DIGL and j >= 0; ++k, --j )
                t.push_back(s[j]);
            std::reverse(t.begin(),t.end());
            c[++l]= std::strtol(t.c_str(),nullptr,10);
            assert( l < c.size() );
        }
        for ( i= c.size()-1; i and not c[i]; --i ) ;
        c.front()= (not i)?1:i;
    }
    friend bignum operator + ( const bignum &a, const bignum &b ) ;
    friend std::ostream &operator << ( std::ostream &out, const bignum &b ) ;
    friend std::istream &operator >> ( std::istream &in, bignum &b ) ;
    friend bool operator < ( const bignum &a, const bignum &b ) ;
    int length() const {
        return c.empty() ? 0 : c.front();
    }
};

bignum operator+(const bignum &a, const bignum &b) {
    bignum res{};
    res.c.resize(std::max(a.c.front(),b.c.front())+3,0);
    int i= 1;
    for ( ;i <= a.c.front() and i <= b.c.front(); res[i]= a[i]+b[i], ++i ) ;
    for ( ;i <= a.c.front(); res[i]+= a[i], ++i ) ;
    for ( ;i <= b.c.front(); res[i]+= b[i], ++i ) ;
    res.c.front()= std::max(a.c.front(),b.c.front())+1;
    for ( i= 1; i <= res.c.front(); ++i )
        if ( res[i] >= BASE )
            res[i+1]+= res[i]/BASE, res[i]%= BASE;
    for ( i= res.c.front(); i >= 1 and not res[i]; --i ) ;
    res.c.front()= (not i)?1:i;
    return res;
}

std::ostream &operator<<( std::ostream &out, const bignum &b ) {
    for ( int i= b.c.front(); i >= 1; --i ) {
        if ( i == b.c.front() ) {
            out << b[i];
            continue ;
        }
        out << std::setw(DIGL) << std::setfill('0') << b[i];
    }
    return out;
}

std::istream &operator>>(std::istream &in, bignum &b) {
    char ch;
    while ( not std::isdigit(ch= in.get()) ) ;
    std::string s{ch};
    for ( ;(ch= in.get()) != in.eof() and std::isdigit(ch); s.push_back(ch) ) ;
    b.value_of(s);
    return in;
}

bool operator<(const bignum &a, const bignum &b) {
    if ( a.length() != b.length() )
        return a.length() < b.length();
    for ( int i= a.length(); i >= 1; --i )
        if ( a[i] != b[i] ) {
            if ( a[i] < b[i] )
                return true ;
            return false ;
        }
    return false ;
}

std::vector<bignum> fibos;

int gcd( int x, int y ) { return not y ? x:gcd(y,x%y); }

int find_idx( const bignum &a ) {
    auto it= std::lower_bound(fibos.begin()+1,fibos.end(),a);
    return it-fibos.begin();
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts,i,j,k;
    fibos.emplace_back(), fibos.push_back(std::move(bignum(1ll))), fibos.push_back(std::move(bignum(1ll)));
    for ( i= 3; ; ++i ) {
        const auto &x= fibos[i-2], &y= fibos[i-1];
        fibos.push_back(x+y);
        if ( DIGL*fibos.back().length() >= 13000 )
            break ;
    }
    for ( is >> ts; ts--; ) {
        bignum a,b;
        is >> a >> b;
        i= find_idx(a), j= find_idx(b);
        assert( i != -1 );
        assert( j != -1 );
        os << fibos[gcd(i,j)] << std::endl;
    }
    return 0;
}
    