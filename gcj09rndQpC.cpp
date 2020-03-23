/**
 * TOPIC: DP
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x200
#define M 20
using i64= std::int64_t;
#define MOD (10000ll)

i64 z[N][M],e[N][M];
const std::string welcome= "welcome to code jam";

int main() {
    int i,j,k,cs= 0,ts,n,m= welcome.size();
    std::istream &is= std::cin;
    std::string tmp;
    std::getline(is,tmp);
    ts= std::strtol(tmp.c_str(),nullptr,10);
    for ( ;ts--; ) {
        std::string text;
        std::getline(is,text), n= text.size();
        memset(z,0,sizeof z);
        memset(e,0,sizeof e);
        for ( i= 1; i <= n; ++i ) {
            if ( text[i-1] == welcome[0] )
                z[i][1]= 1;
            e[i][1]= e[i-1][1]+z[i][1];
        }
        for ( i= 1; i <= n; ++i )
            for ( j= 2; j <= m; ++j ) {
                if (text[i-1] == welcome[j-1]) {
                    z[i][j]= e[i-1][j-1];
                }
                e[i][j]= e[i-1][j]+z[i][j], e[i][j]%= MOD;
            }
        std::cout << "Case #" << ++cs << ": " << std::setfill('0') << std::setw(4) << e[n][m] << std::endl;
    }
    return 0;
}



