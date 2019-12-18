/**
 * TOPIC: interviews, continued fractions, loops, invariants, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
#define N 0x400

int m,n;
vector<string> g;

int main() {
    int i,j,k,t;
    string s;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( ;getline(cin,s); ) {
        sscanf(s.c_str(),"%d %d",&m,&n);
        if ( m == 0 and n == 0 ) break ;
        assert( m < N and n < N );
        g.resize(m);
        for ( auto &x: g )
            getline(cin,x);
        regex r("(\\d+)");
        int64_t x= 1ll, y= 1ll;
        for ( t= 0, i= m-2; i >= 0; i-= 2 ) {
            smatch mtch;
            if ( regex_search(g[i],mtch,r) ) {
                auto num= strtol(mtch.str(1).c_str(),nullptr,10);
                auto tmp= num*y+x;
                x= tmp, swap(x,y);
            }
        }
        swap(x,y);
        cout << x << " " << y << endl;
    }
}
