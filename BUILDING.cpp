/**
 * BUILDING
 * TOPIC: range trees, lazy propagation, DP, stack
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

class rtree {

private:

    std::vector<i64> tr,h;
    int n,m;

#define L(v) ((v)<<1)
#define R(v) (1|L(v))

    void push_down( int v, int i, int j ) {
        if ( i < j and h[v] )
            h[L(v)]+= h[v], h[R(v)]+= h[v];
        tr[v]+= h[v], h[v]= 0;
    }

    void push_up( int v, int i, int j ) {
        if ( i < j )
            tr[v]= tr[L(v)]+tr[R(v)];
    }

    void update( int v, int i, int j, int qi, int qj, i64 val ) {
        push_down(v,i,j);
        if ( qj < i or qi > j )
            return ;
        if ( qi <= i and j <= qj ) {
            h[v]+= val, push_down(v,i,j);
            return ;
        }
        auto k= (i+j)>>1u;
        update(L(v),i,k,qi,qj,val), update(R(v),k+1,j,qi,qj,val);
        push_up(v,i,j);
    }

    i64 access( int v, int i, int j, int pos ) {
        push_down(v,i,j);
        assert( i <= pos and pos <= j );
        if ( i == j )
            return tr[v];
        auto k= (i+j)>>1;
        i64 res= 0;
        if ( pos <= k )
            res= access(L(v),i,k,pos);
        else res= access(R(v),k+1,j,pos);
        push_up(v,i,j);
        return res;
    }

public:

    rtree( int n ) {
        this->n= n, m= 3*n+7;
        tr.resize(m,0), h.resize(m,0);
    }

    void inc( int qi, int qj, i64 val ) {
        update(1,0,n-1,qi,qj,val);
    }

    i64 access( int pos ) {
        return access(1,0,n-1,pos);
    }

#undef L
#undef R

};

bool solve_case( std::istream &is, std::ostream &os ) {
    int n,i,j,k,t;
    std::vector<int> left, right;
    std::vector<i64> h;
    if ( not (is >> n) )
        return false ;
    h.resize(n);
    for ( auto &val: h )
        is >> val;
    left.resize(n), right.resize(n);
    std::stack<int> st{};
    for ( i= 0; i < n; left[i]= st.empty()?-1:st.top(), st.push(i++) )
        for (;not st.empty() and h[st.top()] > h[i]; st.pop() ) ;
    for ( ;not st.empty(); st.pop() ) ;
    for ( i= n-1; i >= 0; right[i]= st.empty()?n:st.top(), st.push(i--) )
        for (;not st.empty() and h[st.top()] >= h[i]; st.pop() ) ;

    auto A= std::make_unique<rtree>(n),
         B= std::make_unique<rtree>(n);
    for ( t= 0; t < n; ++t ) {
        i= left[t], j= right[t];
        assert( i < t and t < j );
        A->inc(i+1,t,(j-t)*(h[t]));
        B->inc(i+1,t,-i*(h[t])*(j-t));
        A->inc(t+1,j-1,-(h[t])*(t-i));
        B->inc(t+1,j-1,j*(h[t])*(t-i));
    }

    for ( i= 0; i < n; ++i ) {
        auto res = i * A->access(i) + B->access(i);
        if ( i ) os << ' ';
        os << res;
    }
    os << '\n';

    return true ;

}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    while ( solve_case(is,os) ) ;
    return 0;
}