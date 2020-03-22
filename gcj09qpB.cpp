/**
 * Problem B: Watersheds
 * TOPIC: dfs, directed graphs
 * NOTE: "Every cell that drains directly or indirectly to the same sink is part of the same drainage basin."
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x80
#define e(x,y) ((x)|((y)<<7))
#define vc(x,y) (0<=(x) and (x)<m and 0<=(y) and (y)<n)
enum { D, T };

int m,n,g[N][N],G[N][N],mark;
int dx[]= {-1,0,0,1},
    dy[]= {0,-1,1,0};
std::vector<int> adj[2][N*N];
std::vector<int> sources,sinks;
bool seen[N*N];
char grid[N][N];
char label;
std::map<int,char> mp;

void dfs( int z ) {
    assert( not seen[z] );
    seen[z]= true, G[z&127][z>>7]= mark;
    for ( auto nz: adj[T][z] )
        if ( not seen[nz] )
            dfs(nz);
}

int main() {
    int i,j,k,t,cs= 0,ts,u,v;
    std::istream &is= std::cin;
    for ( is >> ts; ts--; ) {
        is >> m >> n;
        for ( i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j )
                is >> g[i][j];
        for ( i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j )
                for ( t= D; t <= T; ++t )
                    adj[t][e(i,j)].clear(), seen[e(i,j)]= false;
        for ( i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j ) {
                int best= g[i][j]+1,xx,yy;
                for ( k= 0; k < 4; ++k ) {
                    auto ni= i+dx[k], nj= j+dy[k];
                    if ( vc(ni,nj) and g[ni][nj] < best )
                        best= g[xx= ni][yy= nj];
                }
                if ( best < g[i][j] ) {
                    adj[D][u= e(i,j)].push_back(v= e(xx,yy));
                    adj[T][v].push_back(u);
                }
            }
        for ( mark= 0, sinks.clear(), i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j )
                if ( adj[D][e(i,j)].empty() )
                    sinks.emplace_back(e(i,j));
        for ( auto z: sinks ) {
            assert( not seen[z] );
            dfs(z), ++mark;
        }
        for ( label= 'a', mp.clear(), i= 0; i < m; ++i )
            for ( j= 0; j < n; ++j )
                if ( mp.count(G[i][j]) ) {
                    grid[i][j]= mp[G[i][j]];
                }
                else {
                    grid[i][j]= mp[G[i][j]]= label++;
                }
        std::ostream &os= std::cout;
        os << "Case #" << ++cs << ":" << std::endl;
        for ( i= 0; i < m; ++i, os << '\n' )
            for (os << grid[i][0], j = 1; j < n; os << ' ' << grid[i][j++] );
    }
    return 0;
}
