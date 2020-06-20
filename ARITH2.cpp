/**
 * TOPIC: STL, regex, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
using t_coord= pair<i64,i64>;

string next_token( istream &is= cin ) {
    stringstream s;
    while ( is.peek() != EOF and isspace(is.peek()) and is.peek() != '\n' ) is.get();
    if ( is.peek() == EOF )
        return "";
    if ( is.peek() == '\n' ) {
        is.get();
        return "\n";
    }
    for ( s << (char)is.get(); is.peek() != EOF and (not isspace(is.peek())) and is.peek() != '\n'; s << (char)is.get() ) ;
    return s.str();
}

int main() {
    int i,j,k,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    auto tok= next_token();
    ts= strtol(tok.c_str(),nullptr,10);
    tok= next_token();
    assert( tok == "\n" );
    while ( ts-- ) {
        string T{};
        for ( vector<string> tokens; tokens.empty(); ) {
            for (string t; (not(t = next_token()).empty()) and t != "\n"; tokens.push_back(std::move(t)));
            string a{};
            auto acc = std::accumulate(tokens.begin(), tokens.end(), a, [](string z, string t) {
                return z += t;
            });
            if ( not acc.empty() )
                T= acc;
        }
        assert( not T.empty() );
        regex r[]= {regex("([^\\d=])"),regex("(\\d+)")};
        vector<string> tk[2];
        for ( auto t= 0; t <= 1; ++t ) {
            smatch m;
            for (string str = T; regex_search(str, m, r[t]);) {
                assert(not m.empty());
                tk[t].push_back(m[0]);
                str = m.suffix();
            }
        }
        int64_t res= strtol(tk[1].front().c_str(),nullptr,10);
        std::vector<pair<int64_t,char>> results;
        transform(next(tk[1].begin()),tk[1].end(),tk[0].begin(),back_inserter(results),[&](const string &a, const string &b)->pair<int64_t,char>{
            if ( b == "+" )
                return {strtol(a.c_str(),nullptr,10),'+'};
            if ( b == "-" )
                return {strtol(a.c_str(),nullptr,10),'-'};
            if ( b == "/" )
                return {strtol(a.c_str(),nullptr,10),'/'};
            if ( b == "*" )
                return {strtol(a.c_str(),nullptr,10),'*'};
            assert( false );
        });
        cout << accumulate(results.begin(),results.end(),res,[]( int64_t acc, const pair<int64_t,char> &z ) {
            if ( z.second == '+' )
                return acc+z.first;
            if ( z.second == '-' )
                return acc-z.first;
            if ( z.second == '/' )
                return acc/z.first;
            if ( z.second == '*' )
                return acc*z.first;
        }) << endl;
    }
    return 0;
}

