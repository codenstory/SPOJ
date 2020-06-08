/**
 * MSTRING
 * TOPIC: dp
 * status: Accepted
 */
#include <bits/stdc++.h>
#define A 26
#define N 0x400
#define oo (1<<29)

std::istream &is = std::cin;
std::ostream &os = std::cout;

int z[N][N];
bool seen[N][N];
std::vector<int> pos[A];
std::string s,t;
int m,n;
std::pair<int,int> nxt[N][N];
int way[N][N];

int calc_z( int i, int j ) {
    if ( seen[i][j] )
        return z[i][j];
    seen[i][j]= true, z[i][j]= +oo;
    if ( i == m )
        return z[i][j]= +oo;
    if ( j == n )
        return z[i][j]= 1;
    auto ch= s[i]-'a';
    auto jt= std::lower_bound(pos[ch].begin(),pos[ch].end(),j);
    if ( jt == pos[ch].end() )
        return z[i][j]= 1;
    if ( calc_z(i+1,*jt+1) < +oo )
        return z[i][j]= std::min(calc_z(i+1,j),1+calc_z(i+1,*jt+1));
    return z[i][j]= calc_z(i+1,j);
}

void dump( int i, int k ) {
    if ( k == 0 or i == m ) {
        return ;
    }
    if ( way[i][k] >= 0 ) {
        assert( way[i][k] < n and way[i][k] >= 0 );
        putchar(t[way[i][k]]);
    }
    dump(nxt[i][k].first,nxt[i][k].second);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,ch,ach;
    for (;(is>>s>>t);) {
        m= s.size(), n= t.size();
        for ( ch= 0; ch < A; pos[ch++].clear() ) ;
        for ( j= 0; j < n; ++j ) {
            ch= t[j]-'a';
            assert( 0 <= ch and ch < A );
            pos[ch].push_back(j);
        }
        memset(seen,0,sizeof seen);
        os << calc_z(0,0) << std::endl;
    }
    return 0;
}
    