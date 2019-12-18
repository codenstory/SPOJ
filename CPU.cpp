/**
 * TOPIC: ad hoc, Mobius function, sieve
 * status: WIP
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define N (2000000001)
#define M (1<<16)

bitset<M> is_prime;

#if 0
bitset<N> b;

void mobius() {
    i64 i,j,k;
    for ( i= 2; i < N; ++i )
        b[i]= true;
    for ( i= 2; i < N; ++i )
        if ( b[i] )
            for ( j= i+i; j < N; j+= i ) {
                k= j/i;
                b[j]= (b[j] and b[k] and (0 != k%i));
            }
    for ( k= 0, i= 2; i < N; ++i )
        if ( b[i] ) {
            std::cout << i << std::endl;
        }
    cerr << k << endl;
}
#endif

vector<i64> p;

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
    i64 i,j,k,ts;
#ifndef ONLINE_JUDGE
    //freopen("input.txt","r",stdin);
#endif
    for ( is_prime[2]= true, i= 3; i < M; is_prime[i]= true, i+= 2 ) ;
    for ( i= 3; i < M; i+= 2 )
        if ( is_prime[i] )
            for ( j= i*i; j < M; is_prime[j]= false, j+= i ) ;
    for ( i= 2; i < M; ++i )
        if ( is_prime[i] )
            p.push_back(i);
    for (;cin>>k;) {
        vector<i64> q;
        auto candidate= k;
        for ( i= 0; p[i] <= k/p[i]; ++i )
            if ( not (k%p[i]) ) {
                auto kk= k/p[i];
                if ( not (kk % p[i]) ) {
                    std::cerr << k << " and " << p[i] << std::endl;
                }
                assert( kk % p[i] );
                q.push_back(p[i]);
                k= kk;
            }
        if ( k > 1 )
            q.push_back(k);
        if ( q.size() >= 2 ) {
            auto y= q[1], x= q[0];
            assert( y > x );
            if ( (y-x) % (x-1) )
                goto next;
            auto A= (y-x)/(x-1);
            auto B= x-A;
            assert( y == A*x+B );
            for ( i= 2; i < q.size(); ++i )
                if ( A*q[i-1]+B != q[i] )
                    goto next;
        }
        std::cout << candidate << std::endl;
        next: continue ;
    }
}

