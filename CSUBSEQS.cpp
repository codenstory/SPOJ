/**
 * CSUBSEQS
 * TOPIC: DP, greedy, tricky
 * status: Accepted
 */
#include <bits/stdc++.h>
#define M 4
#define A 26
#define N 51
#define oo (1ll<<60)

std::vector<std::string> s(M);
std::vector<size_t> n(M);
std::vector<std::vector<int>> positions[A];

using i64= std::int64_t;
i64 z[N][N][N][N];

int last_positionsition( const std::vector<int> &p, int k ) {
    if ( p.empty() )
        return k+1;
    auto it= std::lower_bound(p.begin(),p.end(),k);
    if ( it == p.end() )
        return *(it-1);
    if ( *it == k )
        return k;
    if ( it == p.begin() )
        return k+1;
    return *(it-1);
}

i64 calc_z( int i, int j, int k, int t ) {
    if ( i < 0 or j < 0 or k < 0 or t < 0 )
        return 0;
    if ( z[i][j][k][t] < +oo )
        return z[i][j][k][t];
    z[i][j][k][t]= 0;
    for ( auto ch= 0; ch < A; ++ch ) {
        int ii= last_positionsition(positions[ch][0],i);
        int jj= last_positionsition(positions[ch][1],j);
        int kk= last_positionsition(positions[ch][2],k);
        int tt= last_positionsition(positions[ch][3],t);
        if ( ii == i+1 or jj == j+1 or kk == k+1 or tt == t+1 )
            continue ;
        ++z[i][j][k][t];
        z[i][j][k][t]+= calc_z(ii - 1, jj - 1, kk - 1, tt - 1);
    }
    return z[i][j][k][t];
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
    int i,j,k,t,len;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &out = std::cout;
    for ( auto &t: s )
        std::getline(is,t);
    for ( i= 0; i < M; ++i ) {
        n[i] = s[i].size();
        assert( n[i] >= 1 );
    }
    for ( auto ch= 0; ch < A; positions[ch++].resize(M,std::vector<int>{}) ) ;
    for ( i= 0; i < n[0]; ++i )
        for ( j= 0; j < n[1]; ++j )
            for ( k= 0; k < n[2]; ++k )
                for ( t= 0; t < n[3]; ++t )
                    z[i][j][k][t]= +oo;
    for ( i= 0; i < M; ++i )
        for ( j= 0; j < n[i]; ++j )
            positions[s[i][j]-'a'][i].push_back(j);
    i64 ans= calc_z(n[0]-1,n[1]-1,n[2]-1,n[3]-1);
    out << ans << '\n';
    return 0;
}