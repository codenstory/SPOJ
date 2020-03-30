/**
 * TOPIC: DP
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;
using i64= std::int64_t;

std::vector<i64> d,l;
int n;
i64 D;

struct state {
    int idx;
    i64 len;
};

bool operator < ( const state &a, const state &b ) {
    return a.len > b.len or (a.len == b.len and a.idx < b.idx);
}

void solve( int cs ) {
    int i,j;
    i64 swing;

    is >> n, d.resize(n), l.resize(n);
    for ( i= 0; i < n; ++i ) {
        is >> d[i] >> l[i];
    }
    is >> D, d.push_back(D), l.push_back(0);
    std::set<state> s{};
    std::vector<std::optional<i64>> max_swing(n+1,std::nullopt);
    for ( s.insert({0,*(max_swing[0]= d[0])}); not s.empty(); ) {
        auto it= s.begin();
        i= it->idx, s.erase(it);
        if ( i == n ) {
            os << "Case #" << cs << ": YES\n";
            return ;
        }
        assert( i < n );
        for ( auto jt= std::upper_bound(d.begin()+i+1,d.end(),d[i]+*max_swing[i]); --jt != d.begin()+i; ) {
            j= jt-d.begin(), swing= std::min(l[j],d[j]-d[i]);
            // if ( d[j]-d[i] > l[j] ) continue ;
            if ( not max_swing[j] or *max_swing[j] < swing ) {
                //std::cerr << "Liana " << j << " is reachable from " << i << ", new swing for j is " << swing << std::endl;
                s.insert({j, *(max_swing[j] = swing)});
            }
        }
    }
    assert( not max_swing[n] );
    os << "Case #" << cs << ": NO\n";
}

int main() {
    int cs = 0, ts;
    for ( is >> ts; cs < ts; solve(++cs) ) ;
    return 0;
}   
 