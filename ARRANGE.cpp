/**
 * ARRANGE
 * TOPIC: sorting, greedy, special cases
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;

int n;
vector<i64> c;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    int i,j,k,ts;
    for ( cin >> ts; ts--; ) {
        cin >> n, c.resize(n);
        for ( auto &x: c ) cin >> x;
        std::vector<i64> ones;
        std::copy_if(c.begin(),c.end(),std::back_inserter(ones),[](auto x) { return x == 1;});
        auto it= std::remove_if(c.begin(),c.end(),[](auto x){ return x == 1;});
        c.resize(distance(c.begin(),it));
        sort(c.begin(),c.end(),[](auto x,auto y){
            return x>y;
        });
        bool is_first= true ;
        for ( auto x: ones ) {
            if ( not is_first )
                cout << ' ';
            cout << 1;
            is_first= false ;
        }
        if ( c.size() == 2 and c[0] == 3 and c[1] == 2 ) {
            if ( is_first )
                cout << "2 3" << endl;
            else
                cout << " 2 3" << endl;
        }
        else {
            for ( auto x: c ) {
                if ( not is_first )
                    cout << ' ';
                cout << x;
                is_first= false ;
            }
            cout << endl;
        }
    }
    return 0;
}
