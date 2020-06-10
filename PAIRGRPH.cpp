/**
 * PAIRGRPH
 * TOPIC: brute force
 * status: Accepted
 */
#include <bits/stdc++.h>

#define N 8
#define BIT(k) (1ull<<(k))
#define MASK(k) (BIT(k)-1ull)
#define TST(u,k) ((u) & BIT(k))
#define CLR(u,k) ((u)&= ~BIT(k))
#define SET(u,k) ((u)|=BIT(k))
#define LSB(u) ((u) & ((~(u))+1ull))
#define oo (1ll<<31)

enum { S, T };

using state= std::uint8_t;
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n,m[2];
unsigned char who[BIT(N)],bts[BIT(N)];
i64 best_cost,ins[2],del[2];
std::uint8_t adj[2][N];

i64 find_cost( const std::vector<int> &perm ) {
    i64 cost= 0;
    for ( int i= 0; i < n; ++i )
        for ( int j= i+1; j < n; ++j ) {
            bool present_a= TST(adj[S][i],j),
                 present_b= TST(adj[T][perm[i]],perm[j]);
            if ( present_a == present_b )
                continue ;
            if ( not present_a ) {
                assert( present_b );
                cost+= std::min(ins[S],del[T]);
            } else {
                cost+= std::min(ins[T],del[S]);
            }
            if ( cost >= best_cost )
                return best_cost;
        }
    return cost;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,cs= 0;
    for ( i= 0; i < N; who[BIT(i)]= i, ++i ) ;
    for ( bts[0]= 0, k= 1; k < BIT(N); ++k )
        bts[k]= bts[k>>1]+(k&1);
    for ( ;(is >> n >> m[S] >> m[T]) and (n or m[S] or m[T]); ) {
        memset(adj,0,sizeof adj);
        is >> ins[S] >> ins[T] >> del[S] >> del[T];
        for ( int t= S; t <= T; ++t ) {
            for ( k= 0; k < m[t]; ++k ) {
                is >> i >> j;
                assert( i != j and 0 <= i and i < n and 0 <= j and j < n );
                SET(adj[t][i],j), SET(adj[t][j],i);
            }
        }
        best_cost= +oo;
        std::vector<int> idx(n,0);
        std::iota(idx.begin(),idx.end(),0);
        do {
            best_cost= find_cost(idx);
        } while ( std::next_permutation(idx.begin(),idx.end()) );
        assert( best_cost < +oo );
        os << "Case #" << ++cs << ": " << best_cost << '\n';
    }
    return 0;
}
    