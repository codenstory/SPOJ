/**
 * KPMATRIX
 * TOPIC: range trees, sweep line, rank space, counting queries
 * status: Accepted
 */
#include <bits/stdc++.h>
//using i64= std::int64_t;
using i64= std::int32_t;
template<typename T>
using vc= std::vector<T>;
using K= i64;
using V= int;

i64 getnum() {
    static int dig[256]= {0};
    i64 n= 0, sign= 1, ch;
    if ( not dig['0'] )
        for ( ch= '0'; ch <= '9'; dig[ch++]= 1 ) ;
    for ( ;(ch= getchar_unlocked()) != EOF and not(ch == '-' or ch == '+' or dig[ch]); )     ;
    if ( ch == '-' or ch == '+' ) {
        if ( ch == '-' )
            sign= -1;
        ch= getchar_unlocked();
    }
    for ( n= ch-'0'; (ch= getchar_unlocked()) != EOF and dig[ch]; n= 10*n+ch-'0' ) ;
    return sign*n;
}

class rank_space {
private:
    vc<K> data;
public:
    rank_space() {data.clear(); }
    explicit rank_space( const vc<K> &input ) {
        data= input, std::sort(data.begin(),data.end());
        data.erase(std::unique(data.begin(),data.end()),data.end());
    }
    inline V rank( K key ) const {
        return std::lower_bound(data.begin(),data.end(),key)-data.begin();
    }
    size_t universe_size() const {
        return data.size();
    }
};

#define LSB(u) ((u) & ((~(u))+1ul))

class rt {
private:
    vc<V> tr;
    rank_space rs;
    size_t n{},m{};
#define L(v) ((v)<<1u)
#define R(v) (1u|L(v))
    V increment( int v, int i, int j, V key ) {
        if ( key < i or key > j )
            return tr[v];
        if ( i == j )
            return ++tr[v];
        auto k= (i+j)>>1u;
        return tr[v]= increment(L(v),i,k,key)+increment(R(v),k+1,j,key);
    }
    V count( int v, int i, int j, V qi, V qj ) const {
        if ( qi > j or qj-1 < i or not tr[v] )
            return 0;
        if ( qi <= i and j <= qj-1 )
            return tr[v];
        auto k= (i+j)>>1u;
        return count(L(v),i,k,qi,qj)+count(R(v),k+1,j,qi,qj);
    }
    size_t NN= 256;
    V prefix( unsigned int u ) const {
        V ans= 0;
        for ( ;u; ans+= tr[u], u&= ~LSB(u) ) ;
        return ans;
    }
#undef L
#undef R
public:
    explicit rt( const vc<K> &data ) {
        rs= rank_space(data);
        this->n= rs.universe_size();
        for ( NN= 1; NN <= n; NN <<= 1 ) ;
        if ( NN >= 256 ) NN= 256;
        tr.resize(NN,0);
        //, this->m= 4*n+7;
        //tr.resize(m,0ll);
    }
    inline void inc( K a ) {
        //increment(1,0,n-1,rs->rank(a));
        auto k= rs.rank(a)+1;
        for ( unsigned int u= k; u < NN; ++tr[u], u+= LSB(u) ) ;
    }
    inline V count( K a, K b ) const {
        auto qi= rs.rank(a)+1, qj= rs.rank(b)+1;
        //return count(1,0,n-1,(V)qi,(V)qj);
        return prefix(qj-1)-prefix(qi-1);
    }
};

i64 solve( const vc<i64> &data, i64 A, i64 B ) {
    vc<i64> p{};
    p.reserve(data.size());
    std::partial_sum(data.begin(),data.end(),std::back_inserter(p));
    auto T= rt(p);
    i64 ans= 0;
    for ( auto it= p.rbegin(); it != p.rend(); ++it )
        ans+= T.count(A+*it,B+*it+1), T.inc(*it);
    return ans+T.count(A,B+1);
}

vc<vc<i64>> read_input( std::istream &is, int &m, int &n ) {
    if ( m < n ) {
        vc<vc<i64>> g(n,vc<i64>(m));
        for ( int j= 0; j < m; ++j )
            for ( int i= 0; i < n; ++i )
                //is >> g[i][j];
                g[i][j]= getnum();
        std::swap(m,n);
        return g;
    } else {
        vc<vc<i64>> g(m,vc<i64>(n));
        for ( auto &v: g )
            for ( auto &z: v )
                z= getnum();
        return g;
    };
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( int m,n,i,j,k;; ) {
        m= getnum(), n= getnum();
        auto g= read_input(is,m,n);
        //assert( m >= n );
        i64 A,B,ans= 0;
        A= getnum(), B= getnum();
        vc<vc<i64>> prefix(m,vc<i64>{});
        for ( i= 0; i < m; ++i ) {
            prefix[i].reserve(n);
            std::partial_sum(g[i].begin(), g[i].end(), std::back_inserter(prefix[i]));
        }
        vc<i64> c(m);
        for ( j= 0; j < n; ++j )
            for ( k= j; k < n; ++k ) {
                for ( i= 0; i < m; ++i )
                    c[i]= prefix[i][k]-(j==0?0:prefix[i][j-1]);
                ans+= solve(c,A,B);
            }
        os << ans << '\n';
        break ;
    }
    return 0;
}
    