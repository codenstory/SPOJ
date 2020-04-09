/**
 * TOPIC: ad hoc, inequalities
 * status: Accepted
 */
#include <bits/stdc++.h>
using std::vector;
using std::pair;
using std::string;
#define N (1<<21)

std::istream &is = std::cin;
std::ostream &os = std::cout;
double C, F, X;
double pref[N];

double g( int t ) {
    if ( t == 0 )
        return X/2;
    return pref[t]+X/(2+t*F);
}

double f() {
    if ( X <= C )
        return X/2.00;
    int t= static_cast<int>(ceil(X/C-2/F-1));
    double ans= X/2.00;
    pref[0]= 0;
    for ( int i= 1; i < N; ++i )
        pref[i]= pref[i-1]+C/(2+(i-1)*F);
    for ( int i= std::max(t-10,0); i < N; ++i )
        ans= std::min(ans,g(i));
    return ans;
}

void solve(int cs) {
    os << "Case #" << cs << ": ";
    is >> C >> F >> X;
    os << std::setprecision(7) << std::fixed << f() << std::endl;
}

int main() {
    int cs = 0, ts;
    for (is >> ts; ts--; solve(++cs));
    return 0;
}   
 