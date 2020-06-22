/**
 * GEM
 * TOPIC: simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
#define BIT(k)  (1ull<<(k))
#define MASK(k) (BIT(k)-1ull)
using u64   = std::uint64_t;
using state = u64;
using slice = u64;
using mask  = u64;
#define SH (3)
#define A  (10)
#define N  (BIT(SH))
#define E(i,j) (((i)<<SH)+(j))

const slice EMPTY= 0ull;

slice VMASK( int k ) {
    static std::unordered_map<int,slice> mp= {};
    slice res= EMPTY;
    if ( mp.count(k) )
        return mp[k];
    for ( int i= 0, j= 0; i < k; ++i )
        res|= BIT(E(i,j));
    return mp[k]= res;
}

namespace cbd_helper {
    std::vector<mask> rows, cols;
    mask get_row( int r ) {
        if ( rows.empty() )
            for ( int i= 0; i < N; ++i )
                rows.push_back(MASK(N)<<(i*N));
        return rows[r];
    }
    mask get_col( int c ) {
        if ( cols.empty() )
            for ( int j= 0; j < N; ++j ) {
                cols.push_back(EMPTY);
                for ( int i= 0; i < N; ++i )
                    cols[j]|= BIT(E(i,j));
            }
        return cols[c];
    }

    slice reduce( slice s ) {
        slice res= s;
        for ( int i= 0,j,t; i < N; ++i )
            if ( true or __builtin_popcount(s & get_row(i)) >= 3 ) {
                for ( j= 0; j < N; j= t ) {
                    for ( ;j < N and not (s&BIT(E(i,j))); ++j ) ;
                    for ( t= j; t < N and (s&BIT(E(i,t))); ++t ) ;
                    if ( (t-j) >= 3 ) {
                        res&= ~(MASK(t-j) << E(i,j));
                    }
                }
            }
        for ( int j= 0,i,t; j < N; ++j )
            if ( true or __builtin_popcount(s & get_col(j)) >= 3 ) {
                for ( i= 0; i < N; i= t ) {
                    for ( ;i < N and not (s & BIT(E(i,j))); ++i ) ;
                    for ( t= i; t < N and (s&BIT(E(t,j))); ++t ) ;
                    if ( (t-i) >= 3 ) {
                        res&= ~(VMASK(t-i) << E(i,j));
                    }
                }
            }
        return res;
    }
};

#define LSB(u) ((u) & ((~(u))+1ull))
char which[BIT(20)];

int who( u64 u ) {
    if ( u < BIT(20) )
        return which[u];
    if ( u < BIT(40) )
        return 20+which[u>>20];
    if ( u < BIT(60) )
        return 40+which[u>>40];
    return 60+which[u>>60];
}

using grid= std::vector<std::vector<char>>;

class cboard {

    enum {
        ZERO,
        ONE, TWO, THREE, FOUR,
        FIVE, SIX, SEVEN, EIGHT, NINE
    };

    std::vector<slice> slices;
    grid g;

    std::vector<slice> board2slices( const grid &c ) {
        std::vector<slice> s(A,EMPTY);
        for ( int i= 0; i < N; ++i )
            for ( int j= 0; j < N; ++j )
                if ( c[i][j] != '.' ) {
                    auto idx= c[i][j]-'0';
                    assert( 1 <= idx and idx < A );
                    s[idx]|= BIT(E(i,j));
                } else {
                    s[ZERO]|= BIT(E(i,j));
                }
        return s;
    }

    void normalize( grid &c ) {
        for ( int j= 0,i,t; j < N; ++j ) {
            int zero= 0;
            for ( i= 0; i < N; ++i )
                if ( c[i][j] == '.' and ++zero )
                    ;
            for ( t= N-1, i= N-1; i >= 0; --i )
                if ( c[i][j] != '.' )
                    c[t--][j]= c[i][j];
            for ( i= 0; i < zero; ++i )
                c[i][j]= '.';
            assert( i == t+1 );
        }
    }

    void slices2board( const std::vector<slice> &s, grid &c ) {
        for ( auto &v: c )
            std::fill(v.begin(),v.end(),'.');
        for ( int ch= ONE; ch <= NINE; ++ch ) {
            for ( auto u= s[ch]; u; u&= ~LSB(u) ) {
                auto idx= who(LSB(u));
                int x= (idx>>SH), y= (idx&MASK(SH));
                c[x][y]= (ch+'0');
            }
        }
    }

public:

    cboard( const std::vector<std::vector<char>> &g ) {
        this->g= g, slices= board2slices(g);
        auto z= g;
        slices2board(slices,z);
        assert( z == g );
    }

    bool xchg( int r1, int c1, int r2, int c2 ) {
        if ( r1 == r2 and c1 == c2 ) {
            return false;
        }
        int z1= g[r1][c1]=='.'?0:(g[r1][c1]-'0'),
            z2= g[r2][c2]=='.'?0:(g[r2][c2]-'0');
        if ( z1 == z2 ) {
            return false;
        }
        auto &sls= slices;
        assert( (sls[z1] & BIT(E(r1,c1))) );
        assert( (sls[z2] & BIT(E(r2,c2))) );
        sls[z1]&= ~BIT(E(r1,c1)), sls[z1]|= BIT(E(r2,c2));
        sls[z2]&= ~BIT(E(r2,c2)), sls[z2]|= BIT(E(r1,c1));
        sls[z1]= cbd_helper::reduce(sls[z1]);
        sls[z2]= cbd_helper::reduce(sls[z2]);
        //std::swap(g[r1][c1],g[r2][c2]);
        slices2board(sls,g);
        normalize(g);
        slices= board2slices(g);
        return true ;
    }

    cboard &perform_reductions() {
        bool flag;
        do {
            flag= false ;
            for ( int ch= ONE; ch <= NINE; ++ch ) {
                auto s= cbd_helper::reduce(slices[ch]);
                if ( s != slices[ch] )
                    flag= true ;
                slices[ch]= s;
            }
            if ( flag ) {
                slices2board(slices, g), normalize(g), slices = board2slices(g);
            }
        } while ( flag );
        return *this;
    }

    bool is_empty() const {
        return std::count_if(slices.begin()+ONE,slices.end(),[](auto x) {
            return x != 0;
        }) == 0;
    }
};

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
#endif
    int dx[]= {-1,0,1,0},
        dy[]= {0,1,0,-1};
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int i,j,k,t;
    for ( i= 0; i < 20; which[BIT(i)]= i, ++i ) ;
    for ( std::string s; (is >> s) and s != "0"; ) {
        grid g(N,std::vector<char>(N));
        for ( i= 0, j= 0; j < N; ++j ) {
            g[i][j] = s[j];
            assert( s[j] == '.' or std::isdigit(s[j]) );
        }
        for ( ;++i < N; )
            for ( is >> s, j= 0; j < N; ++j ) {
                g[i][j] = s[j];
                assert( s[j] == '.' or std::isdigit(s[j]) );
            }
        cboard c(g);
        for ( i= 0; i < N; ++i )
            for ( j= 0; j < N; ++j )
                for ( int l= 0; l < 4; ++l ) {
                    k= i+dx[l], t= j+dy[l];
                    if ( 0 <= k and k < N and 0 <= t and t < N ) {
                        auto z = c;
                        if (z.xchg(i, j, k, t)) {
                            if (z.perform_reductions().is_empty())
                                goto GOOD;
                        }
                    }
                }
        os << "No" << '\n';
        continue ;
        GOOD: os << "Yes" << '\n';
    }
    return 0;
}
    