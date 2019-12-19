/**
 * TOPIC: STL, string manipulations, maths
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;

uint64_t deg[19];

pair<i64,i64> recover( const string &s, int k ) {
    assert( k >= 1 and k <= s.size() );
    int m= s.size()-k;
    std::string A= s.substr(0,m), B= s.substr(m,std::string::npos);
    uint64_t a= strtol(A.c_str(),nullptr,10), b= strtol(B.c_str(),nullptr,10);
    auto x= a*(deg[k]-1)+b, y= deg[m]*(deg[k]-1);
    auto g= __gcd(x,y);
    x/= g, y/= g;
    return {x,y};
}

int main() {
    int i,j,k;
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( deg[0]= 1ull, i= 1; i < 19; deg[i]= 10*deg[i-1], ++i ) ;
    for ( string s; (cin >> s) and s != "0"; ) {
        regex r(R"(^(0)\.(\d+)\.{3}$)");
        smatch m;
        if ( regex_match(s,m,r) ) {
            // std::cerr << m.str(1) << "\n" << m.str(2) << "\n";
            s= m.str(2);
            vector<pair<i64,i64>> vec;
            for ( k= 1; k <= s.size(); ++k )
                vec.push_back(recover(s,k));
            if ( s.back() == '9' ) {
                auto jt= find_if(s.rbegin(),s.rend(),[]( char ch ) {
                    return ch != '9';
                });
                if ( jt != s.rend() ) {
                    *jt= *jt+1;
                    s.resize(distance(jt,s.rend()));
                    s.push_back('0');
                    vec.push_back(recover(s,1));
                }
            }
            auto it= std::min_element(vec.begin(),vec.end(),[]( const auto &x, const auto &y ) {
                return x.second < y.second;
            });
            cout << it->first << "/" << it->second << endl;
        }
        else {
            throw runtime_error("unexpected input");
        }
    }
}

