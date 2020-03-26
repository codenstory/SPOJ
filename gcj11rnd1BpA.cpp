/**
 * TOPIC: ad hoc, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x80

int n;
std::vector<std::string> g;
std::vector<double> wp,owp,oowp;

int main() {
    int i,j,k,t,cs= 0,ts;
    std::string L;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n;
        g.resize(n), wp.resize(n), owp.resize(n), oowp.resize(n);
        for ( auto &v: g )
            is >> v;
        for ( i= 0; i < n; ++i )
            wp[i]= std::count(g[i].begin(),g[i].end(),'1')/(.0+std::count_if(g[i].begin(),g[i].end(),[](auto ch){
                return ch != '.';
            }));
        for ( i= 0; i < n; owp[i++]/= t )
            for ( t= 0, owp[i]= 0, j= 0; j < n; ++j )
                if ( g[i][j] != '.' ) {
                    assert( i != j );
                    double wins= 0, games= 0;
                    for ( ++t, k= 0; k < n; ++k )
                        if ( g[j][k] != '.' and k != i ) {
                            if ( g[j][k] == '1' )
                                ++wins;
                            ++games;
                        }
                    owp[i]+= wins/games;
                }
        for ( i= 0; i < n; oowp[i++]/= k )
            for ( oowp[i]= 0, k= 0, j= 0; j < n; ++j )
                if ( g[i][j] != '.' )
                    oowp[i]+= owp[j], ++k;
        os << "Case #" << ++cs << ":\n";
        for ( i= 0; i < n; ++i )
            os << std::fixed << 0.25*wp[i] + 0.50*owp[i] + 0.25*oowp[i] << std::endl;
    }
    return 0;
}

