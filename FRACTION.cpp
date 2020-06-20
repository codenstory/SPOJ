/**
 * FRACTION
 * TOPIC: Farey sequence
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { L, R };
#define N (5001)

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
    return dp[y][x]= dp[x][y]= static_cast<int>(g);
}

std::shared_ptr<cell> build( i64 a, i64 b, i64 c, i64 d, const i64 n, std::vector<std::shared_ptr<cell>> &leaves ) {
    std::shared_ptr<cell> x= std::make_shared<cell>();
    x->a= a, x->b= b, x->c= c, x->d= d, x->card= 2;
    assert( b <= n and d <= n );
    auto A= a+c, B= b+d, g= get_gcd(A,B);
    A/= g, B/= g;
    if ( g < N and B <= n ) {
        x->son[L]= build(a,b,A,B,n,leaves), x->son[R]= build(A,B,c,d,n,leaves);
        x->son[L]->parent= x->son[R]->parent= x;
        x->card+= (x->son[L]->card-2)+(x->son[R]->card-2)+1;
    } else {
        leaves.push_back(x);
    }
    return x;
}

void update_upwards( std::shared_ptr<cell> &x ) {
    auto z= x;
    for ( ;z->parent; ) {
        auto p= z->parent;
        assert( p->son[L] and p->son[R] );
        p->card= 2+(p->son[L]->card-2)+(p->son[R]->card-2)+1;
        z= p;
    }
}

void expand( std::shared_ptr<cell> &x, const i64 n, std::vector<std::shared_ptr<cell>> &new_leaves ) {
    i64 A= x->a+x->c, B= x->b+x->d, g= get_gcd(A,B);
    A/= g, B/= g;
    if ( g < N and B <= n ) {
        x->son[L]= build(x->a,x->b,A,B,n,new_leaves), x->son[R]= build(A,B,x->c,x->d,n,new_leaves);
        x->son[L]->parent= x->son[R]->parent= x;
        x->card+= (x->son[L]->card-2)+(x->son[R]->card-2)+1;
        update_upwards(x);
    } else {
        new_leaves.push_back(x);
    }
}

std::pair<i64,i64> f( const std::shared_ptr<cell> &x, i64 k, std::shared_ptr<cell> &z, i64 &rk ) {
    auto y= x;
    assert( x );
    if ( y->card <= k ) {
        assert( x->parent != nullptr );
        for ( y= x; y and y->card <= k; y= y->parent )
            if ( y->parent->son[R] and y->parent->son[R] == y )
                k+= y->parent->son[L]->card, --k;
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

std::vector<std::pair<int,int>> requests;
std::vector<std::vector<std::pair<i64,i64>>> responses;
std::vector<std::vector<std::pair<i64,int>>> blocks;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    i64 i,k,n;
    size_t ts,qr;
    memset(dp,-1,sizeof dp);
    is >> ts, blocks.resize(ts), requests.resize(ts);
    responses.resize(ts);
    for ( int t= 0; t < ts; ++t ) {
        is >> n >> qr, blocks[t].resize(qr), responses[t].resize(qr);
        requests[t]= {n,t};
        auto &questions= blocks[t];
        i= 0;
        for ( auto &z: questions )
            is >> z.first, z.second= static_cast<int>(i++);
        std::sort(questions.begin(),questions.end());
    }
    std::sort(requests.begin(),requests.end());
    std::shared_ptr<cell> T= nullptr;
    std::vector<std::shared_ptr<cell>> leaves{};
    for ( auto &req: requests ) {
        auto request_id= req.second;
        n= req.first;
        if ( not T ) {
            T= build(0,1,1,1,n,leaves);
        } else {
            std::vector<std::shared_ptr<cell>> vec{};
            for ( auto &x: leaves ) {
                expand(x,n,vec);
            }
            leaves= std::move(vec);
        }
        assert( T );
        std::shared_ptr<cell> x= nullptr, y= nullptr;
        qr= blocks[request_id].size();
        for ( k= 0, i= 0; i < qr; ++i ) {
            auto &questions= blocks[request_id];
            auto z= questions[i];
            auto res= f(i==0?T:x,i==0?z.first-1:z.first-questions[i-1].first+k,y,k);
            assert( z.second < qr and z.second >= 0 );
            responses[request_id][z.second]= res, x= y;
        }
    }

    std::cerr << T->card << std::endl;

    for ( auto &resp: responses )
        for ( auto &z: resp )
            os << z.first << '/' << z.second << '\n';

    return 0;
}
    