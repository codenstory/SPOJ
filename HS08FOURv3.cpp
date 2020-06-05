/**
 * status: Accepted
 */
#include <bits/stdc++.h>
#define CLR 4
#define S (5)
#define A 3
#define W (10)
#define M 1000000007ll
using i64= std::int64_t;
template<typename T>
using vc= std::vector<T>;
using tmat= vc<vc<i64>>;
#define forn(i,n) for(i=0;i<n;++i)

std::istream &is = std::cin;
std::ostream &os = std::cout;

tmat mult(const tmat &x, const tmat &y) {
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
                    if ( (tmp[i][j]-=M)>= M )
                        tmp[i][j]%=M;
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
    for(;n;n>>=1u,x=mult(x,x))
        if(n&1)ax=mult(ax,x);
    return ax;
}

unsigned char which[1<<11];
#define L(k) ((k) & ((~(k))+1ul))

tmat &get_d( int t, int i ) {
    if ( D[t][i].empty() ) {
        get_d(t,i-1);
        D[t][i] = mult(D[t][i-1],D[t][i-1]);
    }
    return D[t][i];
}

tmat &get_p( int t, unsigned u ) {
    if ( prt[t][u].empty() ) {
        assert( u >= 1 );
        auto v=u&~L(u);
        assert( v < u );
        prt[t][u]=mult(get_p(t,v),get_d(t,which[L(u)]));
        assert( not prt[t][u].empty() );
    }
    return prt[t][u];
}

tmat slv(unsigned u) {
    auto r= get_p(0,u&MK(W));
    for(int i=1;i<A and (u>>=W);++i)
        r=mult(r,get_p(i,u&MK(W)));
    return r;
}

#define compat(i,j,k) (((i)!=(j)and(j)!=(k)and(k)!=(i))or((i)*(j)*(k)==0))

int wgt[]= {1,3,3,6,3};
vc<vc<i64>> mtx= {{1,3,0,0,0},
                  {0,0,1,2,1},
                  {1,3,0,0,0},
                  {0,0,1,1,0},
                  {0,0,1,0,0}};

int ga(unsigned n) {
    int i,j;
    i64 ans = 0;
    auto res = slv(n-2);
    forn(i,S)forn(j,S)
        if( (ans+= ((wgt[i]*res[i][j])%M) ) >= M )
            if ((ans -= M) >= M)
                ans %= M;
    return ans;
}

std::unordered_map<unsigned,int> cm;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,t,ts,n;
    for (i = 0; i < 11; ++i)
        which[1 << i] = i;

    for (C.resize(A), C[0] = mtx, i = 1; i < A; C[i] = mpw(C[i-1],1u<<W), ++i);
    for (t = 0; t < A; ++t)
        D[t].resize(W), D[t][0] = C[t];
    for (t = 0; t < A; ++t)
        prt[t].resize(B(W)), prt[t][0] = im(S);
    for (is>>ts;ts--;){
        is >> n;
        if ( n == 0 )
            os<<1<<'\n';
        else if ( n==1 )
            os<<CLR<<'\n';
        else if (n==2)
            os<<CLR*CLR<<'\n';
        else if (n==3)
            os<<43<<'\n';
        else
            os<<ga(n)<<'\n';
    }
    return 0;
}
    