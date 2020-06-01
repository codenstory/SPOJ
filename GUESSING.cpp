/**
 * GUESSING
 * TOPIC: simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 6

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::set<std::string> checked;
std::vector<std::pair<std::string,std::pair<int,int>>> previous;

void g( int cur, std::string &s, const unsigned int u, const std::vector<char> &forbidden, std::vector<std::string> &res ) {
    if ( cur == N ) {
        res.push_back(s);
        return ;
    }
    if ( (u>>cur) & 1u ) {
        assert( forbidden[cur] != '$' );
        for (auto ch = '0'; ch <= '9'; ++ch) {
            if (forbidden[cur] != ch) {
                s[cur] = ch, g(cur + 1, s, u, forbidden, res);
            }
            s[cur]= '$';
        }
    } else {
        g(cur+1,s,u,forbidden,res);
    }
}

bool consistent( const std::string &x, const std::string &s, int matches, int mismatches ) {
    int i,j,k;
    for ( k= 0, i= 0; i < N and k <= matches; ++i )
        if ( x[i] == s[i] and ++k ) ;
    if ( k != matches )
        return false ;
    for ( k= 0, i= 0; i < N and k <= mismatches; ++i )
        for ( j= 0; j < N and k <= mismatches; ++j )
            if ( i != j )
                if ( x[i] == s[j] and ++k ) ;
    return k == mismatches;
}

bool consistentWithAllPrevious( const std::string &x ) {
    for ( const auto &z: previous )
        if ( not consistent(x,z.first,z.second.first,z.second.second) )
            return false ;
    return true ;
}

std::vector<std::string> getAllConsistent( const std::string &s, int matches, int mismatches ) {
    unsigned int i,j,k;
    std::vector<std::string> ans{};
    for ( unsigned u= 0; u < (1u<<N); ++u )
        if ( __builtin_popcount(u) == matches ) {
            std::string t(N,'$');
            for ( i= 0; i < N; ++i )
                if ( (u>>i) & 1u )
                   t[i]= s[i];
            auto v= (~u)&((1u<<N)-1u);
            std::vector<char> forbidden(N,'$');
            for ( j= 0; j < N; ++j )
                if ( (v>>j) & 1u ) {
                    forbidden[j]= s[j];
                }

            std::vector<std::string> res{};
            std::string cand(N,'$');
            g(0,cand,v,forbidden,res);

            for ( auto &z: res ) {
                std::string x= t;
                for ( i= 0; i < N; ++i )
                    if ( z[i] != '$' ) {
                        assert( x[i] == '$' );
                        x[i]= z[i];
                    }
                if ( consistent(x,s,matches,mismatches) )
                    ans.push_back(x);
            }
        }
    return ans;
}

bool f( const std::string &s ) {
    if ( checked.count(s) )
        return false ;
    os << "What about " << s << std::endl;
    checked.insert(s);
    std::string feedback;
    is >> feedback;
    if ( feedback[0]-'0' == N ) {
        os << "The answer is " << s << std::endl;
        return true ;
    }
    int mm= std::strtol(feedback.substr(2).c_str(),nullptr,10);
    os << "Read feedback " << feedback << " with " << feedback[0]-'0' << " matches and " << mm << " mismatches" << std::endl;
    previous.emplace_back(s,std::make_pair(feedback[0]-'0',mm));
    auto candidates= getAllConsistent(s,feedback[0]-'0',mm);
    for ( auto &t: candidates ) {
        if ( consistentWithAllPrevious(t) )
            if (t != s and (not checked.count(t)) and f(t))
                return true;
    }
    checked.insert(s);
    return false ;
}

int main() {
    const std::string initial_guess= "123456";
    assert( f(initial_guess) );
    return 0;
}
    