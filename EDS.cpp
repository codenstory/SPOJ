/**
 * TOPIC: ad hoc, simulation, regex
 * status: Accepted
 */
#include <bits/stdc++.h>

class logmanager {
private:
    using type_right= char;
    using entity_type= char;
    unsigned int parse_rights( const std::string &s ) {
        return std::accumulate(s.begin(),s.end(),0u,[](unsigned int acc, char ch) {
            if ( not('a' <= ch and ch <= 'z') ) {
                return acc;
                //std::cerr << "This violates: " << ch << std::endl;
            }
            assert( 'a' <= ch and ch <= 'z' );
            return acc | (1u << (ch - 'a'));
        });
    }
    std::map<entity_type,unsigned int> m;
    std::regex reg_sign= std::regex(R"(([+-=]))");
public:
    logmanager() { m.clear(); }
    void parse_log_entry( const std::string &s ) {
        std::smatch smat;
        if ( std::regex_search(s,smat,reg_sign) ) {
            auto which_op= smat.str(1).front();
            auto pos= smat.position();
            auto E= s.substr(0,pos);
            auto u= parse_rights(s.substr(s.find(which_op)+1));
            /*std::cerr << "operation: " << smat.str(1) << std::endl;
            std::cerr << "applying rights: " << s.substr(pos+1) << " to " << E << std::endl;*/
            //std::cerr << "applying rights: " << s.substr(pos+1) << " to " << E << std::endl;
            std::for_each(E.begin(),E.end(),[u,which_op,this]( entity_type ch ) {
                if ( not m.count(ch) ) m[ch]= 0;
                switch ( which_op ) {
                    case '+': m[ch]|= u; break ;
                    case '-': m[ch]&= ~u; break ;
                    case '=': m[ch]= u; break ;
                    default: assert( false );
                }
            });
        }
    }
    static std::string display( unsigned int u ) {
        std::string res{};
        static std::map<unsigned int,std::string> mm{};
        if ( mm.count(u) )
            return mm[u];
        for ( int i= 0; i < 26; ++i )
            if ( (u>>i)&1u )
                res.push_back(i+'a');
        return mm[u]= res;
    }
    friend std::ostream &operator << ( std::ostream &os, const logmanager &l ) ;
};

std::ostream &operator<<( std::ostream &os, const logmanager &l ) {
    std::map<char,unsigned int> m{};
    std::for_each(l.m.begin(),l.m.end(),[&m](const auto &it) {
        if ( it.second )
            m[it.first]= it.second;
    });
    for ( auto it= m.begin(), jt= it; it != m.end(); it= jt ) {
        for ( jt= it; jt != m.end() and jt->second == it->second; ++jt ) ;
        if ( it->second ) {
            for ( auto kt= it; kt != jt; ++kt )
                os << kt->first;
            os << logmanager::display(it->second);
        }
    }
    return os;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    int cs= 0;
    std::regex r= std::regex(R"(([^,]+))");
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    for ( std::string s; std::getline(is,s) and s.front() != '#'; ) {
        logmanager lm;
        for ( std::smatch m; std::regex_search(s,m,r); ) {
            //std::cerr << "Match: " << m.str(1) << std::endl;
            lm.parse_log_entry(m.str(1)), s = m.suffix();
        }
        os << ++cs << ":" << lm << std::endl;
    }
    return 0;
}

