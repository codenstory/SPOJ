/**
 * TOPIC: STL, precision-issues-avoiding, greedy, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
using t_coord= pair<i64,i64>;

int n;
vector<t_coord> c;

t_coord read_pair() {
    t_coord z;
    double x,y;
    cin >> x >> y;
    z.first= static_cast<i64>(1000*x);
    z.second= static_cast<i64>(1000*y);
    return z;
}

#define SQR(x) ((x)*(x))

int main() {
    int i,j,k,cs= 0,ts;
    i64 Pi= 3141ll;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( ;(cin>>n) and n; ) {
        c.clear(), c.resize(n);
        auto src= read_pair(), dst= read_pair();

        double d; i64 T; cin >> d; T= static_cast<i64>(1'000'000'000ll*d);

        for ( auto &z: c )
            z= read_pair();
        auto fnc= [&]( const t_coord &a, const t_coord &b )-> i64 {
            return Pi*(SQR(a.first-b.first)+SQR(a.second-b.second));
        };
        int ans= 0;
        vector<t_coord> vec= {src,dst};
        for ( auto t= 0; t <= 1; ++t ) {
            for (i = 0; i < n; ++i) {
                if (fnc(vec[t], c[i]) <= T) {
                    auto dist = fnc(vec[t], c[i]), TT = T-dist;
                    ans = max((int)std::count_if(c.begin(), c.end(), [&]( const t_coord &z) {
                        return fnc(vec[t], z) <= dist or fnc(vec[t^1], z) <= TT;
                    }), ans);
                }
            }
        }
        cout << ++cs << ". " << n-ans << endl;
    }
    return 0;
}

