/**
 * HS08FOUR
 * TOPIC: matrix exponentiation, DP
 * status: TLE
 */
#include <bits/stdc++.h>
#define A 4
#define MOD 1000000007ll
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::vector<unsigned> vertices;
std::unordered_set<unsigned> seen;

using t_matrix= std::vector<std::vector<i64>>;

t_matrix mult( const t_matrix &x, const t_matrix &y ) {
    t_matrix m= x, tmp= x;
    int i,j,k,N= x.size();
    for ( i= 0; i < N; ++i )
        for ( j= 0; j < N; ++j )
            for ( tmp[i][j]= 0, k= 0; k < N; ++k ) {
                auto t= x[i][k]*y[k][j];
                if ( t >= MOD )
                    t%= MOD;
                for ( tmp[i][j]+= t; tmp[i][j] >= MOD; tmp[i][j] -= MOD ) ;
            }
    for ( i= 0; i < N; ++i )
        for ( j= 0; j < N; ++j )
            m[i][j]= tmp[i][j];
    return m;
}

t_matrix identity_matrix( int n ) {
    t_matrix m;
    m.resize(n,std::vector<i64>(n,0));
    for ( int i= 0; i < n; ++i )
        for ( int j= 0; j < n; ++j )
            if ( i == j )
                m[i][j]= 1;
            else m[i][j]= 0;
    return m;
}

void display( const std::vector<std::vector<i64>> &m ) {
    int i,j,k;
    for ( i= 0; i < m.size(); ++i ) {
        for ( j= 0; j < m[i].size(); ++j ) {
            os << std::setw(9) << m[i][j];
        }
        os << std::endl;
    }
}

std::vector<t_matrix> C;
std::vector<t_matrix> lower, middle, upper;
#define BIT(k) (1ull<<(k))
#define MASK(k) (BIT(k)-1ull)

t_matrix matpower( const t_matrix &m, i64 n ) {
    auto x= m, tmp= x;
    int i,j,k,N= m.size();
    unsigned int u= n;
    auto ax= lower[u & MASK(10)];
    if ( u >= (1<<10) )
        ax= mult(ax,middle[(u>>10)&MASK(10)]);
    u >>= 10;
    if ( u >= (1<<10) )
        ax= mult(ax,upper[u&MASK(10)]);
    return ax;
}

i64 f( const std::vector<std::vector<i64>> &m, i64 n ) {
    i64 ans= 0;
    auto ax= matpower(m,n);
    for ( auto i= 0; i < ax.size(); ++i )
        for ( auto j= 0; j < ax[i].size(); ++j )
            if ( (ans+= ax[i][j]) >= MOD )
                ans-= MOD;
    return ans;
}

unsigned char which[1<<11];
#define L(k) ((k) & ((~(k))+1ul))

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,t,ts;
    for ( i= 0; i < 11; ++i )
        which[1<<i]= i;
    for ( i= 0; i < A; ++i )
        for ( j= 0; j < A; ++j )
            for ( k= 0; k < A; ++k )
                if ( (i != j and j != k and k != i) or (i == 0 or j == 0 or k == 0) ) {
                    unsigned u= (i|(j<<2)|(k<<4));
                    if ( not seen.count(u) )
                        vertices.emplace_back(u), seen.insert(u);
                }

    std::vector<std::vector<i64>> matrix(vertices.size(),std::vector<i64>(vertices.size(),0));

    for ( i= 0 ;i < vertices.size(); ++i )
        for ( j= 0; j < matrix[i].size(); ++j )
            if ( (vertices[i]&0xf) == (vertices[j]>>2) )
                matrix[i][j]= 1;
    for ( C.resize(64), C[0]= matrix, i= 1; i < 64; ++i )
        C[i]= mult(C[i-1],C[i-1]);

    lower.resize(1<<10), lower[0]= identity_matrix(vertices.size());
    for ( unsigned int u= 1; u < (1<<10); ++u ) {
        auto v= u & ~L(u);
        lower[u]= mult(lower[v],C[which[L(u)]]);
    }

    middle.resize(1<<10), middle[0]= C[10];
    for ( unsigned int u= 1; u < (1<<10); ++u ) {
        auto v= u & ~L(u);
        middle[u]= mult(middle[v],C[10+which[L(u)]]);
    }

    upper.resize(1<<10), upper[0]= C[20];
    for ( unsigned int u= 1; u < (1<<10); ++u ) {
        auto v= u & ~L(u);
        upper[u]= mult(upper[v],C[20+which[L(u)]]);
    }

    i64 n;
    for ( is >> ts; ts--; ) {
        is >> n;
        if ( n == 0 ) {
            os << 1 << std::endl;
            continue ;
        }
        if ( n == 1 ) {
            os << A << std::endl;
            continue ;
        }
        if ( n == 2 ) {
            os << A*A << std::endl;
            continue ;
        }
        assert( n >= 3 );
        os << f(matrix,n-3) << std::endl;
    }
    return 0;
}
    