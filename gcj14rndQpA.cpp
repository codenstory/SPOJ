/**
 * TOPIC: ad hoc, STL
 * status: Accepted
 */
#include <bits/stdc++.h>
using std::vector;
using std::pair;
using std::string;
#define N 16

std::istream &is = std::cin;
std::ostream &os = std::cout;


void solve(int cs) {
    os << "Case #" << cs << ": ";
    int i,j,k,s[2],t;
    vector<int> a[2];
    for ( t= 0; t <= 1; ++t ) {
        a[t].resize(N), is >> s[t], --s[t];
        for ( auto &v: a[t] )
            is >> v;
    }
    int ans, num_candidates= std::accumulate(a[0].begin()+s[0]*4,a[0].begin()+s[0]*4+4,0,[&ans,&a,s]( int acc, auto candidate ) {
        if ( std::count(a[1].begin()+s[1]*4,a[1].begin()+s[1]*4+4,candidate) )
            ans= candidate, ++acc;
        return acc;
    });
    if ( not num_candidates ) {
        os << "Volunteer cheated!" << std::endl;
        return ;
    }
    if ( num_candidates > 1 ) {
        os << "Bad magician!" << std::endl;
        return ;
    }
    os << ans << std::endl;
}

int main() {
    int cs = 0, ts;
    for (is >> ts; ts--; solve(++cs));
    return 0;
}   
 