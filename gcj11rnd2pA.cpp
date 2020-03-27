/**
 * TOPIC: maths, greedy
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

i64 X,S,R,t;
int n;
std::vector<std::pair<std::pair<i64,i64>,i64>> c;

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> X >> S >> R >> t >> n;
        c.resize(n);
        for ( auto &v: c )
            is >> v.first.first >> v.first.second >> v.second;
        std::sort(c.begin(),c.end());
        std::vector<std::pair<std::pair<i64,i64>,i64>> nc{};
        if ( n > 0 and c[0].first.first > 0 )
            nc.push_back({{0,c[0].first.first},0});
        if ( n > 0 and c[n-1].first.second < X )
            nc.push_back({{c[n-1].first.second,X},0});
        for ( i= 0; i < n-1; ++i ) {
            if ( c[i].first.second < c[i+1].first.first )
                nc.push_back({{c[i].first.second,c[i+1].first.first},0});
        }
        c.insert(c.end(),nc.begin(),nc.end());
        auto r= R;
        std::sort(c.begin(),c.end(),[r](auto &a, auto &b) {
            //return (a.first.second-a.first.first+0.00)/(r+a.second) >
            //       (b.first.second-b.first.first+0.00)/(r+b.second);
            return a.second < b.second;
        });
        i64 len= 0;
        for ( i= 0; i < c.size(); ++i )
            len+= c[i].first.second-c[i].first.first;
        assert( len == X );
        double ans= 0.00;
        double T= t;
        for ( i= 0; i < c.size(); ++i ) {
            //std::cerr << "[" << c[i].first.first << ", " << c[i].first.second << "]" << std::endl;
            double used_time= 0;
            if ( T > 0 ) {
                used_time= std::min(T,(c[i].first.second-c[i].first.first+0.00)/(R+c[i].second));
                T-= used_time;
                //std::cerr << "Running for " << used_time << " seconds" << std::endl;
                if ( (c[i].first.second-c[i].first.first+0.00) >= used_time*(R+c[i].second)+1e-7 ) {
                    if ( fabs(T) >= 1e-6 ) {
                        std::cerr << "T= " << T << std::endl;
                    }
                    assert( fabs(T) < 1e-7 );
                    double space_left=
                            (c[i].first.second-c[i].first.first+0.00)-used_time*(R+c[i].second);
                    used_time+= space_left/(S+c[i].second);
                    //std::cerr << "then walking for " << space_left/(S+c[i].second) << " seconds" << std::endl;
                }
            }
            else {
                assert( fabs(T) < 1e-7 );
                used_time= (c[i].first.second-c[i].first.first+0.00)/(S+c[i].second);
                //std::cerr << "Walking for " << used_time << " seconds" << std::endl;
            }
            ans+= used_time;
        }
        //std::cerr << "Done" << std::endl;
        os << "Case #" << ++cs << ": " << std::setprecision(7) << std::fixed << ans << std::endl;
    }
    return 0;
}   
 