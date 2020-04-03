/**
 * TOPIC: constructive, greedy, divide and conquer
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { FROM_E, FROM_R };
#define oo (1ll<<60)

std::istream &is = std::cin;
std::ostream &os = std::cout;

i64 E,R;
int n;
std::vector<i64> v;
std::vector<i64> z;

i64 calc_z( int i, int j, i64 a, i64 b ) {
    if ( i >=j ) return 0;
    auto mxit= std::max_element(v.begin()+i,v.begin()+j);
    int idx= mxit-v.begin();
    i64 tope= std::min(a+(idx-i)*R,E), bote= std::max(static_cast<i64>(0),b-(j-idx)*R);
    return calc_z(i,idx,a,tope) + (tope-bote)*v[idx] + calc_z(idx+1,j,bote+R,b);
}

void solve( int cs ) {
    int t;
    is >> E >> R >> n, v.resize(n);
    for ( auto &x: v) is >> x;
    R= std::min(R,E);
    z.resize(n);
    os << "Case #" << cs << ": " << calc_z(0,n,E,0) << std::endl;
}

void solve2( int cs ) {
    int t;
    is >> E >> R >> n, v.resize(n);
    for ( auto &x: v) is >> x;
    R= std::min(R,E);
    z.resize(n);
    std::stack<int> st{};
    std::vector<int> dp(n+1);
    for ( dp[n-1]= -1, st.push(t= n-1); --t >= 0; dp[t]= st.empty()?-1:st.top(), st.push(t) )
        for ( ;not st.empty() and v[st.top()] <= v[t]; st.pop() ) ;
    i64 ans= 0, cur_level= E;
    for ( int i= 0,j; i < n; ++i ) {
        if ( (j= dp[i]) == -1 ) {
            ans+= cur_level*v[i], cur_level= R;
            continue ;
        }
        if ( std::min(E,cur_level+(j-i)*R) >= E ) {
            auto excess= std::min(cur_level,std::min(E,cur_level+(j-i)*R-E));
            ans+= excess*v[i], cur_level-= excess, cur_level+= R;
            cur_level= std::min(E,cur_level);
            continue ;
        }
        cur_level+= R, cur_level= std::min(E,cur_level);
    }
    os << "Case #" << cs << ": " << ans << std::endl;
}

int main() {
    int cs= 0, ts;
    for ( is >> ts; ts--; solve2(++cs) ) ;
    return 0;
}   
 