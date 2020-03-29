/**
 * TOPIC: ad hoc
 * status: Accepted
 */
#include <bits/stdc++.h>

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;

    std::vector<std::string> in= {"ejp mysljylc kd kxveddknmc re jsicpdrysi",
                                  "rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd",
                                  "de kr kd eoya kw aej tysr re ujdr lkgc jv"
    };

    std::vector<std::string> out= {"our language is impossible to understand",
                                   "there are twenty six factorial possibilities",
                                   "so it is okay if you want to just give up"
    };
    std::map<char,char> m;
    std::vector<std::pair<std::string,std::string>> io{};
    std::transform(in.begin(),in.end(),out.begin(),std::back_inserter(io),[](auto &s, auto &t) {
        return std::make_pair(s,t);
    });
    std::set<char> A,B;
    for ( auto &[x,y]: io ) {
        assert( x.size() == y.size() );
        for ( i= 0; i < x.size(); ++i ) {
            assert( not m.count(x[i]) or m[x[i]] == y[i] );
            if ( x[i] == ' ' ) {
                assert( y[i] == ' ' );
                continue ;
            }
            m[x[i]]= y[i];
            A.insert(x[i]), B.insert(y[i]);
        }
    }
    /*
    for ( auto [x,y]: m )
        std::cerr << x << "->" << y << std::endl;
    */
    m['q']= 'z', m['z']= 'q';
    m[' ']= ' ';
    std::string s;
    std::getline(is,s);
    for ( ts= std::strtol(s.c_str(),nullptr,10); ts--; ) {
        std::getline(is,s);
        std::string t{};
        std::transform(s.begin(),s.end(),std::back_inserter(t),[&m](auto ch) {
            if ( not m.count(ch) ) {
            }
            assert( m.count(ch) );
            return m[ch];
        });
        os << "Case #" << ++cs << ": " << t << std::endl;
    }
    return 0;
}   
 