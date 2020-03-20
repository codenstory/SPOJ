/**
 * TOPIC: Hamiltonian cycles, clever reduction, DP
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 16
#define oo (0xfffffffful)

int K,n,m;
std::vector<std::vector<char>> g;
std::vector<std::vector<int>> w;
unsigned int z[1<<N][N];

template<typename T>
void init( std::vector<std::vector<T>> &e, int row, int col ) {
    e.resize(row);
    for ( auto &v: e )
        v.resize(col);
}

int distance( int first, int last ) {
    int weight= 0;
    for ( int i= 0; i < m-1; ++i )
        weight+= (g[i+1][first] == g[i][last])?0:1;
    return weight;
}

int main() {
    int i,j,k,l,t,cs= 0,ts;
    unsigned int u,v,best;
    std::istream &is= std::cin;
    std::ostream &os= std::cout;
    for ( is >> ts; ts--; ) {
        std::string s;
        is >> K >> s;
        n= s.size();
        assert( 0 == (n%K) );
        m= n/K, init(g,m,K), init(w,K,K);
        for ( k= 0, i= 0; i < m; ++i )
            for ( j= 0; j < K; g[i][j++]= s[k++] ) ;
        assert( k == n );
        for ( best= +oo, t= 0; t < K; ++t ) {
            for ( i= 0; i < K; ++i )
                for ( j= 0; j < K; ++j ) {
                    if ( i == j ) {
                        w[i][j]= 0;
                        continue ;
                    }
                    if (i == t)
                        w[i][j] = distance(j, i);
                    else
                        for (w[i][j] = 0, l = 0; l < m; ++l)
                            w[i][j] += (g[l][i] == g[l][j]) ? 0 : 1;
                }
            memset(z,0xff,sizeof z);
            for ( u= 0; u < (1<<K); ++u )
                for ( i= 0; i < K; ++i )
                    if ( (1<<i) == u )
                        z[u][i]= w[i][t];
                    else if ( u & (1<<i) ) {
                        for ( v= (u&~(1ul<<i)), j= 0; j < K; ++j )
                            if ( v & (1<<j) )
                                if ( z[v][j] < +oo )
                                    if ( z[u][i] > w[i][j]+z[v][j] )
                                        z[u][i]= w[i][j]+z[v][j];
                    }
            best= std::min(best,z[(1<<K)-1][t]);
        }
        os << "Case #" << ++cs << ": " << ++best << std::endl;
    }
    return 0;
}

