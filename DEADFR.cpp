#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;

pair<i64,i64> recover( const string &s, int k ) {
    assert( k >= 1 and k <= s.size() );
}

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( string s; (cin >> s) and s != ""; ) {
        regex r(R"(^(0)\.(\d+)\.{3}$)");
        smatch m;
        if ( regex_match(s,m,r) ) {
            // std::cerr << m.str(1) << "\n" << m.str(2) << "\n";
            s= m.str(2);
        }
        else {
            throw runtime_error("unexpected input");
        }
    }
}

