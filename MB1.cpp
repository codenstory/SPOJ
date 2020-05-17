/**
 *  MB1
 *  TOPIC: precalc, STL
 *  status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<21)
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;
bool is_prime[N];
std::vector<i64> p;

bool is_palin( const std::string &s ) {
    auto t= s;
    std::reverse(t.begin(),t.end());
    return t == s;
}

std::vector<std::pair<i64,i64>> z;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    i64 i,j,k;
    for ( is_prime[2]= true, i= 3; i < N; is_prime[i]= true, i+= 2 ) ;
    for ( i= 3; i < N; i += 2 )
        for ( j= i*i; j < N and is_prime[i]; is_prime[j]= false, j+= i ) ;
    for ( i= 0; i < N; ++i )
        if ( is_prime[i] )
            p.push_back(i);
    for ( k= 0, i= 0; i < p.size() and k < 113; ++i ) {
        std::string s;
        if ( is_palin(s= std::to_string(p[i])) ) {
            ++k;
            auto res= p[std::accumulate(s.begin(),s.end(),1ll,[](i64 acc, char ch) {
                if ( ch == '0' )
                    return acc;
                return acc*(ch-'0');
            })-1];
            z.emplace_back(p[i],res);
        }
    }
    int ts;
    for ( is >> ts; ts--; ) {
        int n;
        is >> n;
        os << z[n-1].first << " " << z[n-1].second << std::endl;
    }
    return 0;
}
    