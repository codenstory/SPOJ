/**
 * TOPIC: latin squares
 * status:
 * NOTES: most probably works for small input only (n <= 5); some other approach needed for n <= 50
 */
#include <bits/stdc++.h>
#define BIT(k) (1ull<<(k))
#define MASK(k) (BIT(k)-1ul)
using u64= std::uint64_t;
#define N 0x40

std::istream &is = std::cin;
std::ostream &os = std::cout;
int n,g[N][N],K;
char b[1<<20];
u64 col[N],row[N];
#define L(k) ((k) & (~(k)+1))

int which( u64 u ) {
    if ( u >= BIT(40) )
        return b[u>>40]+40;
    if ( u >= BIT(20) )
        return b[u>>20]+20;
    return b[u];
}

bool f( int x, int y, int diagsum ) {
    if ( x >= n )
        return diagsum == K;
    auto available= (~row[x] & ~col[y]) & MASK(n);
    auto nx= x, ny= y;
    if ( ++ny == n )
        ++nx, ny= 0;
    while ( available ) {
        auto i= which(L(available));
        auto t= BIT(i);
        row[x]|= t, col[y]|= t;
        g[x][y]= (i+1);
        if ( x == y ) diagsum+= (i+1);
        if ( f(nx,ny,diagsum) )
            return true ;
        if ( x == y ) diagsum-= (i+1);
        row[x]&= ~t, col[y]&= ~t, available&= ~t;
    }
    return false ;
}

void solve( int cs ) {
    is >> n >> K, memset(col,0,sizeof col), memset(row,0,sizeof row);
    int i,j,k;
    os << "Case #" << cs << ": ";
    if ( not f(0,0,0) )
        os << "IMPOSSIBLE\n";
    else {
        os << "POSSIBLE\n";
        for ( i= 0; i < n; ++i, os << '\n' ) {
            for ( j= 0; j < n; ++j ) {
                if ( j > 0 ) os << ' ';
                os << g[i][j];
            }
        }
    }
}

int main() {
    int cs = 0, ts;
    for ( ts= 0; ts < 20; b[BIT(ts)]= ts, ++ts ) ;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 