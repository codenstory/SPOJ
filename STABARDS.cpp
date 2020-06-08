/**
 * STABARDS
 * TOPIC: matching
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x80
#define Q (2*N)
#define QS (0x400)
enum {S,T};

std::istream &is = std::cin;
std::ostream &os = std::cout;

int m,n,gender[N],seen[Q],yes,mate[2][Q],q[QS],head,tail,cnt;
char mat[N][N];
std::vector<int> adj[2][Q];

bool alt_path( int x ) {
    if ( seen[x] == yes )
        return false ;
    seen[x]= yes;
    for ( auto y: adj[S][x] )
        if ( mate[T][y] < 0 or alt_path(mate[T][y]) ) {
            mate[S][mate[T][y]= x]= y;
            return true ;
        }
    return false ;
}

int bpm() {
    int x,k;
    for ( head= tail= 0, cnt= 0, x= 0; x < n+n; ++x )
        q[tail++]= x, tail&= (QS-1), ++cnt;
    for ( x= 0; x < n+n; ++x )
        mate[S][x]= mate[T][x]= -1;
    bool flag;
    do {
        for ( flag= false, k= cnt, ++yes; k--; ) {
            x= q[head++], head&= (QS-1), --cnt;
            if ( alt_path(x) )
                flag= true ;
            else {
                q[tail++]= x, tail&= (QS-1), ++cnt;
            }
        }
    } while ( flag );
    return 2*n-cnt;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( int i,j,k,t,ch; (is >> m >> n) and (m or n); os << bpm() << '\n' ) {
        for ( i= 0; i < m; ++i )
            for ( j= 0; j < m; ++j )
                for ( ;std::isspace(mat[i][j]= is.get()); ) ;
        for ( i= 0; i < n; is >> gender[i++] ) ;
        for ( i= 0; i < 2*n; ++i )
            for ( t= S; t <= T; adj[t++][i].clear() ) ;
        for ( i= 0; i < n; ++i ) {
            for ( j= 0; j < n; ++j )
                if ( i != j and (ch= mat[gender[i]][gender[j]]) != 'X' ) {
                    if ( ch == 'C' ) {
                        adj[S][i].push_back(j+n);
                    } else {
                        assert( ch == 'D' );
                        adj[T][i+n].push_back(i);
                    }
                }
        }
    }
    return 0;
}
    