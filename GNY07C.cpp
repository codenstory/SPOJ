/**
 * GNY07C
 * TOPIC: STL, bitset, walking matrix in unusual order
 * status: Accepted
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

std::regex R= std::regex(R"(^(\d+)\s+(\d+)\s{1}(.*)$)");

void solve_case( int cs ) {
    std::string buff;
    std::getline(is,buff);
    std::smatch mtch;
    if ( not std::regex_search(buff,mtch,R) )
        assert( false );
    int m= strtol(mtch.str(1).c_str(),nullptr,10),
        n= strtol(mtch.str(2).c_str(),nullptr,10),
        i,j,k;
    std::string s= mtch.str(3);
    walker w(m,n);
    std::vector<int> c{};
    std::transform(s.begin(),s.end(),std::back_inserter(c),[](char ch) {
        return ch == ' ' ? 0 : (ch-'A'+1);
    });
    std::vector<std::string> reprs{};
    std::transform(c.begin(),c.end(),std::back_inserter(reprs),[](int x) {
        return std::bitset<5>(x).to_string();
    });
    auto text= std::accumulate(reprs.begin(),reprs.end(),std::string{},[](std::string acc, std::string &z) {
        return acc+=z;
    });
    std::vector<std::string> grid(m);
    for ( auto &z: grid )
        z.resize(n,'0');
    for ( auto ch: text ) {
        auto pr= w.get_current();
        grid[pr.first][pr.second]= ch;
        w.next();
    }
    os << cs << ' ';
    for ( auto &z: grid )
        os << z;
    os << std::endl;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts,cs= 0;
    std::string buff;
    for ( std::getline(is,buff), ts= strtol(buff.c_str(),nullptr,10); ts--; ) {
        solve_case(++cs);
    }
    return 0;
}
    