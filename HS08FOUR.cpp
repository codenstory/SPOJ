#include <bits/stdc++.h>
#define A 4
#define M 1000000007ll
using i64= int;
template<typename T>
using vc= std::vector<T>;
using tmat= vc<vc<i64>>;

std::istream &is = std::cin;
std::ostream &os = std::cout;

vc<unsigned> vx;
std::unordered_set<unsigned> seen;

tmat mult(  tmat &x,  tmat &y ) {
    tmat m=x,tmp=x;
    int i,j,k,N=x.size();
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

struct pi {
    std::array<unsigned char,4> p;
    int n;
    void set_n( i64 n ) {
        this->n= n;
        if ( n < 0 ) return ;
        unsigned int u= n;
        for ( int t= 0; t < 4; ++t )
            p[t]= (u&MK(8)), u>>=8u;
    }
};

struct cell {
    std::map<unsigned char,std::shared_ptr<cell>> s;
    tmat v;
};

void push(std::shared_ptr<cell> &root,pi &p) {
    std::shared_ptr<cell> *hold= &root, x= root;
    for ( int t=0; t<A; ++t) {
        if (not x->s.count(p.p[t]))
            (*hold)->s[p.p[t]]= x->s[p.p[t]]=std::make_shared<cell>();
        hold=&x->s[p.p[t]],x=x->s[p.p[t]];
    }
}

void traverse( std::shared_ptr<cell> &x, int t ) {
    if(not x or t==A) return ;
    assert(not x->v.empty());
    for(auto &[key,val]:x->s) {
        if (val->v.empty())
            val->v=mult(x->v,prt[t][key]);
        traverse(val,t+1);
    }
}

tmat &lookup(std::shared_ptr<cell> &root,pi &p) {
    auto x=root;
    for(int t=0;t<A;x=x->s[p.p[t++]]);
    return x->v;
}

pi T[100100];

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,t,ts,cs= 0,n,ans;
    for ( i= 0; i < 11; ++i )
        which[1<<i]= i;
    vx.reserve(43);
    for ( i= 0; i < A; ++i )
        for ( j= 0; j < A; ++j )
            for ( k= 0; k < A; ++k )
                if ( (i != j and j != k and k != i) or (i == 0 or j == 0 or k == 0) ) {
                    unsigned u= (i|(j<<2)|(k<<4));
                    if ( not seen.count(u) )
                        vx.emplace_back(u), seen.insert(u);
                }

    vc<vc<i64>> mtx(vx.size(),vc<i64>(vx.size(),0));
    for ( i= 0 ;i < vx.size(); ++i )
        for ( j= 0; j < mtx[i].size(); ++j )
            if ( (vx[i]&0xf) == (vx[j]>>2) )
                mtx[i][j]=1;
    std::cerr << "Here" << std::endl;
    for(C.resize(A),C[0]=mtx,i=1;i<A;C[i]=mpw(mtx,1u<<(i*8)),++i);
    for(t=0;t<A;++t)
        for(D[t].resize(8),D[t][0]=C[t],i=1;i<8;++i)
            D[t][i]=mult(D[t][i-1],D[t][i-1]);
    for (t=0;t<4;++t) {
        prt[t].resize(B(8)),prt[t][0]=im(vx.size());
        for (unsigned u=1;u<B(8);++u) {
            auto v=u&~L(u);
            prt[t][u]=mult(prt[t][v],D[t][which[L(u)]]);
        }
    }

    for (is>>ts;ts--;is>>n,T[cs++].set_n(n-3));

    auto root= std::make_shared<cell>();
    root->v= im(vx.size());
    for ( ts= 0; ts < cs; ++ts )
        if ( T[ts].n+3 >= 0 )
            push(root,T[ts]);
    traverse(root,0);

    for ( ts= 0; ts < cs; ++ts ) {
        if ( T[ts].n+3 == 0 )
            os << 1 << '\n';
        else if ( T[ts].n+3 == 1 )
            os << A << '\n';
        else if ( T[ts].n+3 == 2 )
            os << A*A << '\n';
        else {
            auto &ref= lookup(root, T[ts]);
            for(ans=0,i=0;i<ref.size();++i)
                for(j=0;j<ref.size();++j)
                    if((ans+=ref[i][j])>=M)
                        ans-=M;
            os<<ans<<'\n';
        }
    }
    return 0;
}
    