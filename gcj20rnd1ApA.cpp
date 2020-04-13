/**
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>
using std::vector;
using std::pair;
using std::string;

std::istream &is = std::cin;
std::ostream &os = std::cout;
std::vector<std::string> p;
std::vector<int> pref, suff;
int n;

bool starts_with( const std::string &x, const std::string &y ) {
    assert( x.size() >= y.size() );
    return x.substr(0,y.size()) == y;
}

bool ends_with( const std::string &x, const std::string &y ) {
    assert( x.size() >= y.size() );
    return x.substr(x.size()-y.size()) == y;
}

std::string find_prefix( bool &ok ) {
    if ( not ok ) return "";
    std::string prefix{};
    for ( int i= 0; i < n; ++i ) {
        if ( pref[i] > 0 ) {
            std::string a= p[i].substr(0,pref[i]);
            if ( prefix.empty() )
                prefix= a;
            else {
                if ( a.size() <= prefix.size() ) {
                    if ( starts_with(prefix,a) ) continue ;
                    ok= false;
                    return "";
                }
                else {
                    if ( starts_with(a,prefix) ) {
                        prefix= a;
                        continue ;
                    }
                    ok= false;
                    return "";
                }
            }
        }
    }
    return prefix;
}

std::string find_suffix( bool &ok ) {
    if ( not ok ) return "";
    std::string suffix{};
    for ( int i= 0; i < n; ++i ) {
        if ( suff[i]+1 == p[i].size() ) continue ;
        assert( suff[i]+1 < p[i].size() );
        auto a= p[i].substr(suff[i]+1);
        if ( suffix.empty() ) {
            suffix= a;
            continue ;
        }
        if ( a.size() <= suffix.size() ) {
            if ( ends_with(suffix,a) ) continue ;
            ok= false ;
            return "";
        }
        else {
            if ( ends_with(a,suffix) ) {
                suffix= a;
                continue ;
            }
            ok= false ;
            return "";
        }
    }
    return suffix;
}

std::string accommodate( int i ) {
    std::string res{};
    for ( int j= pref[i]+1; j <= suff[i]-1; ++j )
        if ( p[i][j] != '*' )
            res+= p[i][j];
    return res;
}

void solve(int cs) {
    os << "Case #" << cs << ": ";
    int i,j,k;
    is >> n;
    p.resize(n);
    for ( auto &v: p )
        is >> v;
    pref.resize(n), suff.resize(n);
    for ( i= 0; i < n; ++i ) {
        pref[i] = p[i].find('*');
        assert( pref[i] != std::string::npos );
        assert( p[i][pref[i]] == '*' );
        for ( j= ((int)p[i].size())-1; j >= 0 and p[i][j] != '*'; --j ) ;
        assert( j >= 0 );
        suff[i]= j;
        assert( p[i][suff[i]] == '*' );
    }
    bool ok= true ;
    auto prefix= find_prefix(ok);
    //std::cerr << "prefix= " << prefix << std::endl;
    //std::cerr << ok << std::endl;
    auto suffix= find_suffix(ok);
    //std::cerr << "suffix= " << suffix << std::endl;
    //std::cerr << ok << std::endl;
    if ( not ok ) {
        os << "*" << std::endl;
        return ;
    }
    //std::cerr << "prefix= " << prefix << ", suffix= " << suffix << std::endl;
    std::string result= prefix;
    for ( i= 0; i < n; ++i )
        result+= accommodate(i);
    result+= suffix;
    if ( result.size() <= 10000 )
        os << result << std::endl;
    else os << "*" << std::endl;
}

int main() {
    int cs = 0, ts;
    assert( ends_with("abra","ra") );
    assert( not ends_with("abra","xa") );
    assert( ends_with("abra","") );
    assert( starts_with("abra","ab") );
    assert( starts_with("abra","") );
    assert( not starts_with("abra","ax") );
    for (is >> ts; ts--; solve(++cs));
    return 0;
}   
 