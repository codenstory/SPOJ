/**
 *
 */
#include <bits/stdc++.h>
#define M 21
enum { E= 0, S= 1, W= 2, N= 3 };

std::istream &is = std::cin;
std::ostream &os = std::cout;

class walker {
    int m, n;
    bool seen[M][M];
    int cx,cy,dir;
    int dx[4]= {0,1,0,-1},
            dy[4]= {1,0,-1,0};
#define valid_cell(x,y) (0<=(x) and (x)<m and 0<=(y) and (y)<n)
public:
    constexpr const static std::pair<int,int> NONE= {-1,-1};
    walker( int m, int n ): m(m), n(n) {
        memset(seen,0,sizeof seen);
        cx= cy= 0, dir= E;
        seen[cx][cy]= true ;
    }
    std::pair<int,int> next() {
        auto nx= cx+dx[dir],
                ny= cy+dy[dir];
        if ( valid_cell(nx,ny) and not seen[nx][ny] ) {
            seen[nx][ny]= true ;
            return std::make_pair(cx = nx, cy = ny);
        }
        ++dir, dir&= 3;
        nx= cx+dx[dir], ny= cy+dy[dir];
        if ( not valid_cell(nx,ny) or seen[nx][ny] )
            return NONE;
        seen[nx][ny]= true ;
        return std::make_pair(cx= nx, cy= ny);
    }
    std::pair<int,int> get_current() const {
        return std::make_pair(cx,cy);
    }
#undef valid_cell
};

void solve_case( int cs ) {
    int m,n,i,j,k;
    std::string s;
    is >> m >> n >> s;
    std::vector<std::string> grid(m);
    for ( auto &z: grid )
        z.resize(n);
    for ( k= 0, i= 0; i < m; ++i )
        for ( j= 0; j < n; ++j )
            grid[i][j]= s[k++];
    walker w(m,n);
    std::string res{};
    for ( i= 0; i+5 <= m*n; i+= 5 ) {
        int x= 0;
        for ( j= 0; j < 5; ++j ) {
            auto pr= w.get_current();
            x <<= 1, x+= grid[pr.first][pr.second]-'0';
            w.next();
        }
        res.push_back(x==0?' ':x-1+'A');
    }
    os << cs << ' ' << res << std::endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts,cs= 0;
    for ( is >> ts; ts--; solve_case(++cs) ) ;
    return 0;
}
    