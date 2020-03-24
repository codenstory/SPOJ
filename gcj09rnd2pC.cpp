/**
 * TOPIC: Dilworth's Theorem
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x80
enum { S, T };

int n,K;

struct chart {
    std::vector<int> segments;
    int operator[]( int i ) const {
        return segments[i];
    }
};

std::istream &operator >> ( std::istream &is, chart &c ) {
    c.segments.resize(K);
    for ( auto &p: c.segments )
        is >> p;
    return is;
}

bool operator < ( const chart &a, const chart &b ) {
    return std::inner_product(a.segments.begin(),a.segments.end(),b.segments.begin(),
            true,[]( bool flag, bool x ) {
        return flag&&x;
    }, []( int x, int y ) {
        return x < y;
    });
}

int visited[N],yes,mate[2][N];
std::vector<int> adj[N];
std::vector<chart> charts;

bool alt_path( int x ) {
    if ( visited[x] == yes )
        return false ;
    visited[x]= yes;
    for ( auto y: adj[x] )
        if ( mate[T][y] < 0 or alt_path(mate[T][y]) ) {
            mate[S][mate[T][y]= x]= y;
            return true ;
        }
    return false ;
}

void dfs( int x ) {
    if ( visited[x] == yes )
        return ;
    visited[x]= yes;
    if ( mate[S][x] < 0 )
        return ;
    dfs(mate[S][x]);
}

int bpm() {
    int x,k,q[N],head= 0,tail= 0,cnt= 0;
    for ( x= 0; x < n; ++x )
        mate[S][x]= mate[T][x]= -1;
    for ( x= 0; x < n; q[tail++]= x++, ++cnt ) ;
    bool flag;
    do {
        for ( flag= false, k= cnt, ++yes; k--; ) {
            x= q[head++], head&= (N-1), --cnt;
            assert( mate[S][x] < 0 ) ;
            if ( alt_path(x) )
                flag= true;
            else
                q[tail++]= x, ++cnt, tail&= (N-1);
        }
    } while ( flag );
    std::cerr << "max matching is " << n-cnt << std::endl;
    for ( x= 0; x < n; ++x )
        if ( mate[S][x] >= 0 )
            std::cerr << x+1 << " and " << mate[S][x]+1 << std::endl;
    return cnt;
}

int main() {
    int i, j, k, cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> K, charts.resize(n);
        for ( i= 0; i < n; adj[i++].clear() ) ;
        for ( auto &c: charts )
            is >> c;
        for ( i= 0; i < n; ++i )
            for ( j= 0; j < n; ++j )
                if ( charts[i] < charts[j] ) {
                    std::cerr << "Drawing arc: " << i+1 << "->" << j+1 << std::endl;
                    adj[i].push_back(j);
                }
        os << "Case #" << ++cs << ": " << bpm() << std::endl;
    }
    return 0;
}   
 