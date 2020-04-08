/**
 * TOPIC: ad hoc, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;
#define N 4

using board= std::vector<std::string>;
board g(N),e(N);

auto has_won= [](const board &b, char ch)-> bool {
    for ( const auto &v: b )
        if ( std::count(v.begin(),v.end(),ch)+std::count(v.begin(),v.end(),'T') == N )
            return true ;
    std::function<int(int,int)> fnc[]= {[](int k,int i){ return k+i;}, [](int k, int i){ return k-i;}};
    auto main_diag= [&fnc](auto i) {
        return fnc[0](0,i);
    };
    auto aux_diag= [&fnc](auto i) {
        return fnc[1](N-1,i);
    };
    int acc= 0;
    for ( int i= 0; i < N; ++i ) {
        auto j= main_diag(i);
        assert( 0 <= j and j < N );
        if ( b[i][j] == ch or b[i][j] == 'T' )
            ++acc;
    }
    if ( acc == N ) return true ;
    acc= 0;
    for ( int i= 0; i < N; ++i ) {
        auto j= aux_diag(i);
        assert( 0 <= j and j < N );
        if ( b[i][j] == ch or b[i][j] == 'T' )
            ++acc;
    }
    return acc == N;
};

void solve( int cs ) {
    os << "Case #" << cs << ": ";
    int i,j,k;
    for ( i= 0; i < N; is >> g[i++] ) ;
    for ( i= 0; i < N; ++i )
        e[i].resize(N);
    for ( i= 0; i < N; ++i ) {
        for (j = 0; j < N; ++j)
            e[i][j] = g[j][i];
    }
    for ( auto ch: {'X','O'} )
        if ( has_won(g,ch) or has_won(e,ch) ) {
            os << ch << " won" << std::endl;
            return ;
        }
    if ( not std::accumulate(g.begin(),g.end(),0,[](int acc, const auto &v) {
        return acc+std::count(v.begin(),v.end(),'.');
    }) ) {
        os << "Draw" << std::endl;
        return ;
    }
    os << "Game has not completed" << std::endl;
}

int main() {
    int cs= 0, ts;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 