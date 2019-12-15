/**
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (0x80)
#define oo (std::numeric_limits<unsigned int>::max())

int n;
std::map<std::string,int> name2int;
unsigned int d[N][N];

int main() {
    //std::cin.tie(nullptr), std::ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    int i,j,k,cs= 0;
    for ( int R,C ;(std::cin >> n >> C >> R) and (n or C or R); ) {
        std::vector<int> reqs;
        for ( name2int.clear(), i= 0; i < C+1; ++i ) {
            std::string tmp;
            std::cin >> tmp;
            if ( not name2int.count(tmp) ) {
                size_t sz= name2int.size();
                name2int[tmp] = sz;
            }
            if ( i ) reqs.push_back(name2int[tmp]);
        }
        for ( i= 0; i < n; ++i )
            for ( j= i+1; j < n; d[j][i]= d[i][j]= +oo, ++j ) ;
        for ( k= 0; k < R; ++k ) {
            std::string a,b,type;
            std::cin >> a >> type >> b;
            for ( const std::string &z: {a,b} ) {
                if ( not name2int.count(z) ) {
                    auto sz= name2int.size();
                    name2int[z]= sz;
                }
            }
            i= name2int[a], j= name2int[b];
            size_t len= type.size();
            std::string from= type.substr(0,2), to= type.substr(len-2,2);
            unsigned int dist= static_cast<unsigned int>(std::strtol(type.substr(2,len-4).c_str(),nullptr,10));
            assert( 1 <= dist and dist <= 1000 );
            if ( from == "--" and to == "->" ) {
                d[i][j]= std::min(d[i][j],dist);
                continue ;
            }
            if ( from == "<-" and to == "->" ) {
                d[i][j]= std::min(d[i][j],dist);
                d[j][i]= std::min(d[j][i],dist);
                continue ;
            }
            if ( from == "<-" and to == "--" ) {
                d[j][i]= std::min(d[j][i],dist);
                continue ;
            }
            assert( false );
        }
        for ( k= 0; k < n; ++k )
            for ( i= 0; i < n; ++i )
                for ( j= 0; j < n; ++j )
                    if ( d[i][k] < +oo and d[k][j] < +oo )
                        if ( d[i][j] > d[i][k]+d[k][j] )
                            d[i][j]= d[i][k]+d[k][j];
        auto ans= std::accumulate(std::begin(reqs),std::end(reqs),0ul,[&]( unsigned int acc, auto y ) {
            assert( d[0][y] < +oo and d[y][0] < +oo );
            return acc+= d[0][y]+d[y][0];
        });
        std::cout << ++cs << ". " << ans << std::endl;
    }
    return 0;
}

