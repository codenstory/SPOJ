/**
 * RLM
 * TOPIC: modeling
 * status: Accepted
 */
#include <bits/stdc++.h>
using u64= std::uint64_t;

struct num {
    std::vector<std::pair<int,int>> v;
    num() { v.clear(); }
    num( u64 x ) {
        std::stringstream s;
        s << x;
        auto sx= s.str();
        v.clear();
        for ( int i= 0,j; i < sx.size(); i= j ) {
            for ( j= i+1; j < sx.size() and sx[i] == sx[j]; ++j )
                if ( j-i == 9 )
                    break ;
            v.emplace_back(j-i,sx[i]-'0');
        }
    }
    u64 operator()() const {
        u64 res= 0;
        for ( int i= 0; i < v.size(); ++i ) {
            for ( int k= v[i].first; k--; ) {
                res*= 10, res+= v[i].second;
            }
        }
        return res;
    }
};

std::istream &operator >> ( std::istream &is, num &n ) {
    std::string s;
    is >> s;
    n.v.clear();
    for ( int i= 0; i < s.size(); i+= 2 )
        n.v.emplace_back(s[i]-'0',s[i+1]-'0');
    return is;
}

std::ostream &operator << ( std::ostream &os, const num &n ) {
    for ( int i= 0; i < n.v.size(); ++i )
        os << n.v[i].first << n.v[i].second;
    return os;
}

std::regex R= std::regex(R"((\d+)\s*([-+*/])\s*(\d+))");

int main() {
    for ( std::string s; std::getline(std::cin,s); ) {
        std::smatch m;
        if ( std::regex_search(s,m,R) ) {
            std::istringstream a(m.str(1)), b(m.str(3));
            num x,y;
            a >> x, b >> y;
            //std::cerr << m.str(1) << " and " << m.str(3) << std::endl;
            //std::cerr << x() << " and " << y() << std::endl;
            u64 res= 0;
            switch ( m.str(2)[0] ) {
                case '+': res= x()+y(); break ;
                case '-': res= x()-y(); break ;
                case '*': res= x()*y(); break ;
                case '/': res= x()/y(); break ;
                default: assert( false );
            }
            std::cout << s << " = " << num(res) << std::endl;
        } else {
            assert( false );
        }
    }
    return 0;
}
