/**
 * TOPIC: DP
 */
#include <bits/stdc++.h>
#define N 0x200
#define M 19
using i64= std::int64_t;
#define MOD (10000ll)

i64 z[N][M];
const std::string welcome= "welcome to code jam";

int main() {
    int i,j,k,cs= 0,ts,n,m= welcome.size();
    std::istream &is= std::cin;
    for ( is >> ts; ts--; ) {
        std::string text;
        std::getline(is,text), n= text.size();
        memset(z,0,sizeof z);
        for ( i= 0; i <= n; ++i )
            z[i][0]= 1;
        for ( i= 1; i <= n; ++i )
            for ( j= 1; j <= m; ++j )
                if ( text[i-1] == welcome[j-1] )
                    z[i][j]+= z[i-1][j-1], z[i][j]%= MOD;
        i64 ans= 0;
        for ( i= 1; i <= n; ++i )
            ans+= z[i][M], ans%= MOD;
        std::cout << "Case #" << ++cs << ": " << ans << std::endl;
    }
    return 0;
}


