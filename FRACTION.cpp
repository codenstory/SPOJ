/**
 * FRACTION
 * TOPIC: Farey sequence
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { L, R };
#define N (5001)

std::vector<std::pair<i64,int>> questions;

int dp[N][N];

struct cell {
    std::shared_ptr<cell> son[2],parent;
    int a,b,c,d,card;
    cell() { son[L]= son[R]= parent= nullptr; }
};

i64 gcd(i64 x, i64 y) { return not y?x:gcd(y,x%y); }

i64 get_gcd( i64 x, i64 y ) {
    if ( x < N and y < N and dp[x][y] >= 0 )
        return dp[x][y];
    auto g= gcd(x,y);
    if ( x >= N or y >= N ) return g;
    return dp[y][x]= dp[x][y]= g;
}

std::shared_ptr<cell> build( i64 a, i64 b, i64 c, i64 d, const i64 n ) {
    std::shared_ptr<cell> x= std::make_shared<cell>();
    x->a= a, x->b= b, x->c= c, x->d= d;
    x->card= 2;
    assert( b <= n and d <= n );
    auto A= a+c, B= b+d, g= get_gcd(A,B);
    A/= g, B/= g;
    if ( g < N and B <= n ) {
        x->son[L]= build(a,b,A,B,n), x->son[R]= build(A,B,c,d,n);
        x->son[L]->parent= x->son[R]->parent= x;
        x->card+= (x->son[L]->card-2)+(x->son[R]->card-2)+1;
    }
    return x;
}

std::pair<i64,i64> f( const std::shared_ptr<cell> &x, i64 k, std::shared_ptr<cell> &z, i64 &rk ) {
    auto y= x;
    assert( x );
    if ( y->card <= k ) {
        assert( x->parent != nullptr );
        for ( y= x; y and y->card <= k; ) {
            auto pp= y->parent;
            if ( pp->son[R] and pp->son[R] == y )
                k+= pp->son[L]->card, --k;
                //k+= pp->son[L]->card;
            y= pp;
        }
        assert( y and y->card > k );
    }
    assert( y->card > k );
    if ( k == 0 )
        return z= y, rk= k, std::make_pair(y->a,y->b);
    if ( k == y->card-1 )
        return z= y, rk= k, std::make_pair(y->c,y->d);
    if ( y->son[L] and y->son[L]->card > k )
        return f(y->son[L],k,z,rk);
    assert( y->son[R] );
    return f(y->son[R],k-y->son[L]->card+1,z,rk);
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    i64 i,j,k,ts,cs= 0,qr,n;
    memset(dp,-1,sizeof dp);
    for ( is >> ts; ts--; ) {
        is >> n >> qr, questions.resize(qr);
        i= 0;
        for ( auto &z: questions )
            is >> z.first, z.second= i++;
        std::sort(questions.begin(),questions.end());
        std::vector<std::pair<i64,i64>> ans(qr);
        auto root= build(0,1,1,1,n);
        std::shared_ptr<cell> x= nullptr, y= nullptr;
        for ( k= 0, i= 0; i < qr; ++i ) {
            auto z= questions[i];
            auto res= f(i==0?root:x,i==0?z.first-1:z.first-questions[i-1].first+k,y,k);
            assert( z.second < qr and z.second >= 0 );
            ans[z.second]= res, x= y;
        }
        for ( auto z: ans )
            os << z.first << '/' << z.second << '\n';
    }
    return 0;
}
    