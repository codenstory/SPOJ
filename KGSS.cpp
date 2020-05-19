/**
 * KGSS
 * TOPIC: range trees, divide and conquer for top-2
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define oo (1ll<<49)

std::istream &is = std::cin;
std::ostream &os = std::cout;

class rt {
private:
    std::vector<i64> tr[2];
    int m,n;
    const std::vector<i64> &data;
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
    void build( int v, int i, int j ) {
        assert( i <= j );
        if ( i == j ) {
            tr[0][v]= data[i], tr[1][v]= -oo;
            return ;
        }
        auto k= (i+j)>>1;
        build(L(v),i,k), build(R(v),k+1,j);
        tr[0][v]= std::max(tr[0][L(v)],tr[0][R(v)]);
        if ( tr[1][L(v)] >= tr[0][R(v)] ) {
            tr[1][v]= tr[1][L(v)];
        } else if ( tr[1][R(v)] >= tr[0][L(v)] ) {
            tr[1][v]= tr[1][R(v)];
        } else {
            tr[1][v] = std::min(tr[0][L(v)], tr[0][R(v)]);
        }
    }
    std::pair<i64,i64> query( int v, int i, int j, int qi, int qj ) const {
        if ( qi > j or qj < i )
            return std::make_pair(-oo,-oo);
        if ( qi <= i and j <= qj )
            return std::make_pair(tr[0][v],tr[1][v]);
        auto k= (i+j)>>1;
        auto lft= query(L(v),i,k,qi,qj),
             rgt= query(R(v),k+1,j,qi,qj);
        auto res= std::make_pair(-oo,-oo);
        res.first= std::max(lft.first,rgt.first);
        if ( lft.second >= rgt.first )
            res.second= lft.second;
        else if ( rgt.second >= lft.first )
            res.second= rgt.second;
        else res.second= std::min(lft.first,rgt.first);
        //assert( res.first != -oo and res.second != -oo );
        return res;
    }
    void update( int v, int i, int j, int pos, i64 newval ) {
        if ( pos > j or pos < i )
            return ;
        if ( i == j ) {
            assert( pos == i );
            tr[0][v]= newval, tr[1][v]= -oo;
            return ;
        }
        auto k= (i+j)>>1;
        update(L(v),i,k,pos,newval), update(R(v),k+1,j,pos,newval);
        tr[0][v]= std::max(tr[0][L(v)],tr[0][R(v)]);
        if ( tr[1][L(v)] >= tr[0][R(v)] ) {
            tr[1][v]= tr[1][L(v)];
        } else if ( tr[1][R(v)] >= tr[0][L(v)] ) {
            tr[1][v]= tr[1][R(v)];
        } else {
            tr[1][v] = std::min(tr[0][L(v)], tr[0][R(v)]);
        }
    }
public:
    rt( const std::vector<i64> &data ): data(data) {
        n= data.size(), m= 4*n+7;
        for ( auto t= 0; t <= 1; tr[t++].resize(m) ) ;
        build(1,0,n-1);
    }
    void update( int pos, i64 newval ) {
        update(1,0,n-1,pos-1,newval);
    }
    i64 query( int qi, int qj ) const {
        auto pr= query(1,0,n-1,qi-1,qj-1);
        return pr.first+pr.second;
    }
};

std::vector<i64> data;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( int n, qr,i,j; (is >> n) and n; ) {
        data.resize(n);
        for ( auto &v: data )
            is >> v;
        auto T= std::make_unique<rt>(data);
        for ( is >> qr; qr--; ) {
            std::string comm;
            is >> comm >> i >> j;
            if ( comm.front() == 'Q' ) {
                os << T->query(i,j) << '\n';
            } else {
                T->update(i,j);
            }
        }
    }
    return 0;
}
