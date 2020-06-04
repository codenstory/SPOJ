#include <bits/stdc++.h>
#define CLR 4
#define A 5
#define W (6)
#define M 1000000007ll
using i64= int;
#define S (43)
template<typename T>
using vc= std::vector<T>;
using tmat= vc<vc<i64>>;

std::istream &is = std::cin;
std::ostream &os = std::cout;

vc<unsigned> vx;
std::unordered_set<unsigned> se;

tmat mult(tmat &x, tmat &y) {
    tmat m=x,tmp=x;
    int i,j,k,N=x.size();
    assert( x.size() == y.size() );
    for(i=0;i<N;++i)
        for(j=0;j<N;++j)
            for(tmp[i][j]=0,k=0;k<N;++k) {
                auto t=(int64_t)(x[i][k])*y[k][j];
                if(t>=M)
                    t%=M;
                if((tmp[i][j]+=t)>=M)
                    tmp[i][j]-=M;
            }
    for(i=0;i<N;++i)
        for(j=0;j<N;++j)
            m[i][j]= tmp[i][j];
    return m;
}

tmat im( int n ) {
    tmat m;
    m.resize(n,vc<i64>(n,0));
    for (int i=0; i<n; ++i)
        m[i][i]=1;
    return m;
}

vc<tmat> C,prt[A],D[A];
#define B(k) (1ull<<(k))
#define MK(k) (B(k)-1ull)

tmat mpw(tmat &m,i64 n) {
    auto ax= im(m.size()),x= m;
    for(;n;n>>=1,x=mult(x,x))
        if(n&1)ax=mult(ax,x);
    return ax;
}

unsigned char which[1<<11];
#define L(k) ((k) & ((~(k))+1ul))

tmat &get_p( int t, unsigned u ) {
    if ( prt[t][u].empty() ) {
        assert( u >= 1 );
        auto v=u&~L(u);
        assert( v < u );
        prt[t][u]=mult(get_p(t,v),D[t][which[L(u)]]);
        assert( not prt[t][u].empty() );
    }
    return prt[t][u];
}

int ga(unsigned n){
    auto res= get_p(0,n&MK(W));
    for(int i=1;i<A and(n>>=W);++i)
        res=mult(res,get_p(i,n&MK(W)));
    i64 ans=0;
    for(int i=0;i<S;++i)
        for(int j=0;j<S; ++j)
            if((ans+=res[i][j])>=M)
                ans-=M;
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i, j, k, t, ts, n;
    for (i = 0; i < 11; ++i)
        which[1 << i] = i;
    vx.reserve(S);
    for (i = 0; i < CLR; ++i)
        for (j = 0; j < CLR; ++j)
            for (k = 0; k < CLR; ++k)
                if ((i != j and j != k and k != i) or (i == 0 or j == 0 or k == 0)) {
                    unsigned u = (i | (j << 2) | (k << 4));
                    if (not se.count(u))
                        vx.emplace_back(u), se.insert(u);
                }
    tmat mtx(vx.size(), vc<i64>(S, 0));
    assert(vx.size() == S);
    for (i = 0; i < vx.size(); ++i)
        for (j = 0; j < mtx[i].size(); ++j)
            if ((vx[i] & 0xf) == (vx[j] >> 2))
                mtx[i][j] = 1;
    for (C.resize(A), C[0] = mtx, i = 1; i < A; C[i] = mpw(mtx, 1u << (i*W)), ++i);
    for (t = 0; t < A; ++t)
        for (D[t].resize(W), D[t][0] = C[t], i = 1; i < W; ++i)
            D[t][i] = mult(D[t][i - 1], D[t][i - 1]);

    for (t = 0; t < A; ++t)
        prt[t].resize(B(10)), prt[t][0] = im(S);

    for (is>>ts;ts--;){
        is >> n;
        if ( n == 0 )
            os<<1<<'\n';
        else if ( n==1 )
            os<<CLR<<'\n';
        else if (n==2)
            os<<CLR*CLR<<'\n';
        else
            os<<ga(n-3)<<'\n';
    }
    return 0;
}
    