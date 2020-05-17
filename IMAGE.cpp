/**
 * IMAGE
 * TOPIC: floodfill
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x21

std::istream &is = std::cin;
std::ostream &os = std::cout;

int m,n,sx,sy,seen[N][N],yes;
char g[N][N];

void dfs( int x, int y ) {
    assert( seen[x][y] != yes );
    seen[x][y]= yes;
    for ( int nx= x-1; nx <= x+1; ++nx )
        for ( int ny= y-1; ny <= y+1; ++ny )
            if ( 1 <= nx and nx <= m and 1 <= ny and ny <= n )
                if ( g[nx][ny] == 'X' and seen[nx][ny] != yes )
                    dfs(nx,ny);
}

int calc_contribution( int x, int y ) {
    int ans= 0;
    for ( int nx= x-1; nx <= x+1; ++nx )
        for ( int ny= y-1; ny <= y+1; ++ny )
            if ( abs(nx-x)+abs(y-ny) == 1 )
                if ( 0 <= nx and nx <= m+1 and 0 <= ny and ny <= n+1 )
                    if ( g[nx][ny] != 'X' )
                        ++ans;
    return ans;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for (;(is >> m >> n >> sx >> sy) and (m or n or sx or sy);) {
        memset(g,'.',sizeof g);
        for ( int i= 1; i <= m; ++i ) {
            std::string tmp;
            is >> tmp;
            for ( int j= 1; j <= n; ++j )
                g[i][j]= tmp[j-1];
        }
        if ( g[sx][sy] == '.' ) {
            os << "0\n";
            continue ;
        }
        ++yes, dfs(sx,sy);
        int ans= 0;
        for ( int i= 1; i <= m; ++i )
            for ( int j= 1; j <= n; ++j )
                if ( seen[i][j] == yes ) {
                    ans+= calc_contribution(i,j);
                }
        os << ans << std::endl;
    }
    return 0;
}
    