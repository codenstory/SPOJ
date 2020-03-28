/**
 * TOPIC: barycenters
 * status: Accepted
 */
#include <bits/stdc++.h>
#define tol (1e-7)
using i64= std::int64_t;
#define N 0x200

int m,n;
std::vector<std::string> grid;
double g[N][N],z[3][N][N],D;

double sum( int t, int i1, int j1, int i2, int j2 ) {
    return z[t][i2][j2]-z[t][i1-1][j2]-z[t][i2][j1-1]+z[t][i1-1][j1-1];
}

int main() {
    int i, j, K, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> m >> n >> D;
        grid.resize(m);
        for ( auto &v: grid )
            is >> v;
        double total_mass= 0.00;
        for ( i= 1; i <= m; ++i )
            for ( j= 1; j <= n; ++j )
                total_mass+= (g[i][j]= (grid[i-1][j-1]-'0')+D);
        for ( i= 1; i <= m; ++i )
            for ( j= 1; j <= n; ++j ) {
                z[0][i][j]= z[0][i-1][j]+z[0][i][j-1]+i*g[i][j]-z[0][i-1][j-1];
                z[1][i][j]= z[1][i-1][j]+z[1][i][j-1]+j*g[i][j]-z[1][i-1][j-1];
                z[2][i][j]= z[2][i-1][j]+z[2][i][j-1]+g[i][j]-z[2][i-1][j-1];
            }
        // std::cerr << "==================================" << std::endl;
        for ( K= std::min(m,n); K >= 3; --K ) {
            for ( i= 1; i <= m; ++i )
                for ( j= 1; j <= n; ++j ) {
                    auto ni= i-K+1, nj= j-K+1;
                    if ( ni >= 1 and nj >= 1 ) {
                        double cx= sum(0,ni,nj,i,j)-i*g[i][j]-i*g[i][nj]-ni*g[ni][j]-ni*g[ni][nj];
                        double cy= sum(1,ni,nj,i,j)-j*g[i][j]-nj*g[i][nj]-j*g[ni][j]-nj*g[ni][nj];
                        double unit_mass= sum(2,ni,nj,i,j)-g[i][j]-g[ni][j]-g[i][nj]-g[ni][nj];
                        unit_mass/= (K*K-4), cx/= (K*K-4), cy/= (K*K-4);
                        double ex= unit_mass*(i+ni)/2.00, ey= unit_mass*(j+nj)/2.00;
                        /*
                        std::cerr << "(" << ni << "," << nj << "), (" << i << ", " << j << ")" << std::endl;
                        std::cerr << std::setprecision(3) << std::fixed << ex << ", " << ey << " vs " << cx << ", " << cy << std::endl;
                         */
                        if ( fabs(cx-ex) < tol and fabs(cy-ey) < tol )
                            goto nx;
                    }
                }
        }
        nx:
        os << "Case #" << ++cs << ": " << (K >= 3 ? std::to_string(K) : "IMPOSSIBLE") << std::endl;
    }
    return 0;
}   
 