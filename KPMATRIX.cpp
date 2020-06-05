/**
 * KPMATRIX
 * TOPIC: range trees, sweep line, rank space, counting queries
 * status:
 */
#include <bits/stdc++.h>
//using i64= std::int64_t;
using i64= std::int32_t;
template<typename T>
using vc= std::vector<T>;

template<typename T>
class rank_space {
private:
    vc<T> data;
    std::unordered_map<T,size_t> mp;
public:
    explicit rank_space( const vc<T> &input ) {
        data= input, std::sort(data.begin(),data.end());
        data.erase(std::unique(data.begin(),data.end()),data.end());
        mp.clear();
        for ( auto i= 0; i < data.size(); ++i )
            mp[data[i]]= i;
    }
    size_t rank( T key ) const {
        if ( mp.count(key) )
            return mp.find(key)->second;
        return std::lower_bound(data.begin(),data.end(),key)-data.begin();
    }
    size_t universe_size() const {
        return data.size();
    }
};

template<typename T>
class rt {
private:
    vc<size_t> tr;
    std::unique_ptr<rank_space<T>> rs;
    size_t n{},m{};
#define L(v) ((v)<<1u)
#define R(v) (1u|L(v))
    size_t increment( int v, int i, int j, T key ) {
        if ( key < i or key > j )
            return tr[v];
        if ( i == j )
            return ++tr[v];
        auto k= (i+j)>>1u;
        return tr[v]= increment(L(v),i,k,key)+increment(R(v),k+1,j,key);
    }
    size_t count( int v, int i, int j, T qi, T qj ) const {
        if ( qi > j or qj-1 < i or not tr[v] )
            return 0;
        if ( qi <= i and j <= qj-1 )
            return tr[v];
        auto k= (i+j)>>1u;
        return count(L(v),i,k,qi,qj)+count(R(v),k+1,j,qi,qj);
    }
#undef L
#undef R
public:
    explicit rt( const vc<T> &data ) {
        rs= std::make_unique<rank_space<T>>(data);
        this->n= rs->universe_size(), this->m= 4*n+7;
        tr.resize(m,0ll);
    }
    void inc( T a ) {
        increment(1,0,n-1,rs->rank(a));
    }
    size_t count( T a, T b ) const {
        auto qi= rs->rank(a), qj= rs->rank(b);
        return count(1,0,n-1,(T)qi,(T)qj);
    }
};

i64 solve( const vc<i64> &data, i64 A, i64 B ) {
    vc<i64> p{};
    p.reserve(data.size());
    std::partial_sum(data.begin(),data.end(),std::back_inserter(p));
    auto T= std::make_unique<rt<i64>>(p);
    i64 ans= 0;
    for ( auto it= p.rbegin(); it != p.rend(); ++it )
        ans+= T->count(A+*it,B+*it+1), T->inc(*it);
    return ans+T->count(A,B+1);
}

vc<vc<i64>> read_input( std::istream &is, int &m, int &n ) {
    if ( m < n ) {
        vc<vc<i64>> g(n,vc<i64>(m));
        for ( int j= 0; j < m; ++j )
            for ( int i= 0; i < n; ++i )
                is >> g[i][j];
        std::swap(m,n);
        return g;
    } else {
        vc<vc<i64>> g(m,vc<i64>(n));
        for ( auto &v: g )
            for ( auto &z: v )
                is >> z;
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
    for ( int m,n,i,j,k; (is >> m >> n) and (m or n); ) {
        vc<vc<i64>> g= read_input(is,m,n);
        //assert( m >= n );
        i64 A,B,ans= 0;
        is >> A >> B;
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
    }
    return 0;
}
    