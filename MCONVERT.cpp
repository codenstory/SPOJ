/**
 * MCONVERT
 * TOPIC: number bases, bigint
 * status:
 */
#include <bits/stdc++.h>
enum { BASE= 100000LL, DIGL= 5 };
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

class bigint {
private:
    const std::map<char,int> &rank;
    const std::map<int,char> &dig;
    std::vector<i64> c;
    i64 operator []( int i ) const {
        return c[i];
    }
    i64 &operator[] ( int i ) {
        return c[i];
    }
public:

    bigint( const std::map<char,int> &rank, const std::map<int,char> &dig ) : rank(rank), dig(dig) {
        c= std::vector<i64>{1,0};
    }

    bigint interpret( const std::string &s, i64 base ) const {
        bigint b(rank,dig);
        std::vector<i64> z{};
        z.push_back(s.size());
        std::transform(s.begin(),s.end(),std::back_inserter(z),[this](char ch) {
            assert( rank.count(ch) );
            return rank.find(ch)->second;
        });
        return b;
    }

    void operator *= ( const bigint &other ) {
    }

    void operator += ( const bigint &other ) {
        std::vector<i64> z(std::max(c[0],other[0])+3,0);
        int i,j;
        for ( i= 1; i <= c[0] and i <= other[0]; ++i )
            z[i]= (*this)[i]+other[i];
        for (;i <= c[0]; z[i]= (*this)[i], ++i ) ;
        for (;i <= other[0]; z[i]= other[i], ++i ) ;
        z[0]= std::max(c[0],other[0])+1;
        for ( i= 1; i <= z[0]; ++i )
            if ( z[i] >= BASE )
                z[i+1]+= z[i]/BASE, z[i]%= BASE;
        for ( i= z[0]; i >= 1 and not z[i]; --i ) ;
        z[0]= (not i)?1:i;
        std::swap(c,z);
    }

    friend std::ostream &operator << ( std::ostream &, const bigint & ) ;

};

std::ostream &operator<<(std::ostream &o, const bigint &z ) {
    for ( int i= z[0]; i >= 1; --i )
        o << std::setfill('0') << std::setw(DIGL) << z[i];
    return o;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,cs= 0;
    for ( std::string s; std::getline(is,s) and not s.empty(); ) {
        if ( ++cs > 1 ) os << std::endl;
        std::map<char,int> rank;
        std::map<int,char> dig;
        for ( dig.clear(), rank.clear(), i= 0; i < s.size(); ++i )
            dig[rank[s[i]]= i]= s[i];
        int B= s.size(), LB;
        for ( std::string t; std::getline(is,t) and not t.empty(); ) {
            bigint b(rank,dig);
            LB= rank[*std::max_element(t.begin(),t.end(),[&rank](char x, char y) {
                assert( rank.count(x) );
                assert( rank.count(y) );
                return rank[x] < rank[y];
            })];
            for ( auto base= LB+1; base <= B; b+= b.interpret(t,base++) ) ;
            std::stringstream str;
            str << b;
            auto res= str.str();
            auto it= std::find_if(res.begin(),res.end(),[](char ch) {
                return ch != '0';
            });
            if ( it == res.end() ) {
                os << '0' << std::endl;
            } else {
                os << res.substr(it-res.begin()) << std::endl;
            }
        }
    }
    return 0;
}
    