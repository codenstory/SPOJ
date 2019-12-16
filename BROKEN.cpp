/**
 * TOPIC: caterpillar method, two pointers, interviews, maintaining invariants
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;

int freq[256];

int main() {
    int i,j,k;
    string s;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    while ( getline(cin,s) ) {
        int m= strtol(s.c_str(),nullptr,10);
        if ( not m ) break ;
        getline(cin,s);
        int n= s.size();
        assert( m >= 1 );
        int cur_distinct= 0, ans= numeric_limits<int>::min();
        i= j= 0, memset(freq,0,sizeof freq);
        do {
            assert( i <= j and cur_distinct <= m );
            for (;j < n; ++freq[s[j++]] )
                if ( not freq[s[j]] ) {
                    if ( cur_distinct == m ) break ;
                    ++cur_distinct;
                }
            for ( ans= max(ans,j-i); i < j and cur_distinct == m; ++i )
                if ( not --freq[s[i]] )
                    --cur_distinct;
        } while ( j < n );
        cout << ans << endl;
    }
    return 0;
}
